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
  ConstructCryostat(detector);

  G4RotationMatrix* xRot = new G4RotationMatrix;
  xRot->rotateX(pi/2);
  new G4PVPlacement(xRot, G4ThreeVector(), fVolCryoOuterWall, detector->GetLV()->GetName()+"_pos1", volWorld, false, 0);
}

void Cryostat::ConstructCryostat(Detector* detector)
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

  std::vector<G4double> cryostatDim = config->CryostatDimensions(); util.ConvertToUnits(cryostatDim);
  G4double cryoInnerR             = cryostatDim[0];
  G4double cryoInnerDepth         = cryostatDim[1];
  G4double cryoInnerWallThickness = cryostatDim[2];
  G4double cryoWallGap            = cryostatDim[3];
  G4double cryoOuterR             = cryostatDim[4];
  G4double cryoOuterDepth         = cryostatDim[5];
  G4double cryoOuterWallThickness = cryostatDim[6];
  for (auto d : cryostatDim) std::cout << d << std::endl;

  // repeated variables
  G4double bowlR;    
  G4double sinTheta; 
  G4double cosTheta;
  G4double y;       
  G4double h;       
  G4double x;   
  G4double theta;

  //****
  // Inner bath
  //****
  bowlR    = 2*cryoInnerR;  // bowl radius of curvature (arbitrarly chosen)
  sinTheta = cryoInnerR/bowlR;
  cosTheta = std::sqrt(1 - sinTheta*sinTheta);
  y        = bowlR*(1 - cosTheta);
  h        = cryoInnerDepth - y;
  x        = bowlR - 0.5*h - y;
  theta    = std::abs(std::asin(sinTheta)*180/pi);

  G4Tubs* solCryoInnerBathTub = new G4Tubs("solCryoInnerBathTub",
                                            0,
                                            cryoInnerR,
                                            h/2.,
                                            0*degree, 360*degree);

  G4Sphere* solCryoInnerBathCap = new G4Sphere("solCryoInnerBathCap",
                                                0,
                                                bowlR,
                                                0*degree, 360*degree,
                                                (180-theta)*degree, 180*degree);

  G4UnionSolid* solCryoInnerBath = new G4UnionSolid("solCryoInnerBath", 
                                                     solCryoInnerBathCap, 
                                                     solCryoInnerBathTub,
                                                     0,
                                                     G4ThreeVector(0,0,-1*x));
  fVolCryoInnerBath = new G4LogicalVolume(solCryoInnerBath,
                                          matMan->FindMaterial("LAr"),
                                          "volCryoInnerBath");

  // Go ahead and place our detector
  G4RotationMatrix* xRot = new G4RotationMatrix;
  xRot->rotateX(3*pi/2);
  G4LogicalVolume* volMod = detector->GetLV();
  G4double modHeight = ((G4Box*)volMod->GetSolid())->GetYHalfLength();
  G4double shift = modHeight - (cryoInnerDepth-bowlR);
  G4ThreeVector transl(0, 0, -1*shift);
  new G4PVPlacement(xRot, transl, detector->GetLV(), detector->GetLV()->GetName()+"_pos1", fVolCryoInnerBath, false, 0);                                          

  //****
  // Inner wall
  //****
  bowlR    = 2*(cryoInnerR+cryoInnerWallThickness);  // bowl radius of curvature (arbitrarly chosen)
  sinTheta = (cryoInnerR+cryoInnerWallThickness)/bowlR;
  cosTheta = std::sqrt(1 - sinTheta*sinTheta);
  y        = bowlR*(1 - cosTheta);
  h        = (cryoInnerDepth+cryoInnerWallThickness) - y;
  x        = bowlR - 0.5*h - y;
  theta    = std::abs(std::asin(sinTheta)*180/pi);

  G4Tubs* solCryoInnerWallTub = new G4Tubs("solCryoInnerWallTub",
                                            0,
                                            cryoInnerR+cryoInnerWallThickness,
                                            h/2.,
                                            0*degree, 360*degree);

  G4Sphere* solCryoInnerWallCap = new G4Sphere("solCryoInnerWallCap",
                                                0,
                                                bowlR,
                                                0*degree, 360*degree,
                                                (180-theta)*degree, 180*degree);

  G4UnionSolid* solCryoInnerWall = new G4UnionSolid("solCryoInnerWall", 
                                                     solCryoInnerWallCap, 
                                                     solCryoInnerBathTub,
                                                     0,
                                                     G4ThreeVector(0,0,-1*x));
  fVolCryoInnerWall = new G4LogicalVolume(solCryoInnerWall,
                                          matMan->FindMaterial("LAr"),
                                          "volCryoInnerWall");

  new G4PVPlacement(0, transl, fVolCryoInnerBath, detector->GetLV()->GetName()+"_pos1", fVolCryoInnerWall, false, 0);                                           

  //****
  // Outer bath
  //****
  bowlR    = 2*cryoOuterR;  // bowl radius of curvature (arbitrarly chosen)
  sinTheta = cryoOuterR/bowlR;
  cosTheta = std::sqrt(1 - sinTheta*sinTheta);
  y        = bowlR*(1 - cosTheta);
  h        = cryoOuterDepth - y;
  x        = bowlR - 0.5*h - y;
  theta    = std::abs(std::asin(sinTheta)*180/pi);

  std::cout << y << " " << bowlR << " " << h << " " << cryoOuterDepth << std::endl;

  G4Tubs* solCryoOuterBathTub = new G4Tubs("solCryoOuterBathTub",
                                            0,
                                            cryoOuterR,
                                            h/2.,
                                            0*degree, 360*degree);

  G4Sphere* solCryoOuterBathCap = new G4Sphere("solCryoOuterBathCap",
                                                0,
                                                bowlR,
                                                0*degree, 360*degree,
                                                (180-theta)*degree, 180*degree);

  G4UnionSolid* solCryoOuterBath = new G4UnionSolid("solCryoOuterBath", 
                                                     solCryoOuterBathCap, 
                                                     solCryoOuterBathTub,
                                                     0,
                                                     G4ThreeVector(0,0,-1*x));
  fVolCryoOuterBath = new G4LogicalVolume(solCryoOuterBath,
                                          matMan->FindMaterial("LAr"),
                                          "volCryoOuterBath");

  new G4PVPlacement(0, transl, fVolCryoInnerWall, detector->GetLV()->GetName()+"_pos1", fVolCryoOuterBath, false, 0);                                          

  //****
  // Outer wall
  //****
  bowlR    = 2*(cryoOuterR+cryoOuterWallThickness);  // bowl radius of curvature (arbitrarly chosen)
  sinTheta = cryoOuterR/bowlR;
  cosTheta = std::sqrt(1 - sinTheta*sinTheta);
  y        = bowlR*(1 - cosTheta);
  h        = (cryoOuterR+cryoOuterWallThickness) - y;
  x        = bowlR - 0.5*h - y;
  theta    = std::abs(std::asin(sinTheta)*180/pi);

  G4Tubs* solCryoOuterWallTub = new G4Tubs("solCryoOuterWallTub",
                                            0,
                                            cryoOuterR+cryoOuterWallThickness,
                                            h/2.,
                                            0*degree, 360*degree);

  G4Sphere* solCryoOuterWallCap = new G4Sphere("solCryoOuterWallCap",
                                                0,
                                                bowlR,
                                                0*degree, 360*degree,
                                                (180-theta)*degree, 180*degree);

  G4UnionSolid* solCryoOuterWall = new G4UnionSolid("solCryoOuterWall", 
                                                     solCryoOuterWallCap, 
                                                     solCryoOuterWallTub,
                                                     0,
                                                     G4ThreeVector(0,0,-1*x));
  fVolCryoOuterWall = new G4LogicalVolume(solCryoOuterWall,
                                          matMan->FindMaterial("LAr"),
                                          "volCryoOuterWall");  

  new G4PVPlacement(0, transl, fVolCryoOuterBath, detector->GetLV()->GetName()+"_pos1", fVolCryoOuterWall, false, 0);                                          

}
}
