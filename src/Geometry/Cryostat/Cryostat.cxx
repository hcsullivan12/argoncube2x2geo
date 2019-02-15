// 
// File: Cryostat.cxx
//
// Author: Hunter Sullivan
//

#include "Geometry/Cryostat/Cryostat.h"
#include "QuantityStore.h"
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

void Cryostat::ConstructVolume(G4LogicalVolume* volDetEnclosure,
                               Detector*        detector)
{
  ConstructSubVolumes(detector);
  PlaceSubVolumes(volDetEnclosure);
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
  QuantityStore* qStore = QuantityStore::Instance();
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();

  // Get the qStore parameters 
  G4double cryoLegOffset = qStore->kCryostatLegOffset;
  G4double bodyH         = qStore->kCryoDepth;
  G4double bodyR         = qStore->kCryoOuterWallR;
  G4double legH          = qStore->kCryoLegHeight;
  G4double ftH           = qStore->kCryoFTHeight;
  std::vector<G4double> moduleMedFTDim = qStore->kModuleMedFTDim; 
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
  G4Tubs* solCryostat = new G4Tubs("solCryostat",
                                    0,
                                    1.5*m,
                                    (bodyH+legH+2*solCryostatFlange_whole->GetZHalfLength()+ftH-cryoLegOffset)/2.,
                                    0*degree, 360*degree);

  fVolCryostat = new G4LogicalVolume(solCryostat,
                                     matMan->FindMaterial("Air"),
                                     "volCryostat"); 
}

void Cryostat::PlaceSubVolumes(G4LogicalVolume* volDetEnclosure)
{
  QuantityStore* qStore = QuantityStore::Instance();

  // We need the leg offset 
  G4double cryoLegOffset = qStore->kCryostatLegOffset; 

  // Compute shifts
  // Special case
  G4LogicalVolume* volCryoFT = fCryostatFT->GetLV();
  G4double motherHDim = ((G4Tubs*)fVolCryostat->GetSolid())->GetZHalfLength();

  std::vector<G4double> geomsDim = { ((G4Box*)volCryoFT->GetSolid())->GetZHalfLength(),
                                     fCryostatFlangeThickness,
                                     ((G4Tubs*)fVolCryostatFlangeWrap->GetSolid())->GetZHalfLength(),
                                     qStore->kCryoOuterWallTubDepth/2.,
                                     qStore->kCryoLegHeight/2. };
  std::vector<G4double> shift = { motherHDim -   geomsDim[0],
                                  motherHDim - 2*geomsDim[0] +   geomsDim[1],
                                  motherHDim - 2*geomsDim[0] /*- 2*geomsDim[1]*/ - geomsDim[2],
                                  motherHDim - 2*geomsDim[0] /*- 2*geomsDim[1]*/ - geomsDim[3],
                               -1*motherHDim + geomsDim[4] };

  // Place flange
  new G4PVPlacement(0, G4ThreeVector(0,0,shift[0]), volCryoFT, volCryoFT->GetName(),                           fVolCryostat, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0,0,shift[1]), fVolCryostatFlange, fVolCryostatFlange->GetName(),         fVolCryostat, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0,0,shift[2]), fVolCryostatFlangeWrap, fVolCryostatFlangeWrap->GetName(), fVolCryostat, false, 0);
  // Place body
  new G4PVPlacement(0, G4ThreeVector(0,0,shift[3]), fCryostatBody->GetLV(), fCryostatBody->GetLV()->GetName(), fVolCryostat, false, 0);
  // Place cryo FTs 
  G4double x = 1*m;
  G4double y = 15*cm;
  G4double z = shift[1]+geomsDim[1]+fCryoFTHeight/2.;
  new G4PVPlacement(0, G4ThreeVector(   x,   y,z), fVolCryoMedFt, fVolCryoMedFt->GetName(), fVolCryostat, false, 0);
  new G4PVPlacement(0, G4ThreeVector(   x,-1*y,z), fVolCryoLgFt, fVolCryoMedFt->GetName(),  fVolCryostat, false, 0);
  new G4PVPlacement(0, G4ThreeVector(-1*x,-1*y,z), fVolCryoMedFt, fVolCryoMedFt->GetName(), fVolCryostat, false, 0);
  new G4PVPlacement(0, G4ThreeVector(-1*x,   y,z), fVolCryoLgFt, fVolCryoMedFt->GetName(),  fVolCryostat, false, 0);
  new G4PVPlacement(0, G4ThreeVector(   y,-1*x,z), fVolCryoMedFt, fVolCryoMedFt->GetName(), fVolCryostat, false, 0);
  new G4PVPlacement(0, G4ThreeVector(-1*y,-1*x,z), fVolCryoLgFt, fVolCryoMedFt->GetName(),  fVolCryostat, false, 0);
  new G4PVPlacement(0, G4ThreeVector(-1*y,   x,z), fVolCryoMedFt, fVolCryoMedFt->GetName(), fVolCryostat, false, 0);
  new G4PVPlacement(0, G4ThreeVector(   y,   x,z), fVolCryoLgFt, fVolCryoMedFt->GetName(),  fVolCryostat, false, 0);

  // Place module fasteners
  //new G4PVPlacement(0, G4ThreeVector(0,70*cm,z), fVolModFastener, fVolModFastener->GetName(),  fVolCryostat, false, 0);

  // Place all legs 
  unsigned nLegs(5);
  G4double r = qStore->kCryoOuterWallR+qStore->kCryoLegShinR;
  for (unsigned l = 1; l <= nLegs; l++)
  {
    G4double thetaDiff = (360/nLegs)*degree;
    G4double theta = (l-1)*thetaDiff;

    G4double x = r*std::cos(theta);
    G4double y = r*std::sin(theta);

    G4ThreeVector pos(x,y,shift[4]);
    // Instead of using copyID, add ID to name
    new G4PVPlacement(0, pos, fCryostatLeg->GetLV(), fCryostatLeg->GetLV()->GetName()+std::to_string(l), fVolCryostat, false, 0);
  }

  // Place in detector enclosure
  // Coordinate system is:
  //    z -> beam direction
  //    z = 0 at front of upstream TPCs
  //    y = 0 is middle of pixel plane
  //
  // FIX ME! Center in y!
  // Compute the shift in z
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  G4Box* solDetector = ((G4Box*)lvStore->GetVolume("volDetector")->GetSolid());
  G4LogicalVolume* volWorld = lvStore->GetVolume("volWorld");

  G4ThreeVector detShift(0,0,solDetector->GetZHalfLength());
  G4RotationMatrix* xRot2 = new G4RotationMatrix;
  xRot2->rotateX(pi/2);
  new G4PVPlacement(xRot2, detShift, fVolCryostat, fVolCryostat->GetName(), volDetEnclosure, false, 0);
  new G4PVPlacement(0, G4ThreeVector(), volDetEnclosure, volDetEnclosure->GetName(), volWorld, false, 0);
}

}
