// 
// File: ModuleActive.cxx
//
// Author: Hunter Sullivan
//

#include "Geometry/Module/ModuleActive.h"
#include "Configuration.h"
#include "MaterialManager.h"
#include "Utilities.h"

#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"

namespace geo
{

ModuleActive::ModuleActive()
{}

ModuleActive::~ModuleActive()
{}

void ModuleActive::ConstructVolume()
{
  ConstructSubVolumes();
  PlaceSubVolumes();
}

void ModuleActive::ConstructSubVolumes()
{
  //****
  // Build from inside out
  //****
  MaterialManager* matMan = MaterialManager::Instance();
  Configuration*   config = Configuration::Instance();

  // Get variables
  std::vector<G4double> activeLArDim = config->ActiveLArDim();        
  std::vector<G4double> lightDetDim  = config->LightDetDim();         
  G4double fieldShellThickness       = config->FieldShellThickness(); 
  G4double cathodeThickness          = config->CathodeThickness();    
  G4double pixelPlaneThickness       = config->PixelPlaneThickness(); 
  G4double moduleWallThickness       = config->ModuleWallThickness(); 
  
  // Active LAr
  G4Box* solActiveLAr = new G4Box("solActiveLAr", 
                                  (activeLArDim[0]/4.), 
                                  (activeLArDim[1]/2.), 
                                  (activeLArDim[2]/2.));
  fVolActiveLAr = new G4LogicalVolume(solActiveLAr, 
                                      matMan->FindMaterial("LAr"),
                                      "volActiveLAr"); 
  // Light US plane
  G4Box* solLightUSPlane = new G4Box("solLightUSPlane", 
                                     solActiveLAr->GetXHalfLength(),
                                     solActiveLAr->GetYHalfLength(), 
                                     (lightDetDim[2]/2.));
  fVolLightUSPlane = new G4LogicalVolume(solLightUSPlane, 
                                         matMan->FindMaterial("PVT"),
                                         "volLightUSPlane"); 
  // Light DS plane
  G4Box* solLightDSPlane = new G4Box("solLightDSPlane", 
                                     solActiveLAr->GetXHalfLength(), 
                                     solActiveLAr->GetYHalfLength(), 
                                     (lightDetDim[2]/2.));
  fVolLightDSPlane = new G4LogicalVolume(solLightDSPlane, 
                                         matMan->FindMaterial("PVT"),
                                         "volLightDSPlane"); 
  // Active light
  G4Box* solActiveLight = new G4Box("solActiveLight", 
                                    solActiveLAr->GetXHalfLength(),
                                    solActiveLAr->GetYHalfLength(),
                                    2*solLightDSPlane->GetZHalfLength()+solActiveLAr->GetZHalfLength());
  fVolActiveLight = new G4LogicalVolume(solActiveLight, 
                                        matMan->FindMaterial("LAr"),
                                        "volActiveLight"); 
  // Field shell solid
  G4Box* solResistFieldShell_whole = new G4Box("solResistFieldShellSide_whole",
                                                solActiveLight->GetXHalfLength(),
                                                solActiveLight->GetYHalfLength()+(fieldShellThickness/2.),
                                                solActiveLight->GetZHalfLength()+(fieldShellThickness/2.));
  // Field shell, subtract pixel/cathode walls
  G4Box* solResistFieldShell_subtract = new G4Box("solResistFieldShellSide_subtract",
                                                   2*solActiveLight->GetXHalfLength(),
                                                   solActiveLight->GetYHalfLength(),
                                                   solActiveLight->GetZHalfLength());
  // Field shell
  G4SubtractionSolid* solFieldShell = new G4SubtractionSolid("solFieldShell", solResistFieldShell_whole, solResistFieldShell_subtract);
  fVolFieldShell = new G4LogicalVolume(solFieldShell, 
                                       matMan->FindMaterial("Kapton"),
                                      "volFieldShell"); 
  // Right sub module
  G4double subModuleDY = solActiveLight->GetYHalfLength()+2*(solResistFieldShell_whole->GetYHalfLength()-solResistFieldShell_subtract->GetYHalfLength());
  G4double subModuleDZ = solActiveLight->GetZHalfLength()+2*(solResistFieldShell_whole->GetZHalfLength()-solResistFieldShell_subtract->GetZHalfLength());
  G4Box* solRightSubModule = new G4Box("solRightSubModule",
                                        solActiveLight->GetXHalfLength(),
                                        subModuleDY,
                                        subModuleDZ);
  fVolRightSubModule = new G4LogicalVolume(solRightSubModule, 
                                           matMan->FindMaterial("LAr"),
                                           "volRightSubModule"); 
  // Left sub module
  G4Box* solLeftSubModule = new G4Box("solLeftSubModule",
                                       solActiveLight->GetXHalfLength(),
                                       subModuleDY,
                                       subModuleDZ);
  fVolLeftSubModule = new G4LogicalVolume(solLeftSubModule, 
                                          matMan->FindMaterial("LAr"),
                                          "volLeftSubModule"); 
  // Cathode
  G4Box* solCathode = new G4Box("solCathode",
                                (cathodeThickness/2.),
                                solLeftSubModule->GetYHalfLength(),
                                solLeftSubModule->GetZHalfLength());
  fVolCathode = new G4LogicalVolume(solCathode, 
                                    matMan->FindMaterial("Copper"),
                                    "volCathode"); 
  // Pixel planes
  G4Box* solLeftPixelPlane = new G4Box("solLeftCathode",
                                        (pixelPlaneThickness/2.),
                                        solLeftSubModule->GetYHalfLength(),
                                        solLeftSubModule->GetZHalfLength());
  fVolLeftPixelPlane = new G4LogicalVolume(solLeftPixelPlane,
                                           matMan->FindMaterial("FR4"),
                                           "volLeftPixelPlane");
  G4Box* solRightPixelPlane = new G4Box("solRightCathode",
                                        (pixelPlaneThickness/2.),
                                        solRightSubModule->GetYHalfLength(),
                                        solRightSubModule->GetZHalfLength());
  fVolRightPixelPlane = new G4LogicalVolume(solRightPixelPlane,
                                           matMan->FindMaterial("FR4"),
                                           "volRightPixelPlane"); 
  // Active container
  G4double modDX = 2*solLeftSubModule->GetXHalfLength()+2*solLeftPixelPlane->GetXHalfLength()+solCathode->GetXHalfLength();
  G4Box* solActiveContainer = new G4Box("solActiveContainer",
                                         modDX,
                                         solLeftSubModule->GetYHalfLength(),
                                         solLeftSubModule->GetZHalfLength());
  fVolActiveContainer = new G4LogicalVolume(solActiveContainer, 
                                                  matMan->FindMaterial("LAr"),
                                                  "volActiveContainer"); 

  // Module wall, subtract out top and bottom walls
  G4Box* solModuleWall_whole = new G4Box("solModuleWall_whole",
                                          solActiveContainer->GetXHalfLength()+(moduleWallThickness/2.),
                                          solActiveContainer->GetYHalfLength()+(moduleWallThickness/2.),
                                          solActiveContainer->GetZHalfLength()+(moduleWallThickness/2.));
  G4Box* solModuleWall_subtract = new G4Box("solModuleWall_subtract",
                                             solActiveContainer->GetXHalfLength(),
                                             2*solActiveContainer->GetYHalfLength(),
                                             solActiveContainer->GetZHalfLength());
  G4SubtractionSolid* solActiveModuleWall = new G4SubtractionSolid("solActiveModuleWall", solModuleWall_whole, solModuleWall_subtract);
  fVolActiveModuleWall = new G4LogicalVolume(solActiveModuleWall,
                                             matMan->FindMaterial("FR4"),
                                             "volActiveModuleWall");

  // Final container
  G4Box* solActiveModule = new G4Box("solActiveModule",
                                      solModuleWall_whole->GetXHalfLength(),
                                      solModuleWall_whole->GetYHalfLength(),
                                      solModuleWall_whole->GetZHalfLength());  

  fVolActiveModule = new G4LogicalVolume(solActiveModule,
                                         matMan->FindMaterial("LAr"),
                                         "volActiveModule");                                       
}

void ModuleActive::PlaceSubVolumes()
{
  std::vector<G4LogicalVolume*> geoms;
  std::vector<G4double>         geomsDim;
  std::vector<G4ThreeVector>    positions;
  std::vector<G4double>         steps;
  G4ThreeVector zeroVec(0,0,0);
  arcutil::Utilities util;

  // LAr active and light planes
  geoms    = { fVolLightUSPlane, 
               fVolActiveLAr, 
               fVolLightDSPlane };
  geomsDim = { ((G4Box*)geoms[0]->GetSolid())->GetZHalfLength(), 
               ((G4Box*)geoms[1]->GetSolid())->GetZHalfLength(), 
               ((G4Box*)geoms[2]->GetSolid())->GetZHalfLength() };

  steps = util.Stack(geomsDim, ((G4Box*)fVolActiveLight->GetSolid())->GetZHalfLength());

  positions.resize(steps.size());
  for (unsigned s = 0; s < steps.size(); s++) positions[s] = G4ThreeVector(0,0,steps[s]); 

  util.Place(geoms, positions, fVolActiveLight);
  geoms.clear(); geomsDim.clear(); positions.clear(); steps.clear();

  // LAr active light and field shell
  geoms = { fVolActiveLight, 
            fVolFieldShell };
  geomsDim = {((G4Box*)geoms[0]->GetSolid())->GetZHalfLength(), 
              ((G4Box*)geoms[1]->GetSolid())->GetZHalfLength()};

  positions = {zeroVec, zeroVec};
  util.Place(geoms, positions, fVolLeftSubModule);
  util.Place(geoms, positions, fVolRightSubModule);
  geoms.clear(); geomsDim.clear(); positions.clear();

  // Sub modules, pixel plane, and cathode
  geoms = { fVolLeftPixelPlane, 
            fVolLeftSubModule, 
            fVolCathode, 
            fVolRightSubModule, 
            fVolRightPixelPlane };
  geomsDim = { ((G4Box*)geoms[0]->GetSolid())->GetXHalfLength(),
               ((G4Box*)geoms[1]->GetSolid())->GetXHalfLength(),
               ((G4Box*)geoms[2]->GetSolid())->GetXHalfLength(),
               ((G4Box*)geoms[3]->GetSolid())->GetXHalfLength(),
               ((G4Box*)geoms[4]->GetSolid())->GetXHalfLength() };

  steps = util.Stack(geomsDim, ((G4Box*)fVolActiveContainer->GetSolid())->GetXHalfLength());

  positions.resize(steps.size());
  for (unsigned s = 0; s < steps.size(); s++) {positions[s] = G4ThreeVector(steps[s],0,0); }

  util.Place(geoms, positions, fVolActiveContainer);
  geoms.clear(); geomsDim.clear(); positions.clear();

  // Active container
  geoms = {fVolActiveContainer};
  positions = {zeroVec};
  util.Place(geoms, positions, fVolActiveModuleWall);

  // Wall
  geoms = {fVolActiveModuleWall};
  util.Place(geoms, positions, fVolActiveModule);
}
}
