// 
// File: ModuleActive.cxx
//
// Author: Hunter Sullivan
//

#include "Geometry/Module/ModuleActive.h"
#include "Geometry/Auxiliary/PixelPlane.h"
#include "QuantityStore.h"
#include "MaterialManager.h"
#include "Utilities.h"

#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4PhysicalConstants.hh"

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
  QuantityStore*   qStore = QuantityStore::Instance();

  // Get variables
  std::vector<G4double> activeLArDim = qStore->kActiveLArDim;        
  std::vector<G4double> lightDetDim  = qStore->kLightDetDim;         
  G4double fieldShellThickness       = qStore->kFieldShellThickness; 
  G4double cathodeThickness          = qStore->kCathodeThickness;    
  G4double pixelPlaneThickness       = qStore->kPixelPlaneThickness; 
  G4double pixelSpacing              = qStore->kPixelSpacing;
  G4double pixelRMax                 = qStore->kPixelRadius;
  G4double moduleWallThickness       = qStore->kModuleWallThickness; 
  
  // Active LAr
  G4Box* solTPCActive = new G4Box("solTPCActive", 
                                  (activeLArDim[0]/4.), 
                                  (activeLArDim[1]/2.), 
                                  (activeLArDim[2]/2.));
  fVolTPCActive = new G4LogicalVolume(solTPCActive, 
                                      matMan->FindMaterial("LAr"),
                                      "volTPCActive"); 
  // Light US plane
  G4Box* solOpDetSensitive = new G4Box("solOpDetSensitive", 
                                        solTPCActive->GetXHalfLength(),
                                        solTPCActive->GetYHalfLength(), 
                                        (lightDetDim[2]/2.));
  fVolOpDetSensitiveUS = new G4LogicalVolume(solOpDetSensitive, 
                                             matMan->FindMaterial("PVT"),
                                             "volOpDetSensitiveUS"); 
  fVolOpDetSensitiveDS = new G4LogicalVolume(solOpDetSensitive, 
                                             matMan->FindMaterial("PVT"),
                                             "volOpDetSensitiveDS"); 
  // Active light
  G4Box* solActiveLight = new G4Box("solActiveLight", 
                                    solTPCActive->GetXHalfLength(),
                                    solTPCActive->GetYHalfLength(),
                                    2*solOpDetSensitive->GetZHalfLength()+solTPCActive->GetZHalfLength());
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
  // Sub module
  G4double subModuleDY = solActiveLight->GetYHalfLength()+2*(solResistFieldShell_whole->GetYHalfLength()-solResistFieldShell_subtract->GetYHalfLength());
  G4double subModuleDZ = solActiveLight->GetZHalfLength()+2*(solResistFieldShell_whole->GetZHalfLength()-solResistFieldShell_subtract->GetZHalfLength());
  G4Box* solSubModule = new G4Box("solSubModule",
                                   solResistFieldShell_whole->GetXHalfLength(),
                                   subModuleDY,
                                   subModuleDZ);
  fVolSubModule = new G4LogicalVolume(solSubModule, 
                                      matMan->FindMaterial("LAr"),
                                      "volSubModule"); 

  // Cathode
  G4Box* solCathode = new G4Box("solCathode",
                                (cathodeThickness/2.),
                                solSubModule->GetYHalfLength(),
                                solSubModule->GetZHalfLength());
  fVolCathode = new G4LogicalVolume(solCathode, 
                                    matMan->FindMaterial("Copper"),
                                    "volCathode"); 
  // Pixel plane
  PixelPlane pp;
  fVolTPCPlane = pp.ConstructVolume("TPCPlane1",
                                    solSubModule->GetYHalfLength(),
                                    solSubModule->GetZHalfLength(),
                                    pixelPlaneThickness/2.,
                                    pixelSpacing,
                                    pixelRMax);

  // TPCs
  G4Box* solTPC = new G4Box("solTPC",
                            solSubModule->GetXHalfLength()+pixelPlaneThickness/2.,
                            solSubModule->GetYHalfLength(),
                            solSubModule->GetZHalfLength());
  fVolTPCR = new G4LogicalVolume(solTPC,
                                 matMan->FindMaterial("LAr"),
                                 "volTPCR");            
  fVolTPCL = new G4LogicalVolume(solTPC,
                                 matMan->FindMaterial("LAr"),
                                 "volTPCL");                                                 

  // Active container
  G4double modDX = 2*solTPC->GetXHalfLength()+solCathode->GetXHalfLength();
  G4Box* solActiveContainer = new G4Box("solActiveContainer",
                                         modDX,
                                         solTPC->GetYHalfLength(),
                                         solTPC->GetZHalfLength());
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
  // Reusable containers
  // BE SURE TO CLEAR
  std::vector<G4LogicalVolume*>  geoms;
  std::vector<G4double>          geomsDim;
  std::vector<G4ThreeVector>     positions;
  std::vector<G4RotationMatrix*> rotations;
  std::vector<G4double>          steps;
  arcutil::Utilities util;

  //****************************************
  // LAr active and light planes in active light
  geoms    = { fVolOpDetSensitiveUS, 
               fVolTPCActive, 
               fVolOpDetSensitiveDS };
  geomsDim = { ((G4Box*)geoms[0]->GetSolid())->GetZHalfLength(), 
               ((G4Box*)geoms[1]->GetSolid())->GetZHalfLength(), 
               ((G4Box*)geoms[2]->GetSolid())->GetZHalfLength() };

  steps = util.Stack(geomsDim, ((G4Box*)fVolActiveLight->GetSolid())->GetZHalfLength());
  positions.resize(steps.size());
  rotations.resize(steps.size(), 0);

  for (unsigned s = 0; s < steps.size(); s++) positions[s] = G4ThreeVector(0,0,steps[s]); 

  util.Place(geoms, positions, rotations, fVolActiveLight);
  geoms.clear(); geomsDim.clear(); positions.clear(); rotations.clear(); steps.clear();

  //*******************************************
  // LAr active light and field shell in sub module
  geoms = { fVolActiveLight, 
            fVolFieldShell };
  geomsDim = {((G4Box*)geoms[0]->GetSolid())->GetZHalfLength(), 
              ((G4Box*)geoms[1]->GetSolid())->GetZHalfLength()};

  positions = {G4ThreeVector(), G4ThreeVector()};
  rotations.resize(positions.size(), 0);

  util.Place(geoms, positions, rotations, fVolSubModule);
  geoms.clear(); geomsDim.clear(); positions.clear(); rotations.clear(); steps.clear();

  //***********************************************
  // Sub module and pixel plane in tpc
  geoms    = { fVolTPCPlane,
               fVolSubModule };
  geomsDim = { ((G4Box*)geoms[0]->GetSolid())->GetZHalfLength(), // planes are built with normal in z direction
               ((G4Box*)geoms[1]->GetSolid())->GetXHalfLength() }; 

  steps = util.Stack(geomsDim, ((G4Box*)fVolTPCR->GetSolid())->GetXHalfLength());  
  
  positions.resize(steps.size());
  rotations.resize(steps.size(), 0);

  for (unsigned s = 0; s < steps.size(); s++) positions[s] = G4ThreeVector(steps[s],0,0);     
  
  // For the pixel plane
  G4RotationMatrix* rot1 = new G4RotationMatrix;
  rot1->rotateX(pi/2);
  rot1->rotateY(pi/2);
  rotations[0] = rot1;

  util.Place(geoms, positions, rotations, fVolTPCR);
  // We have to rotate the sub module to have light detectors in correct position
  G4RotationMatrix* rot2 = new G4RotationMatrix;
  rot2->rotateY(pi);
  rotations[1] = rot2;

  util.Place(geoms, positions, rotations, fVolTPCL);
  geoms.clear(); geomsDim.clear(); positions.clear(); rotations.clear(); steps.clear();         

  //***********************************************
  // Sub modules, pixel plane, and cathode
  geoms = { fVolTPCR, 
            fVolCathode, 
            fVolTPCL };
  geomsDim = { ((G4Box*)geoms[0]->GetSolid())->GetXHalfLength(),
               ((G4Box*)geoms[1]->GetSolid())->GetXHalfLength(),
               ((G4Box*)geoms[2]->GetSolid())->GetXHalfLength() }; 

  steps = util.Stack(geomsDim, ((G4Box*)fVolActiveContainer->GetSolid())->GetXHalfLength());

  positions.resize(steps.size());
  rotations.resize(steps.size(), 0);

  for (unsigned s = 0; s < steps.size(); s++) positions[s]   = G4ThreeVector(steps[s],0,0); 

  // Rotate the other TPC
  rotations[2] = rot2;

  new G4PVPlacement(rotations[0], positions[0], geoms[0], geoms[0]->GetName()+"1", fVolActiveContainer, false, 0);
  new G4PVPlacement(rotations[1], positions[1], geoms[1], geoms[1]->GetName(),     fVolActiveContainer, false, 0);
  new G4PVPlacement(rotations[2], positions[2], geoms[2], geoms[2]->GetName()+"2", fVolActiveContainer, false, 0);
  geoms.clear(); geomsDim.clear(); positions.clear(); rotations.clear(); steps.clear();

  //**********************************************
  // Active container
  geoms = {fVolActiveContainer};
  positions = {G4ThreeVector()};
  rotations.resize(positions.size(), 0);

  util.Place(geoms, positions, rotations, fVolActiveModuleWall);
  geoms.clear(); geomsDim.clear(); positions.clear(); rotations.clear(); steps.clear();

  //***********************************************
  // Wall
  geoms = {fVolActiveModuleWall};
  positions = {G4ThreeVector()};
  rotations.resize(positions.size(), 0);

  util.Place(geoms, positions, rotations, fVolActiveModule);
}
}
