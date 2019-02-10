// 
// File: Detector.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to construct module volume.
//

#include "Geometry/Detector/Detector.h"
#include "Configuration.h"
#include "MaterialManager.h"
#include "Utilities.h"

#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

namespace geo
{

Detector::Detector()
{}

Detector::~Detector()
{}

void Detector::ConstructVolume()
{
  ConstructSubVolumes();
  PlaceSubVolumes();
}

void Detector::ConstructSubVolumes()
{
  // Modules
  fModule = new Module();
  fModule->ConstructVolume();

  Configuration* config   = Configuration::Instance();
  MaterialManager* matMan = MaterialManager::Instance();

  G4double modInsideGap = config->ModuleClearance(); 
  G4LogicalVolume* volModule = fModule->GetLV();
  G4Box* solModule = (G4Box*)volModule->GetSolid();

  // Module container
  G4Box* solModuleContainer = new G4Box("solModuleContainer",
                                         2*solModule->GetXHalfLength()+(modInsideGap/2.),
                                         solModule->GetYHalfLength(),
                                         2*solModule->GetZHalfLength()+(modInsideGap/2.));
  fVolModuleContainer = new G4LogicalVolume(solModuleContainer,
                                            matMan->FindMaterial("LAr"),
                                            "volModuleContainer");
}

void Detector::PlaceSubVolumes()
{
  // Placement in z
  arcutil::Utilities util;
  Configuration* config = Configuration::Instance();
  G4double modInsideGap = config->ModuleClearance(); 
  G4LogicalVolume* volModule = fModule->GetLV();
  G4Box* solModule = (G4Box*)volModule->GetSolid();

  G4double boundZ = -1*((G4Box*)fVolModuleContainer->GetSolid())->GetZHalfLength();
  G4double boundX = -1*((G4Box*)fVolModuleContainer->GetSolid())->GetXHalfLength();

  std::vector<G4double> stepsX = {boundX + solModule->GetXHalfLength(),
                                  boundX + 3*solModule->GetXHalfLength() + modInsideGap};
  std::vector<G4double> stepsZ = {boundZ + solModule->GetZHalfLength(),
                                  boundZ + 3*solModule->GetZHalfLength() + modInsideGap};

  std::vector<G4ThreeVector> positions = {G4ThreeVector(stepsX[0], 0, stepsZ[0]),
                                          G4ThreeVector(stepsX[0], 0, stepsZ[1]),
                                          G4ThreeVector(stepsX[1], 0, stepsZ[0]),
                                          G4ThreeVector(stepsX[1], 0, stepsZ[1])};

  new G4PVPlacement(0, positions[0], volModule, volModule->GetName()+"_pos1", fVolModuleContainer, false, 1);
  new G4PVPlacement(0, positions[1], volModule, volModule->GetName()+"_pos2", fVolModuleContainer, false, 2);
  new G4PVPlacement(0, positions[2], volModule, volModule->GetName()+"_pos3", fVolModuleContainer, false, 3);
  new G4PVPlacement(0, positions[3], volModule, volModule->GetName()+"_pos4", fVolModuleContainer, false, 4);
}

}
