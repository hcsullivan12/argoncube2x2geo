// 
// File: ModuleFlange.cxx
//
// Author: Hunter Sullivan
//

#include "Geometry/Module/ModuleFlange.h"
#include "Configuration.h"
#include "MaterialManager.h"
#include "Utilities.h"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolumeStore.hh"

namespace geo
{

ModuleFlange::ModuleFlange()
{}

ModuleFlange::~ModuleFlange()
{}

void ModuleFlange::ConstructVolume()
{
  // NEEDS WORK!

  // Get material manager and config
  MaterialManager* matMan       = MaterialManager::Instance();
  Configuration* config         = Configuration::Instance();
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  arcutil::Utilities util;

  std::vector<G4double> moduleMedFTDim = config->ModuleMedFTDim(); 
  G4double innerR = moduleMedFTDim[0];
  G4double outerR = moduleMedFTDim[1];
  G4double height = moduleMedFTDim[2];

  G4double moduleTopWallThickness      = config->ModuleTopWallThickness(); 
  G4Box* volModuleWall = (G4Box*)lvStore->GetVolume("volModuleWall")->GetSolid();

  //****
  // Top 
  //****
  G4Box* solModuleTopWall = new G4Box("solModuleTopWall",
                                       volModuleWall->GetXHalfLength(),
                                       moduleTopWallThickness/2.,
                                       volModuleWall->GetZHalfLength() );
  fVolModuleTopSolid = new G4LogicalVolume(solModuleTopWall,
                                           matMan->FindMaterial("SSteel304"),
                                           "volModuleTopSolid");

  // Container for feedthroughs
  G4Box* solModuleFTContainer = new G4Box("solModuleFTContainer",
                                           volModuleWall->GetXHalfLength(),
                                           height,
                                           volModuleWall->GetZHalfLength() );
  fVolModuleFTContainer = new G4LogicalVolume(solModuleFTContainer,
                                              matMan->FindMaterial("Air"),
                                              "volModuleFTContainer");
  
  // Medium sized feethrough
  G4Tubs* ftTub1 = new G4Tubs("solModuleFTTub1_",
                               innerR,
                               outerR,
                               solModuleFTContainer->GetYHalfLength(),
                               0*degree, 360*degree);
  G4Tubs* ftTub2 = new G4Tubs("solModuleFTTub2_",
                               0,
                               outerR+2*cm,
                               2*cm,
                               0*degree, 360*degree); 

  G4UnionSolid* solModuleMedFT = new G4UnionSolid("solModuleMedFT",
                                                   ftTub1,
                                                   ftTub2,
                                                   0,
                                                   G4ThreeVector(0,0,ftTub1->GetZHalfLength()-ftTub2->GetZHalfLength()) );
  fVolModuleMedFT = new G4LogicalVolume(solModuleMedFT,
                                        matMan->FindMaterial("SSteel304"),
                                        "volModuleMedFT");

  // loosely positioned here
  G4double xBound = volModuleWall->GetXHalfLength();
  G4double yBound = volModuleWall->GetZHalfLength();
  G4double x1 = xBound-5*cm;
  G4double x2 = x1 - 5*cm;
  G4double z2 = x1;
  new G4PVPlacement(0, G4ThreeVector(x1,0,0), fVolModuleMedFT, fVolModuleMedFT->GetName()+"_pos1", fVolModuleFTContainer, false, 0);
  new G4PVPlacement(0, G4ThreeVector(-x1,0,0), fVolModuleMedFT, fVolModuleMedFT->GetName()+"_pos2", fVolModuleFTContainer, false, 1);
  new G4PVPlacement(0, G4ThreeVector(x2,z2,0), fVolModuleMedFT, fVolModuleMedFT->GetName()+"_pos3", fVolModuleFTContainer, false, 2);
  new G4PVPlacement(0, G4ThreeVector(-x2,z2,0), fVolModuleMedFT, fVolModuleMedFT->GetName()+"_pos4", fVolModuleFTContainer, false, 3);

  G4Box* solModuleFlange = new G4Box("solModuleFlange", 
                                      solModuleFTContainer->GetXHalfLength(),
                                      solModuleFTContainer->GetYHalfLength()+solModuleTopWall->GetYHalfLength(),
                                      solModuleFTContainer->GetZHalfLength());
  fVolModuleFlange = new G4LogicalVolume(solModuleFlange,
                                        matMan->FindMaterial("Air"),
                                        "volModuleFlange");


  std::vector<G4LogicalVolume*> geoms = {fVolModuleTopSolid,
                                         fVolModuleFTContainer};
  std::vector<G4double> geomsDim = {((G4Box*)geoms[0]->GetSolid())->GetYHalfLength(),
                                    ((G4Box*)geoms[1]->GetSolid())->GetYHalfLength() };


  std::vector<G4double> steps = util.Stack(geomsDim, solModuleFlange->GetYHalfLength());
  std::vector<G4ThreeVector> positions;
  positions.resize(steps.size());
  for (unsigned s = 0; s < steps.size(); s++) {positions[s] = G4ThreeVector(0,steps[s],0); }
  util.Place(geoms, positions, fVolModuleFlange);
  geoms.clear(); geomsDim.clear(); positions.clear();
}


}
