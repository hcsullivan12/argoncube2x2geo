// 
// File: ModuleTop.cxx
//
// Author: Hunter Sullivan
//

#include "Geometry/Module/ModuleTop.h"
#include "Configuration.h"
#include "MaterialManager.h"
#include "Utilities.h"

#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolumeStore.hh"

namespace geo
{

ModuleTop::ModuleTop()
{}

ModuleTop::~ModuleTop()
{}

void ModuleTop::ConstructVolume()
{
  ConstructSubVolumes();
  PlaceSubVolumes();
}

void ModuleTop::ConstructSubVolumes()
{
  //****
  // Build from inside out
  //****
  MaterialManager* matMan = MaterialManager::Instance();
  Configuration*   config = Configuration::Instance();
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  arcutil::Utilities util;

  G4Box* activeMod = (G4Box*)lvStore->GetVolume("volActiveModule")->GetSolid();
  G4Box* moduleWall = (G4Box*)lvStore->GetVolume("volModuleWall")->GetSolid();
  std::vector<G4double> topLArDim  = config->TopLArDim(); util.ConvertToUnits(topLArDim);
  std::vector<G4double> topGArDim  = config->TopGArDim(); util.ConvertToUnits(topGArDim);
  
  // LAr
  G4Box* solTopLAr = new G4Box("solTopLAr", 
                               activeMod->GetXHalfLength(), 
                               (topLArDim[1]/2.0), 
                               activeMod->GetZHalfLength());
  fVolTopLAr = new G4LogicalVolume(solTopLAr, 
                                   matMan->FindMaterial("LAr"),
                                  "volTopLAr");

  // GAr
  G4Box* solTopGAr = new G4Box("solTopGAr", 
                                activeMod->GetXHalfLength(), 
                                (topGArDim[1]/2.), 
                                activeMod->GetZHalfLength());
  fVolTopGAr = new G4LogicalVolume(solTopGAr, 
                                   matMan->FindMaterial("GAr"),
                                  "volTopGAr");   

  // Top container
  // We don't want extra layers of FR4 here,
  // we want just the walls

  // Top container
  // this will get rid of walls on top and bottom
  G4double dimY = solTopLAr->GetYHalfLength()+solTopGAr->GetYHalfLength();
  G4Box* solTopModule = new G4Box("solTopModule", 
                                  moduleWall->GetXHalfLength(), 
                                  dimY, 
                                  moduleWall->GetZHalfLength());
  fVolModuleTop = new G4LogicalVolume(solTopModule, 
                                      matMan->FindMaterial("FR4"),
                                     "volTopModule");      
}

void ModuleTop::PlaceSubVolumes()
{
  std::vector<G4LogicalVolume*> geoms;
  std::vector<G4double>         geomsDim;
  std::vector<G4double>         steps;
  std::vector<G4ThreeVector>    positions;
  arcutil::Utilities util;

  geoms = { fVolTopLAr, 
            fVolTopGAr };
  geomsDim = { ((G4Box*)geoms[0]->GetSolid())->GetYHalfLength(),
               ((G4Box*)geoms[1]->GetSolid())->GetYHalfLength() };  

  steps = util.Stack(geomsDim, geomsDim[0]+geomsDim[1]);

  positions.resize(steps.size());
  for (unsigned s = 0; s < steps.size(); s++) {positions[s] = G4ThreeVector(0,steps[s],0); }

  util.Place(geoms, positions, fVolModuleTop);
}
}
