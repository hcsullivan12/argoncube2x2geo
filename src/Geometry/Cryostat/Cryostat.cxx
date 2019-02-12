// 
// File: Cryostat.cxx
//
// Author: Hunter Sullivan
//

#include "Geometry/Cryostat/Cryostat.h"
#include "Configuration.h"
#include "MaterialManager.h"

#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SubtractionSolid.hh"

namespace geo
{

Cryostat::Cryostat()
{}

Cryostat::~Cryostat()
{}

void Cryostat::ConstructVolume(G4LogicalVolume* volWorld,
                               Detector*        detector)
{
  ConstructSubVolumes(detector);
  PlaceSubVolumes(volWorld);
}

void Cryostat::ConstructSubVolumes(Detector* detector)
{
  fCryostatBody = new CryostatBody();
  fCryostatBody->ConstructVolume(detector);

  fCryostatLeg = new CryostatLeg();
  fCryostatLeg->ConstructVolume();

  fCryostatFT = new CryostatFT();
  fCryostatFT->ConstructVolume();

  MaterialManager* matMan = MaterialManager::Instance();
  Configuration* config = Configuration::Instance();
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();

  // Get the config parameters 
  G4double cryoLegOffset = config->CryostatLegOffset();
  G4double bodyH         = fCryostatBody->GetDepth();
  G4double bodyR         = fCryostatBody->GetOuterWallR();
  G4double legH          = fCryostatLeg->GetHeight();
  G4double ftH           = fCryostatFT->GetHeight();
  std::vector<G4double> moduleMedFTDim = config->ModuleMedFTDim(); 
  G4double innerR = moduleMedFTDim[0];
  G4double outerR = moduleMedFTDim[1];
  G4double height = moduleMedFTDim[2];
  
  // Flange object to wrap around cryostat
  G4Tubs* solCryotatFlangeWrap = new G4Tubs("solCryotatFlangeWrap",
                                             bodyR,
                                             bodyR+10*cm,
                                             10*cm,
                                             0*degree, 360*degree);

  fVolCryostatFlangeWrap = new G4LogicalVolume(solCryotatFlangeWrap,
                                              matMan->FindMaterial("SSteel304"),
                                              "volCryotatFlangeWrap");

  // Thin plate for the top of the cryostat
  // Let's subtract out the module's top wall
  G4Box*  solModTopContainer = (G4Box*)lvStore->GetVolume("volTopContainer")->GetSolid();
  G4Tubs* solCryostatFlange_whole = new G4Tubs("solCryostatFlange_whole",
                                          0*m,
                                          solCryotatFlangeWrap->GetRMax(),
                                          0.5*cm,
                                          0*degree, 360*degree);
  G4Box*  solCryostatFlange_subtract = new G4Box("solCryostatFlange_subtract",
                                           solModTopContainer->GetXHalfLength(),
                                           solModTopContainer->GetYHalfLength(),
                                           5*cm);                                     
  G4SubtractionSolid* solCryostatFlangeSub = new G4SubtractionSolid("solCryostatFlangeSub",
                                                                     solCryostatFlange_whole,
                                                                     solCryostatFlange_subtract,
                                                                     0,
                                                                     G4ThreeVector());
  fCryostatFlangeThickness = solCryostatFlange_whole->GetZHalfLength();                                                               
  fVolCryostatFlange = new G4LogicalVolume(solCryostatFlangeSub,
                                           matMan->FindMaterial("SSteel304"),
                                           "volCryostatFlange");                                      
  
  
  // It's easier to put cryostat feedthroughs here
  Feedthrough ft;
  fVolCryoMedFt = ft.ConstructVolume("CryoMedFT", innerR, outerR, height, "Steel"); 
  fVolCryoLgFt  = ft.ConstructVolume("CryoLgFT", innerR+2*cm, outerR+2*cm, height, "Steel");
  fCryoFTHeight = height;

  // Get the module fasteners 
  ModuleFastener modFast;
  fVolModFastener = modFast.ConstructVolume("ModuleFastener", "SSteel304");

  // Container for entire cryostat
  G4Tubs* solCryostatContainer = new G4Tubs("solCryostatContainer",
                                            0,
                                            1.5*m,
                                            (bodyH+legH+2*solCryostatFlange_whole->GetZHalfLength()+ftH-cryoLegOffset)/2.,
                                            0*degree, 360*degree);

  fVolCryostatContainer = new G4LogicalVolume(solCryostatContainer,
                                              matMan->FindMaterial("Air"),
                                              "volCryostatContainer"); 
}

void Cryostat::PlaceSubVolumes(G4LogicalVolume* volWorld)
{
  Configuration* config = Configuration::Instance();

  // We need the leg offset 
  G4double cryoLegOffset = config->CryostatLegOffset(); 

  // Compute shifts
  // Special case
  G4LogicalVolume* volCryoFT = fCryostatFT->GetLV();
  G4double motherHDim = ((G4Tubs*)fVolCryostatContainer->GetSolid())->GetZHalfLength();

  std::vector<G4double> geomsDim = { ((G4Box*)volCryoFT->GetSolid())->GetZHalfLength(),
                                     fCryostatFlangeThickness,
                                     ((G4Tubs*)fVolCryostatFlangeWrap->GetSolid())->GetZHalfLength(),
                                     fCryostatBody->GetOuterWallTubDepth()/2.,
                                     fCryostatLeg->GetHeight()/2. };
  std::vector<G4double> shift = { motherHDim -   geomsDim[0],
                                  motherHDim - 2*geomsDim[0] +   geomsDim[1],
                                  motherHDim - 2*geomsDim[0] /*- 2*geomsDim[1]*/ - geomsDim[2],
                                  motherHDim - 2*geomsDim[0] /*- 2*geomsDim[1]*/ - geomsDim[3],
                               -1*motherHDim + geomsDim[4] };

  // Place flange
  new G4PVPlacement(0, G4ThreeVector(0,0,shift[0]), volCryoFT, volCryoFT->GetName(),                           fVolCryostatContainer, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0,0,shift[1]), fVolCryostatFlange, fVolCryostatFlange->GetName(),         fVolCryostatContainer, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0,0,shift[2]), fVolCryostatFlangeWrap, fVolCryostatFlangeWrap->GetName(), fVolCryostatContainer, false, 0);
  // Place body
  new G4PVPlacement(0, G4ThreeVector(0,0,shift[3]), fCryostatBody->GetLV(), fCryostatBody->GetLV()->GetName(), fVolCryostatContainer, false, 0);
  // Place cryo FTs 
  G4double x = 1*m;
  G4double y = 15*cm;
  G4double z = shift[1]+geomsDim[1]+fCryoFTHeight/2.;
  new G4PVPlacement(0, G4ThreeVector(   x,   y,z), fVolCryoMedFt, fVolCryoMedFt->GetName(), fVolCryostatContainer, false, 0);
  new G4PVPlacement(0, G4ThreeVector(   x,-1*y,z), fVolCryoLgFt, fVolCryoMedFt->GetName(),  fVolCryostatContainer, false, 0);
  new G4PVPlacement(0, G4ThreeVector(-1*x,-1*y,z), fVolCryoMedFt, fVolCryoMedFt->GetName(), fVolCryostatContainer, false, 0);
  new G4PVPlacement(0, G4ThreeVector(-1*x,   y,z), fVolCryoLgFt, fVolCryoMedFt->GetName(),  fVolCryostatContainer, false, 0);
  new G4PVPlacement(0, G4ThreeVector(   y,-1*x,z), fVolCryoMedFt, fVolCryoMedFt->GetName(), fVolCryostatContainer, false, 0);
  new G4PVPlacement(0, G4ThreeVector(-1*y,-1*x,z), fVolCryoLgFt, fVolCryoMedFt->GetName(),  fVolCryostatContainer, false, 0);
  new G4PVPlacement(0, G4ThreeVector(-1*y,   x,z), fVolCryoMedFt, fVolCryoMedFt->GetName(), fVolCryostatContainer, false, 0);
  new G4PVPlacement(0, G4ThreeVector(   y,   x,z), fVolCryoLgFt, fVolCryoMedFt->GetName(),  fVolCryostatContainer, false, 0);

  // Place module fasteners
  //new G4PVPlacement(0, G4ThreeVector(0,70*cm,z), fVolModFastener, fVolModFastener->GetName(),  fVolCryostatContainer, false, 0);

  // Place all legs 
  unsigned nLegs(5);
  G4double r = fCryostatBody->GetOuterWallR()+fCryostatLeg->GetShinR();
  for (unsigned l = 1; l <= nLegs; l++)
  {
    G4double thetaDiff = (360/nLegs)*degree;
    G4double theta = (l-1)*thetaDiff;

    G4double x = r*std::cos(theta);
    G4double y = r*std::sin(theta);

    G4ThreeVector pos(x,y,shift[4]);
    new G4PVPlacement(0, pos, fCryostatLeg->GetLV(), fCryostatLeg->GetLV()->GetName()+std::to_string(l), fVolCryostatContainer, false, l-1);
  }

  // Place in world
  G4RotationMatrix* xRot2 = new G4RotationMatrix;
  xRot2->rotateX(pi/2);
  new G4PVPlacement(xRot2, G4ThreeVector(), fVolCryostatContainer, fVolCryostatContainer->GetName(), volWorld, false, 0);
}

}
