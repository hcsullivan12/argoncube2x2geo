// 
// File: Module.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to construct module volume.
//

#include "Module.h"
#include "Configuration.h"
#include "MaterialManager.h"
#include "Utilities.h"

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

namespace geo
{

Module::Module()
 : fVolActiveLAr(NULL),
   fVolLightUSPlane(NULL),
   fVolLightDSPlane(NULL),
   fVolActiveLight(NULL),
   fVolFieldShell(NULL),
   fVolRightSubModule(NULL),
   fVolLeftSubModule(NULL),
   fVolCathode(NULL),
   fVolLeftPixelPlane(NULL),
   fVolRightPixelPlane(NULL),
   fVolActiveModule(NULL),
   fVolModuleWall(NULL)
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
  PlaceVolumes();
}

void Module::PlaceVolumes()
{
  }

void Module::ConstructSubVolumes()
{
  ConstructActiveVolume();
  ConstructBottomVolume();
  ConstructTopVolume();
}

void Module::ConstructBottomVolume()
{
  if (!fVolModuleWall) return;

  //****
  // Build from inside out
  //****
  MaterialManager* matMan = MaterialManager::Instance();
  Configuration*   config = Configuration::Instance();
  arcutil::Utilities util;

  // Get variables
  std::vector<G4double> modLegDim      = config->ModuleLegDim();         util.ConvertToUnits(modLegDim);
  std::vector<G4double> modLegFootDim  = config->ModuleLegFootDim();     util.ConvertToUnits(modLegFootDim);
  std::vector<G4double> dummyFlangeDim = config->BottomDummyFlangeDim(); util.ConvertToUnits(dummyFlangeDim);
  std::vector<G4double> legPosition    = config->ModuleLegPosition();    util.ConvertToUnits(legPosition);
 
  // Legs
  G4Box* solModuleLegShin = new G4Box("solModuleLegShin", 
                                      (modLegDim[0]/2.), 
                                      (modLegDim[1]/2.), 
                                      (modLegDim[2]/2.));
  G4Box* solModuleLegFoot = new G4Box("solModuleLegFoot", 
                                      (modLegFootDim[0]/2.), 
                                      (modLegFootDim[1]/2.), 
                                      (modLegFootDim[2]/2.)); 
  G4ThreeVector transl1(0, solModuleLegShin->GetYHalfLength()-solModuleLegFoot->GetYHalfLength(), 0);
  G4UnionSolid* solModuleLegTemp = new G4UnionSolid("solModuleLegTemp", 
                                                     solModuleLegShin, 
                                                     solModuleLegFoot, 
                                                     0, 
                                                     transl1);
  G4UnionSolid* solModuleLeg = new G4UnionSolid("solModuleLeg", 
                                                 solModuleLegTemp, 
                                                 solModuleLegFoot, 
                                                 0, 
                                                 -1*transl1);                                                    
  fVolModuleLeg = new G4LogicalVolume(solModuleLeg, 
                                      matMan->FindMaterial("FR4"),
                                      "volModuleLeg");  

  // Place all four legs in container
  auto moduleWall = (G4Box*)fVolModuleWall->GetSolid();
  G4Box* solModuleLegContainer = new G4Box("solModuleLegContainer",
                                            moduleWall->GetXHalfLength(),
                                           (modLegDim[1]/2.),
                                            moduleWall->GetZHalfLength());
  fVolModuleLegContainer = new G4LogicalVolume(solModuleLegContainer,
	                                             matMan->FindMaterial("LAr"),
	                                            "volModuleLegContainer");

  G4ThreeVector pos(legPosition[0], 0, legPosition[2]);
  new G4PVPlacement(0, pos, fVolModuleLeg, fVolModuleLeg->GetName()+"_pos1", fVolModuleLegContainer, false, 0);

  pos = {-1*legPosition[0], 0, legPosition[2]};
  new G4PVPlacement(0, pos, fVolModuleLeg, fVolModuleLeg->GetName()+"_pos2", fVolModuleLegContainer, false, 1);

  pos = {-1*legPosition[0], 0, -1*legPosition[2]};
  new G4PVPlacement(0, pos, fVolModuleLeg, fVolModuleLeg->GetName()+"_pos3", fVolModuleLegContainer, false, 2);

  pos = {legPosition[0], 0, -1*legPosition[2]};
  new G4PVPlacement(0, pos, fVolModuleLeg, fVolModuleLeg->GetName()+"_pos4", fVolModuleLegContainer, false, 3);                                      

  // Dummy flange
  G4Box* solBottomDummyFlange = new G4Box("solBottomDummyFlange", 
                                          solModuleLegContainer->GetXHalfLength(), 
                                          (dummyFlangeDim[1]/2.), 
                                          solModuleLegContainer->GetZHalfLength());
  fVolBottomDummyFlange = new G4LogicalVolume(solBottomDummyFlange, 
                                              matMan->FindMaterial("SSteel304"),
                                              "volBottomDummyFlange");

}

void Module::ConstructTopVolume()
{
  if (!fVolModuleWall) return;

  //****
  // Build from inside out
  //****
  MaterialManager* matMan = MaterialManager::Instance();
  Configuration*   config = Configuration::Instance();
  arcutil::Utilities util;

  G4Box* activeMod = (G4Box*)fVolActiveModule->GetSolid();
  G4Box* moduleWall = (G4Box*)fVolModuleWall->GetSolid();
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
  fVolTopModule = new G4LogicalVolume(solTopModule, 
                                      matMan->FindMaterial("FR4"),
                                     "volTopModule");                                                                                          

}

void Module::ConstructActiveVolume()
{
  //****
  // Build from inside out
  //****
  MaterialManager* matMan = MaterialManager::Instance();
  Configuration*   config = Configuration::Instance();
  arcutil::Utilities util;


  // Get variables
  std::vector<G4double> activeLArDim = config->ActiveLArDim();        util.ConvertToUnits(activeLArDim);
  std::vector<G4double> lightDetDim  = config->LightDetDim();         util.ConvertToUnits(lightDetDim);
  G4double fieldShellThickness       = config->FieldShellThickness(); util.ConvertToUnits(fieldShellThickness);
  G4double cathodeThickness          = config->CathodeThickness();    util.ConvertToUnits(cathodeThickness);
  G4double pixelPlaneThickness       = config->PixelPlaneThickness(); util.ConvertToUnits(pixelPlaneThickness);
  G4double moduleWallThickness       = config->ModuleWallThickness(); util.ConvertToUnits(moduleWallThickness);
  
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
  // Active module
  G4double modDX = 2*solLeftSubModule->GetXHalfLength()+2*solLeftPixelPlane->GetXHalfLength()+solCathode->GetXHalfLength();
  G4Box* solActiveModule = new G4Box("solActiveModule",
                                      modDX,
                                      solLeftSubModule->GetYHalfLength(),
                                      solLeftSubModule->GetZHalfLength());
  fVolActiveModule = new G4LogicalVolume(solActiveModule, 
                                        matMan->FindMaterial("LAr"),
                                        "volActiveModule"); 
  // Module wall
  G4Box* solModuleWall = new G4Box("solModuleWall", 
                                    solActiveModule->GetXHalfLength()+(moduleWallThickness/2.),
                                    solActiveModule->GetYHalfLength()+(moduleWallThickness/2.),
                                    solActiveModule->GetZHalfLength()+(moduleWallThickness/2.));
  fVolModuleWall = new G4LogicalVolume(solModuleWall,
                                       matMan->FindMaterial("FR4"),
                                       "volModuleWall"); 
}

void Module::PlaceSubVolumes()
{
  //****
  // Build from inside out
  //
  // Active 
  //****
  std::vector<G4LogicalVolume*> geoms;
  std::vector<G4double> geomsDim;
  std::vector<G4ThreeVector> positions;
  std::vector<G4double> steps;
  G4ThreeVector zeroVec(0,0,0);
  arcutil::Utilities util;
  MaterialManager* matMan = MaterialManager::Instance();

  // LAr active and light planes
  geoms    = {fVolLightUSPlane, fVolActiveLAr, fVolLightDSPlane};
  geomsDim = {((G4Box*)geoms[0]->GetSolid())->GetZHalfLength(), 
              ((G4Box*)geoms[1]->GetSolid())->GetZHalfLength(), 
              ((G4Box*)geoms[2]->GetSolid())->GetZHalfLength()};
  steps = util.Stack(geomsDim, ((G4Box*)fVolActiveLight->GetSolid())->GetZHalfLength());
  positions.resize(steps.size());
  for (unsigned s = 0; s < steps.size(); s++) positions[s] = G4ThreeVector(0,0,steps[s]); 
  util.Place(geoms, positions, fVolActiveLight);
  geoms.clear(); geomsDim.clear(); positions.clear(); steps.clear();

  // LAr active light and field shell
  geoms = {fVolActiveLight, fVolFieldShell};
  geomsDim = {((G4Box*)geoms[0]->GetSolid())->GetZHalfLength(), 
              ((G4Box*)geoms[1]->GetSolid())->GetZHalfLength()};
  positions = {zeroVec, zeroVec};
  util.Place(geoms, positions, fVolLeftSubModule);
  util.Place(geoms, positions, fVolRightSubModule);
  geoms.clear(); geomsDim.clear(); positions.clear();

  // Sub modules, pixel plane, and cathode
  geoms = {fVolLeftPixelPlane, fVolLeftSubModule, fVolCathode, fVolRightSubModule, fVolRightPixelPlane};
  geomsDim = {((G4Box*)geoms[0]->GetSolid())->GetXHalfLength(),
              ((G4Box*)geoms[1]->GetSolid())->GetXHalfLength(),
              ((G4Box*)geoms[2]->GetSolid())->GetXHalfLength(),
              ((G4Box*)geoms[3]->GetSolid())->GetXHalfLength(),
              ((G4Box*)geoms[4]->GetSolid())->GetXHalfLength()};
  steps = util.Stack(geomsDim, ((G4Box*)fVolActiveModule->GetSolid())->GetXHalfLength());
  positions.resize(steps.size());
  for (unsigned s = 0; s < steps.size(); s++) {positions[s] = G4ThreeVector(steps[s],0,0); }
  util.Place(geoms, positions, fVolActiveModule);
  geoms.clear(); geomsDim.clear(); positions.clear();

  // Active module
  geoms = {fVolActiveModule};
  positions = {zeroVec};
  util.Place(geoms, positions, fVolModuleWall);
  geoms.clear(); geomsDim.clear(); positions.clear();


  // Top module
  geoms = {fVolTopLAr, fVolTopGAr};
  geomsDim = {((G4Box*)geoms[0]->GetSolid())->GetYHalfLength(),
              ((G4Box*)geoms[1]->GetSolid())->GetYHalfLength()};               
  steps = util.Stack(geomsDim, geomsDim[0]+geomsDim[1]);
  positions.resize(steps.size());
  for (unsigned s = 0; s < steps.size(); s++) {positions[s] = G4ThreeVector(0,steps[s],0); }
  util.Place(geoms, positions, fVolTopModule);
  geoms.clear(); geomsDim.clear(); positions.clear();   


  // Stack sub volumes
  // Complete Module 
  geoms = {fVolBottomDummyFlange, 
           fVolModuleLegContainer, 
           fVolModuleWall, 
           fVolTopModule};
  geomsDim = {((G4Box*)geoms[0]->GetSolid())->GetYHalfLength(),
              ((G4Box*)geoms[1]->GetSolid())->GetYHalfLength(),
              ((G4Box*)geoms[2]->GetSolid())->GetYHalfLength(),
              ((G4Box*)geoms[3]->GetSolid())->GetYHalfLength()};

  std::vector<G4double> moduleDim = {((G4Box*)geoms[2]->GetSolid())->GetXHalfLength(),
                                     geomsDim[0]+geomsDim[1]+geomsDim[2]+geomsDim[3], 
                                     ((G4Box*)geoms[2]->GetSolid())->GetZHalfLength()};

  G4Box* solModule = new G4Box("solModule",
                                moduleDim[0],
                                moduleDim[1],
                                moduleDim[2]);
  fVolModule = new G4LogicalVolume(solModule,
                                   matMan->FindMaterial("LAr"),
                                   "volModule");
  
  steps = util.Stack(geomsDim, moduleDim[1]);
  positions.resize(steps.size());
  for (unsigned s = 0; s < steps.size(); s++) {positions[s] = G4ThreeVector(0,steps[s],0); }
  util.Place(geoms, positions, fVolModule);
  geoms.clear(); geomsDim.clear(); positions.clear();
}
}
