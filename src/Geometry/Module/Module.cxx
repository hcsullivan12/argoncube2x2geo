// 
// File: Module.cxx
//
// Author: Hunter Sullivan
//

#include "Geometry/Module/Module.h"
#include "Configuration.h"
#include "MaterialManager.h"
#include "Utilities.h"

#include "G4Box.hh"
#include "G4UnionSolid.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

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

  fModuleActive = new ModuleActive();
  fModuleActive->ConstructVolume();

  fModuleBottom = new ModuleBottom();
  fModuleBottom->ConstructVolume();

  fModuleTop = new ModuleTop();
  fModuleTop->ConstructVolume();

  G4Box* solModActive = (G4Box*)fModuleActive->GetLV()->GetSolid(); 
  G4Box* solModBottom = (G4Box*)fModuleBottom->GetLV()->GetSolid(); 
  G4Box* solModTop    = (G4Box*)fModuleTop->GetLV()->GetSolid(); 
  G4double yLen = solModActive->GetYHalfLength()+solModBottom->GetYHalfLength()+solModTop->GetYHalfLength();
  std::vector<G4double> moduleDim = { solModActive->GetXHalfLength(),
                                      yLen, 
                                      solModActive->GetZHalfLength()};

  G4Box* solModule = new G4Box("solModule",
                                moduleDim[0],
                                moduleDim[1],
                                moduleDim[2]);
  fVolModule = new G4LogicalVolume(solModule,
                                   matMan->FindMaterial("LAr"),
                                   "volModule");
}

void Module::PlaceSubVolumes()
{
  arcutil::Utilities util;
  MaterialManager* matMan = MaterialManager::Instance(); 

  // Complete Module 
  std::vector<G4LogicalVolume*> geoms = { fModuleBottom->GetLV(),
                                          fModuleActive->GetLV(),
                                          fModuleTop->GetLV() };

  std::vector<G4double> geomsDim = { ((G4Box*)geoms[0]->GetSolid())->GetYHalfLength(),
                                      ((G4Box*)geoms[1]->GetSolid())->GetYHalfLength(),
                                      ((G4Box*)geoms[2]->GetSolid())->GetYHalfLength() };

  std::vector<G4double> moduleDim = {((G4Box*)geoms[2]->GetSolid())->GetXHalfLength(),
                                     geomsDim[0]+geomsDim[1]+geomsDim[2]+geomsDim[3], 
                                     ((G4Box*)geoms[2]->GetSolid())->GetZHalfLength()};
  
  std::vector<G4double> steps = util.Stack(geomsDim, moduleDim[1]);
  std::vector<G4ThreeVector> positions;
  positions.resize(steps.size());
  
  for (unsigned s = 0; s < steps.size(); s++) {positions[s] = G4ThreeVector(0,steps[s],0); }
  util.Place(geoms, positions, fVolModule);
}
}
