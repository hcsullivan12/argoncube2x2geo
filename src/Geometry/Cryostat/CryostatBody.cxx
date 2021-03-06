// 
// File: CryostatBody.cxx
//
// Author: Hunter Sullivan
//

#include "Geometry/Cryostat/CryostatBody.h"
#include "QuantityStore.h"
#include "MaterialManager.h"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

namespace geo
{

CryostatBody::CryostatBody()
{}

CryostatBody::~CryostatBody()
{}

void CryostatBody::ConstructVolume(Detector* detector)
{
  ConstructSubVolumes();
  PlaceSubVolumes(detector);
}

void CryostatBody::ConstructSubVolumes()
{
  // Get material manager and qStore
  MaterialManager* matMan = MaterialManager::Instance();
  QuantityStore* qStore = QuantityStore::Instance();

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
  std::vector<G4double> cryostatDim = qStore->kCryostatDim;
  G4double cryoInnerR             = cryostatDim[0];
  G4double cryoInnerDepth         = cryostatDim[1];
  G4double cryoInnerWallThickness = cryostatDim[2];
  G4double cryoWallGap            = cryostatDim[3];
  G4double cryoOuterR             = cryostatDim[4];
  G4double cryoOuterDepth         = cryostatDim[5];
  G4double cryoOuterWallThickness = cryostatDim[6];

  // Create the baths and walls
  // Wall depths are assumed to be just the tub component
  // Total depth = tub + cap 
  G4double totalDepth(0);
  fCryoInnerBathTubDepth = cryoInnerDepth;
  G4UnionSolid* solCryoInnerBath = GetShape("solCryoInnerBath", cryoInnerR, fCryoInnerBathTubDepth, totalDepth);

  fCryoInnerWallTubDepth = cryoInnerDepth+cryoInnerWallThickness;
  G4UnionSolid* solCryoInnerWall = GetShape("solCryoInnerWall", cryoInnerR+cryoInnerWallThickness, fCryoInnerWallTubDepth, totalDepth);

  fCryoOuterBathTubDepth = cryoOuterDepth;
  G4UnionSolid* solCryoOuterBath = GetShape("solCryoOuterBath", cryoOuterR, fCryoOuterBathTubDepth, totalDepth);

  qStore->kCryoOuterWallTubDepth = cryoOuterDepth+cryoOuterWallThickness;
  qStore->kCryoOuterWallR     = cryoOuterR+cryoOuterWallThickness;
  qStore->kCryoDepth             = totalDepth;
  G4UnionSolid* solCryoOuterWall = GetShape("solCryoOuterWall", qStore->kCryoOuterWallR, qStore->kCryoOuterWallTubDepth, qStore->kCryoDepth);

  fVolCryoInnerBath = new G4LogicalVolume(solCryoInnerBath,
                                          matMan->FindMaterial("LAr"),
                                          "volCryoInnerBath");
  fVolCryoInnerWall = new G4LogicalVolume(solCryoInnerWall,
                                          matMan->FindMaterial("SSteel304"),
                                          "volCryoInnerWall");
  fVolCryoOuterBath = new G4LogicalVolume(solCryoOuterBath,
                                          matMan->FindMaterial("Vacuum"),
                                          "volCryoOuterBath");
  fVolCryoOuterWall = new G4LogicalVolume(solCryoOuterWall,
                                          matMan->FindMaterial("SSteel304"),
                                          "volCryoOuterWall");  
}

G4UnionSolid* CryostatBody::GetShape(const G4String& name, 
                                     const G4double& cryoR,
                                     const G4double& cryoD,
                                     G4double& totalDepth)
{                             
  // Tub component         
  G4Tubs* tub = new G4Tubs(name+"Tub",
                           0,
                           cryoR,
                           cryoD/2.,
                           0*degree, 360*degree);

  // Cap component
  G4double bowlR    = 2*cryoR;  // bowl radius of curvature 
  G4double sinTheta = cryoR/bowlR;
  G4double cosTheta = std::sqrt(1-sinTheta*sinTheta);
  G4double y        = bowlR*(1 - cosTheta);
  G4double x        = bowlR - 0.5*cryoD - y;
  G4double theta    = std::abs(std::asin(sinTheta)*180/pi);
  totalDepth = cryoD + y;
  
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

void CryostatBody::PlaceSubVolumes(Detector* detector)
{
  QuantityStore* qStore = QuantityStore::Instance();

  //****
  // Place our detector
  //****
  G4RotationMatrix* xRot1 = new G4RotationMatrix;
  xRot1->rotateX(3*pi/2);

  G4LogicalVolume* volMod = detector->GetLV();
  G4double modHeight = ((G4Box*)volMod->GetSolid())->GetYHalfLength();
  G4double shift = fCryoInnerBathTubDepth/2.0 - modHeight;
  G4ThreeVector transl(0, 0, shift);
  new G4PVPlacement(xRot1, transl, volMod, volMod->GetName(), fVolCryoInnerBath, false, 0); 

  //****
  // Cryostat layers
  //****
  std::vector<G4double> shifts  = {fCryoInnerWallTubDepth/2.0         - fCryoInnerBathTubDepth/2.0,
                                   fCryoOuterBathTubDepth/2.0         - fCryoInnerWallTubDepth/2.0,
                                   qStore->kCryoOuterWallTubDepth/2.0 - fCryoOuterBathTubDepth/2.0 };
                                   
  new G4PVPlacement(0, G4ThreeVector(0,0,shifts[0]), fVolCryoInnerBath, fVolCryoInnerBath->GetName(), fVolCryoInnerWall, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0,0,shifts[1]), fVolCryoInnerWall, fVolCryoInnerWall->GetName(), fVolCryoOuterBath, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0,0,shifts[2]), fVolCryoOuterBath, fVolCryoOuterBath->GetName(), fVolCryoOuterWall, false, 0);
}
}
