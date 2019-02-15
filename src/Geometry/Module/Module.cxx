// 
// File: Module.cxx
//
// Author: Hunter Sullivan
//

#include "Geometry/Module/Module.h"
#include "QuantityStore.h"
#include "MaterialManager.h"
#include "Utilities.h"

#include "G4Box.hh"
#include "G4UnionSolid.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"

namespace geo
{

Module::Module()
{}

Module::~Module()
{}

void Module::ConstructVolume()
{
  //**** 
  // Module Volumes
  //****
  ConstructSubVolumes();

  //****
  // Placement
  //****
  PlaceSubVolumes();
}

void Module::ConstructSubVolumes()
{
  MaterialManager* matMan = MaterialManager::Instance(); 
  QuantityStore*   qStore = QuantityStore::Instance();

  fModuleActive = new ModuleActive();
  fModuleActive->ConstructVolume();

  fModuleBottom = new ModuleBottom();
  fModuleBottom->ConstructVolume();

  fModuleTop = new ModuleTop();
  fModuleTop->ConstructVolume();

  G4Box* solModActive = (G4Box*)fModuleActive->GetLV()->GetSolid(); 
  G4Box* solModBottom = (G4Box*)fModuleBottom->GetLV()->GetSolid(); 
  G4Box* solModTop    = (G4Box*)fModuleTop->GetLV()->GetSolid(); 

  // Add extra G10 layers
  G4Box* solModuleMiddleFrame_whole = new G4Box("solModuleMiddleFrame_whole",
                                                 solModActive->GetXHalfLength(),
                                                 (30/2.)*mm,
                                                 solModActive->GetZHalfLength());
  G4Box* solModuleMiddleFrame_subtract = new G4Box("solModuleMiddleFrame_subtract",
                                                    solModActive->GetXHalfLength() - 5.0*cm,
                                                    2*(30/2.)*mm,
                                                    solModActive->GetZHalfLength() - 5.0*cm);
  qStore->kModuleMiddleFrameYHalfL = solModuleMiddleFrame_whole->GetYHalfLength();                                                   
  G4SubtractionSolid* solModuleMiddleFrame = new G4SubtractionSolid("solModuleMiddleFrame",
                                                                     solModuleMiddleFrame_whole,
                                                                     solModuleMiddleFrame_subtract,
                                                                     0,
                                                                     G4ThreeVector()); 
  fVolModuleMiddleFrame = new G4LogicalVolume(solModuleMiddleFrame,
                                              matMan->FindMaterial("FR4"),
                                              "volModuleMiddleFrame"); 

  G4Box* solModuleBottomWall = new G4Box("solModuleBottomWall",
                                          solModActive->GetXHalfLength(),
                                          (20/2.)*mm,
                                          solModActive->GetZHalfLength());
  fVolModuleBottomWall = new G4LogicalVolume(solModuleBottomWall,
                                             matMan->FindMaterial("FR4"),
                                             "volModuleBottomWall");                                          

  // Module container
  G4double yLen =   solModBottom->GetYHalfLength()
                  + solModuleBottomWall->GetYHalfLength()
                  + solModActive->GetYHalfLength()
                  + qStore->kModuleMiddleFrameYHalfL
                  + qStore->kTopModuleYHalfL;
  
  G4Box* solModule = new G4Box("solModule",
                                solModActive->GetXHalfLength(),
                                yLen,
                                solModActive->GetZHalfLength());
  fVolModule = new G4LogicalVolume(solModule,
                                   matMan->FindMaterial("LAr"),
                                   "volModule");
}

void Module::PlaceSubVolumes()
{
  arcutil::Utilities util;
  MaterialManager* matMan = MaterialManager::Instance(); 
  QuantityStore*   qStore = QuantityStore::Instance();

  // Complete Module 
  std::vector<G4LogicalVolume*> geoms = { fModuleBottom->GetLV(),
                                          fVolModuleBottomWall,
                                          fModuleActive->GetLV(),
                                          fVolModuleMiddleFrame,
                                          fModuleTop->GetLV() };

  std::vector<G4double> geomsDim = { ((G4Box*)geoms[0]->GetSolid())->GetYHalfLength(),
                                     ((G4Box*)geoms[1]->GetSolid())->GetYHalfLength(),
                                     ((G4Box*)geoms[2]->GetSolid())->GetYHalfLength(),
                                     qStore->kModuleMiddleFrameYHalfL,
                                     qStore->kTopModuleYHalfL };                                                                         

  std::vector<G4double> moduleDim = {((G4Box*)fVolModule->GetSolid())->GetXHalfLength(),
                                     ((G4Box*)fVolModule->GetSolid())->GetYHalfLength(),
                                     ((G4Box*)fVolModule->GetSolid())->GetZHalfLength()};
  
  std::vector<G4double> steps = util.Stack(geomsDim, moduleDim[1]);

  std::vector<G4ThreeVector>     positions;
  std::vector<G4RotationMatrix*> rotations;

  positions.resize(steps.size());
  rotations.resize(steps.size(), 0);

  for (unsigned s = 0; s < steps.size(); s++) positions[s] = G4ThreeVector(0,steps[s],0); 
  util.Place(geoms, positions, rotations, fVolModule);
}
}
