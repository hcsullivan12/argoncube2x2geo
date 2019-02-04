// 
// File: Cryostat.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to construct cryostat volume.
//

#include "Cryostat.h"
#include "Configuration.h"
#include "MaterialManager.h"
#include "Utilities.h"

#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"

namespace geo
{

Cryostat::Cryostat()
{}

Cryostat::~Cryostat()
{}

void Cryostat::ConstructVolume(G4LogicalVolume* volWorld,
                               Detector*        detector)
{

  //****
  // Construct cryostat volumes
  //****
  ConstructBody();
  ConstructLegs();
  ConstructFlange();

  //****
  // Placement
  //****
  PlaceVolumes(volWorld, detector);
}

void Cryostat::ConstructBody()
{
  // Get material manager and config
  MaterialManager* matMan = MaterialManager::Instance();
  Configuration* config = Configuration::Instance();
  arcutil::Utilities util;

  //**** 
  // 	Cryostat shape
  //****
  // Expecting (IDs not ODs):
  //     [0] --> inner radius          
  //     [1] --> inner depth
  //     [2] --> inner wall thickness
  //     [3] --> wall gap
  //     [4] --> outer radius
  //     [5] --> outer depth      
  //     [6] --> outer wall thickness
  std::vector<G4double> cryostatDim = config->CryostatDim(); util.ConvertToUnits(cryostatDim);
  G4double cryoInnerR             = cryostatDim[0];
  G4double cryoInnerDepth         = cryostatDim[1];
  G4double cryoInnerWallThickness = cryostatDim[2];
  G4double cryoWallGap            = cryostatDim[3];
  G4double cryoOuterR             = cryostatDim[4];
  G4double cryoOuterDepth         = cryostatDim[5];
  G4double cryoOuterWallThickness = cryostatDim[6];

  // Create the baths and walls
  fCryoInnerBathDepth = cryoInnerDepth;
  G4UnionSolid* solCryoInnerBath = CryostatShape("solCryoInnerBath", cryoInnerR, fCryoInnerBathDepth);

  fCryoInnerWallDepth = cryoInnerDepth+cryoInnerWallThickness;
  G4UnionSolid* solCryoInnerWall = CryostatShape("solCryoInnerWall", cryoInnerR+cryoInnerWallThickness, fCryoInnerWallDepth);

  fCryoOuterBathDepth = cryoOuterDepth;
  G4UnionSolid* solCryoOuterBath = CryostatShape("solCryoOuterBath", cryoOuterR, fCryoOuterBathDepth);

  fCryoOuterWallDepth = cryoOuterDepth+cryoOuterWallThickness;
  fVolCryoOuterWallR  = cryoOuterR+cryoOuterWallThickness;
  G4UnionSolid* solCryoOuterWall = CryostatShape("solCryoOuterWall", cryoOuterR+cryoOuterWallThickness, fCryoOuterWallDepth);

  fVolCryoInnerBath = new G4LogicalVolume(solCryoInnerBath,
                                          matMan->FindMaterial("LAr"),
                                          "volCryoInnerBath");
  fVolCryoInnerWall = new G4LogicalVolume(solCryoInnerWall,
                                          matMan->FindMaterial("SSteel304"),
                                          "volCryoInnerWall");
  fVolCryoOuterBath = new G4LogicalVolume(solCryoOuterBath,
                                          matMan->FindMaterial("LAr"),
                                          "volCryoOuterBath");
  fVolCryoOuterWall = new G4LogicalVolume(solCryoOuterWall,
                                          matMan->FindMaterial("SSteel304"),
                                          "volCryoOuterWall");
}

void Cryostat::ConstructLegs()
{     
  // Get material manager and config
  MaterialManager* matMan = MaterialManager::Instance();
  Configuration* config = Configuration::Instance();
  arcutil::Utilities util;

  std::vector<G4double> cryostatLegDim = config->CryostatLegDim(); util.ConvertToUnits(cryostatLegDim);
  std::vector<G4double> cryostatLegFootDim = config->CryostatLegFootDim(); util.ConvertToUnits(cryostatLegFootDim);

  G4Tubs* solCryoLegShin = new G4Tubs("solCryoLegShin",
                                       0,
                                       cryostatLegDim[1],
                                       cryostatLegDim[2]/2.,
                                       0*degree, 360*degree);
                                         
  G4Tubs* solCryoLegFoot = new G4Tubs("solCryoLegFoot",
                                       0,
                                       cryostatLegFootDim[1],
                                       cryostatLegFootDim[2]/2.,
                                       0*degree, 360*degree);

  G4ThreeVector transl(0, 0, solCryoLegShin->GetZHalfLength()-solCryoLegFoot->GetZHalfLength());
  G4UnionSolid* solCryoLeg = new G4UnionSolid("solCryoLeg",
                                               solCryoLegShin,
                                               solCryoLegFoot,
                                               0,
                                               -1*transl);

  fVolCryoLeg = new G4LogicalVolume(solCryoLeg,
                                    matMan->FindMaterial("SSteel304"),
                                    "volCryoLeg");
                                               

  // Place all four legs in container
  G4double legHeight = 2*(solCryoLegShin->GetZHalfLength()+solCryoLegFoot->GetZHalfLength());
  G4double offset = 10*cm; // this will do for now?
  G4Tubs* solCryoLegContainer = new G4Tubs("solCryoLegContainer",
                                           0,
                                           fVolCryoOuterWallR+2*solCryoLegShin->GetRMax()+offset,
                                           fCryoOuterWallDepth/2.0+legHeight/2.0 - offset,
                                           0*degree, 360*degree);
  fVolCryoLegContainer = new G4LogicalVolume(solCryoLegContainer,
                                             matMan->FindMaterial("LAr"),
                                             "volCryoLegContainer");

  unsigned nLegs(5);
  G4double r = fVolCryoOuterWallR+solCryoLegShin->GetRMax();
  G4double z = -1*solCryoLegContainer->GetZHalfLength()+legHeight/2.;
  for (unsigned l = 1; l <= nLegs; l++)
  {
    G4double thetaDiff = (360/nLegs)*degree;
    G4double theta = (l-1)*thetaDiff;

    G4double x = r*std::cos(theta);
    G4double y = r*std::sin(theta);

    G4ThreeVector pos(x,y,z);
    new G4PVPlacement(0, pos, fVolCryoLeg, fVolCryoLeg->GetName()+"_pos"+std::to_string(l), fVolCryoLegContainer, false, l-1);
  }
}

G4UnionSolid* Cryostat::CryostatShape(const G4String& name, 
                                      const G4double& cryoR,
                                      const G4double& cryoD)
{                             
  // Tub component         
  G4Tubs* tub = new G4Tubs(name+"Tub",
                           0,
                           cryoR,
                           cryoD/2.,
                           0*degree, 360*degree);

  // Cap component
  G4double bowlR    = 2*cryoR;  // bowl radius of curvature (arbitrarly chosen)
  G4double sinTheta = cryoR/bowlR;
  G4double cosTheta = std::sqrt(1-sinTheta*sinTheta);
  G4double y        = bowlR*(1 - cosTheta);
  G4double x        = bowlR - 0.5*cryoD - y;
  G4double theta    = std::abs(std::asin(sinTheta)*180/pi);
  
  G4Sphere* cap = new G4Sphere(name+"Cap",
                               0,
                               bowlR,
                               0*degree, 360*degree,
                               (180-theta)*degree, 180*degree); 

  // Union
  G4ThreeVector transl(0, 0, x);
  return new G4UnionSolid(name,
                          tub,
                          cap,
                          new G4RotationMatrix(0,0,0),
                          transl);                           
}

void Cryostat::ConstructFlange()
{
  // NEEDS WORK!!

  // Get material manager and config
  MaterialManager* matMan = MaterialManager::Instance();
  Configuration* config = Configuration::Instance();
  arcutil::Utilities util;

  std::vector<G4double> cryoFlangeDim = config->CryostatFlangeDim(); util.ConvertToUnits(cryoFlangeDim);

  G4Tubs* solCryoFlange = new G4Tubs("solCryoFlange",
                                      0,
                                      cryoFlangeDim[1],
                                      cryoFlangeDim[2]/2.,
                                      0*degree, 360*degree); 

  fVolCryoFlange = new G4LogicalVolume(solCryoFlange,
                                       matMan->FindMaterial("SSteel304"),
                                       "volCryoFlange");
}

void Cryostat::PlaceVolumes(G4LogicalVolume* volWorld,
                            Detector*        detector)
{
  //****
  // Place our detector
  //****
  G4RotationMatrix* xRot1 = new G4RotationMatrix;
  xRot1->rotateX(3*pi/2);

  G4LogicalVolume* volMod = detector->GetLV();
  G4double modHeight = ((G4Box*)volMod->GetSolid())->GetYHalfLength();
  G4double shift = fCryoInnerBathDepth/2.0 - modHeight;
  G4ThreeVector transl(0, 0, shift);

  new G4PVPlacement(xRot1, transl, detector->GetLV(), detector->GetLV()->GetName()+"_pos1", fVolCryoInnerBath, false, 0);  

  
  
  //****
  // Cryos
  //****
  std::vector<G4double> shifts  = {fCryoInnerWallDepth/2.0 - fCryoInnerBathDepth/2.0,
                                   fCryoOuterBathDepth/2.0 - fCryoInnerWallDepth/2.0,
                                   fCryoOuterWallDepth/2.0 - fCryoOuterBathDepth/2.0,
                                   ((G4Tubs*)fVolCryoLegContainer->GetSolid())->GetZHalfLength() - fCryoOuterWallDepth/2.0 };

  new G4PVPlacement(0, G4ThreeVector(0,0,shifts[0]), fVolCryoInnerBath, fVolCryoInnerBath->GetName()+"_pos", fVolCryoInnerWall, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0,0,shifts[1]), fVolCryoInnerWall, fVolCryoInnerWall->GetName()+"_pos", fVolCryoOuterBath, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0,0,shifts[2]), fVolCryoOuterBath, fVolCryoOuterBath->GetName()+"_pos", fVolCryoOuterWall, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0,0,shifts[3]), fVolCryoOuterWall, fVolCryoOuterWall->GetName()+"_pos1", fVolCryoLegContainer, false, 0);
  

  G4RotationMatrix* xRot2 = new G4RotationMatrix;
  xRot2->rotateX(pi/2);
  new G4PVPlacement(xRot2, G4ThreeVector(), fVolCryoLegContainer, fVolCryoLegContainer->GetName()+"_pos1", volWorld, false, 0);  

}
}
