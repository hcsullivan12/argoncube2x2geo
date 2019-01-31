// 
// File: Detector.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to construct module volume.
//

#include "Detector.h"
#include "Configuration.h"
#include "MaterialManager.h"
#include "Utilities.h"

#include "G4Box.hh"
#include "G4PVPlacement.hh"

namespace geo
{

Detector::Detector()
{}

Detector::~Detector()
{}

void Detector::ConstructVolume(G4LogicalVolume* volWorld,
                               Module*          module)

{
  //**** 
  // Module Volumes
  //****
  Configuration* config = Configuration::Instance();
  MaterialManager* matMan = MaterialManager::Instance();
  G4double modInsideGap = config->ModuleClearance();

  const G4LogicalVolume* volModule = module->GetLV();
  G4Box* solModule = (G4Box*)volModule->GetSolid();

  // Module containers
  G4Box* solModuleSubContainer1 = new G4Box("solModuleSubContainer1",
                                            2*solModule->GetXHalfLength()+(modInsideGap/2.)*cm,
                                            solModule->GetYHalfLength(),
                                            solModule->GetZHalfLength()+(modInsideGap/2.)*cm);
  G4LogicalVolume* volModuleSubContainer1 = new G4LogicalVolume(solModuleSubContainer1,
                                                                matMan->FindMaterial("LAr"),
                                                                "volModuleSubContainer1");
  G4Box* solModuleSubContainer2 = new G4Box("solModuleSubContainer2",
                                             2*solModule->GetXHalfLength()+(modInsideGap/2.)*cm,
                                             solModule->GetYHalfLength(),
                                             solModule->GetZHalfLength()+(modInsideGap/2.)*cm);
  G4LogicalVolume* volModuleSubContainer2 = new G4LogicalVolume(solModuleSubContainer2,
                                                                matMan->FindMaterial("LAr"),
                                                                "volModuleSubContainer2");     

  G4Box* solModuleContainer = new G4Box("solModuleContainer",
                                         solModuleSubContainer1->GetXHalfLength(),
                                         solModuleSubContainer1->GetYHalfLength(),
                                         2*solModuleSubContainer1->GetZHalfLength());
  fVolModuleContainer = new G4LogicalVolume(solModuleContainer,
                                            matMan->FindMaterial("LAr"),
                                            "volModuleContainer");
 
  //****
  // Placement in z
  //****
  std::vector<G4LogicalVolume*> geoms;
  std::vector<G4double> geomsDim;
  std::vector<G4ThreeVector> positions;
  std::vector<G4double> steps;
  G4ThreeVector zeroVec(0,0,0);
  arcutil::Utilities util;

  geoms    = {volModuleSubContainer1, volModuleSubContainer2};
  geomsDim = {((G4Box*)geoms[0]->GetSolid())->GetZHalfLength(),
              ((G4Box*)geoms[1]->GetSolid())->GetZHalfLength()};
  steps = util.Stack(geomsDim, ((G4Box*)fVolModuleContainer->GetSolid())->GetZHalfLength());
  positions.resize(steps.size());
  for (unsigned s = 0; s < steps.size(); s++) positions[s] = G4ThreeVector(0,0,steps[s]);
  util.Place(geoms, positions, fVolModuleContainer);
  geoms.clear(); geomsDim.clear(); positions.clear(); steps.clear();
  

  // Place in world
  geoms = {fVolModuleContainer};
  positions = {G4ThreeVector(0,0,0)};
  util.Place(geoms, positions, volWorld); 
}
}
