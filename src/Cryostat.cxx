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
  ConstructTubs();
  ConstructCaps();
  ConstructFlange();

  //****
  // Placement
  //****
  // Go ahead and place our detector
  G4RotationMatrix* xRot1 = new G4RotationMatrix;
  xRot1->rotateX(3*pi/2);
  G4LogicalVolume* volMod = detector->GetLV();
  G4double modHeight = ((G4Box*)volMod->GetSolid())->GetYHalfLength();
  G4double tubHeight = ((G4Tubs*)fVolCryoInnerBathTub->GetSolid())->GetZHalfLength();
  G4double shift = tubHeight - modHeight;
  G4ThreeVector transl(0, 0, shift);
  new G4PVPlacement(xRot1, transl, detector->GetLV(), detector->GetLV()->GetName()+"_pos1", fVolCryoInnerBathTub, false, 0);  

  PlaceVolumes();

  //ConstructCryostat(detector);

  G4RotationMatrix* xRot2 = new G4RotationMatrix;
  xRot2->rotateX(pi/2);
  new G4PVPlacement(xRot2, G4ThreeVector(), fVolCryoInnerBathTub, detector->GetLV()->GetName()+"_pos1", volWorld, false, 0);
  new G4PVPlacement(xRot2, G4ThreeVector(), fVolCryoInnerBathCap, fVolCryoInnerBathCap->GetName()+"_pos1", volWorld, false, 0);

}

void Cryostat::ConstructTubs()
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

  G4Tubs* solCryoInnerBathTub = new G4Tubs("solCryoInnerBathTub",
                                            0,
                                            cryoInnerR,
                                            cryoInnerDepth/2.,
                                            0*degree, 360*degree);

  fVolCryoInnerBathTub = new G4LogicalVolume(solCryoInnerBathTub,
                                             matMan->FindMaterial("LAr"),
                                             "volCryoInnerBathTub");                                            

  G4Tubs* solCryoInnerWallTub = new G4Tubs("solCryoInnerWallTub",
                                            0,
                                            cryoInnerR+cryoInnerWallThickness,
                                            cryoInnerDepth/2.,
                                            0*degree, 360*degree);  

  fVolCryoInnerWallTub = new G4LogicalVolume(solCryoInnerWallTub,
                                             matMan->FindMaterial("SSteel304"),
                                             "volCryoInnerWallTub");                                                                                      

  G4Tubs* solCryoOuterBathTub = new G4Tubs("solCryoOuterBathTub",
                                            0,
                                            cryoOuterR,
                                            cryoOuterDepth/2.,
                                            0*degree, 360*degree);

  fVolCryoOuterBathTub = new G4LogicalVolume(solCryoOuterBathTub,
                                             matMan->FindMaterial("LAr"),
                                             "volCryoOuterBathTub");                                            

  G4Tubs* solCryoOuterWallTub = new G4Tubs("solCryoOuterWallTub",
                                            0,
                                            cryoOuterR+cryoOuterWallThickness,
                                            cryoOuterDepth/2.,
                                            0*degree, 360*degree); 

  fVolCryoOuterWallTub = new G4LogicalVolume(solCryoOuterWallTub,
                                             matMan->FindMaterial("SSteel304"),
                                             "volCryoOuterWallTub");

}

void Cryostat::ConstructCaps()
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
  y        = bowlR*(1 - cosTheta);
  theta    = std::abs(std::asin(sinTheta)*180/pi);
  
  G4Sphere* solCryoInnerBathCap = new G4Sphere("solCryoInnerBathCap",
                                                0,
                                                bowlR,
                                                0*degree, 360*degree,
                                                (180-theta)*degree, 180*degree);

  fVolCryoInnerBathCap = new G4LogicalVolume(solCryoInnerBathCap,
                                             matMan->FindMaterial("LAr"),
                                             "volCryoInnerBathCap");                                         

}

void Cryostat::ConstructFlange()
{
  // NEEDS WORK!!

  // Get material manager and config
  MaterialManager* matMan = MaterialManager::Instance();
  Configuration* config = Configuration::Instance();
  arcutil::Utilities util;

  /*std::vector<G4double> cryoFlangeDim = config->CryostatFlangeDim(); util.ConvertToUnits(cryoFlangeDim);

  G4Tubs* solCryoFlange = new G4Tubs("solCryoFlange",
                                      0,
                                      cryoFlangeDim[1],
                                      cryoFlangeDim[2]/2.,
                                      0*degree, 360*degree); 

  fVolCryoFlange = new G4LogicalVolume(solCryoFlange,
                                       matMan->FindMaterial("SSteel304"),
                                       "volCryoFlange");                                  
*/
}

void Cryostat::PlaceVolumes()
{
  //****
  // Tubs
  //****
  std::vector<G4LogicalVolume*> vols = {fVolCryoInnerBathTub,
                                        fVolCryoInnerWallTub,
                                        fVolCryoOuterBathTub,
                                        fVolCryoOuterWallTub};
  std::vector<G4double> shifts  = { ((G4Tubs*)vols[1]->GetSolid())->GetZHalfLength() - ((G4Tubs*)vols[0]->GetSolid())->GetZHalfLength(),
                                    ((G4Tubs*)vols[2]->GetSolid())->GetZHalfLength() - ((G4Tubs*)vols[1]->GetSolid())->GetZHalfLength(),
                                    ((G4Tubs*)vols[3]->GetSolid())->GetZHalfLength() - ((G4Tubs*)vols[2]->GetSolid())->GetZHalfLength() };

  //new G4PVPlacement(0, G4ThreeVector(0,0,shifts[0]), fVolCryoInnerBathTub, fVolCryoInnerBathTub->GetName()+"_pos", fVolCryoInnerWallTub, false, 0);
  // new G4PVPlacement(0, G4ThreeVector(0,0,shifts[1]), fVolCryoInnerWallTub, fVolCryoInnerWallTub->GetName()+"_pos", fVolCryoOuterBathTub, false, 0);
  //new G4PVPlacement(0, G4ThreeVector(0,0,shifts[2]), fVolCryoOuterBathTub, fVolCryoOuterBathTub->GetName()+"_pos", fVolCryoOuterWallTub, false, 0);  

  //****
  // Caps
  //****
  //std::vector<G4LogicalVolume*> vols = {fVolCryoInnerBathCap};
  
  //new G4PVPlacement(0, G4ThreeVector(0,0,0), fVolCryoInnerBathCap, fVolCryoInnerBathTub->GetName()+"_pos", fVolCryoInnerWallTub, false, 0);


  /*
  G4Box* solCryoContainer = new G4Box("solCryoContainer",
                                       ((G4Tubs*)fVolCryoFlange->GetSolid())->GetRmax(),
                                       ((G4Tubs*)fVolCryoFlange->GetSolid())->GetZHalfLength()+ ((G4Box*)fVolCryoOuterWallTub->GetSolid())->GetZHalfLength(),
                                       ((G4Tubs*)fVolCryoFlange->GetSolid())->GetRmax());

  fVolCryoContainer = new G4LogicalVolume(solCryoContainer,
                                          matMan->FindMaterial("Air"),
                                          "volCryoContainer");
  


  std::vector<G4LogicalVolume*> geoms;
  std::vector<G4double> geomsDim;
  std::vector<G4ThreeVector> positions;
  std::vector<G4double> steps;

  geoms    = {fVolCryoOuterWallTub, fVolCryoFlange};
  geomsDim = {((G4Tubs*)geoms[0]->GetSolid())->GetZHalfLength(),
              ((G4Tubs*)geoms[1]->GetSolid())->GetZHalfLength()};
  steps = util.Stack(geomsDim, ((G4Box*)fVolCryoContainer->GetSolid())->GetYHalfLength());
  positions.resize(steps.size());
  for (unsigned s = 0; s < steps.size(); s++) positions[s] = G4ThreeVector(0,0,steps[s]);
  geoms.clear(); geomsDim.clear(); positions.clear(); steps.clear();                     
  */                     

}
}
