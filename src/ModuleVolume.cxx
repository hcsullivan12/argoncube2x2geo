// 
// File: ModuleVolume.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to construct module volume.
//

#include "ModuleVolume.h"
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

namespace geo
{

ModuleVolume::ModuleVolume()
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

ModuleVolume::~ModuleVolume()
{}

void ModuleVolume::ConstructVolume(G4VPhysicalVolume* worldPV,
                                   G4LogicalVolume*   worldLV)
{
  //**** 
  // Module Volumes
  //****
  ConstructSubVolumes();

  //****
  // Placement
  //****
  PlaceSubVolumes();
  PlaceVolumes(worldLV);
}

void ModuleVolume::PlaceVolumes(G4LogicalVolume* worldLV)
{
  // Stack sub volumes
  // Complete Module 
  // Our module legs are of boolean type, surely there is a better
  // way to access the constituent's dimensions, but this will do for now
  Configuration*   config = Configuration::Instance();
  std::vector<G4double> modLegDim = config->ModuleLegDim();
  std::vector<G4double> legPosition = config->LegPosition();

  arcutil::Utilities util;
  MaterialManager* matMan = MaterialManager::Instance();

  // Place all four legs in container
  auto moduleWall = (G4Box*)fVolModuleWall->GetSolid();
  G4Box* solLegContainer = new G4Box("solLegContainer",
                                     moduleWall->GetXHalfLength(),
                                     (modLegDim[1]/2.)*cm,
                                     moduleWall->GetZHalfLength());
  fVolLegContainer = new G4LogicalVolume(solLegContainer,
	                                       matMan->FindMaterial("Air"),
	                                       "volLegContainer");

  G4ThreeVector pos(legPosition[0]*cm, 0, legPosition[2]*cm);
  new G4PVPlacement(0, pos, fVolModuleLeg, fVolModuleLeg->GetName()+"_pos1", fVolLegContainer, false, 0);

  pos = {-1*legPosition[0]*cm, 0, legPosition[2]*cm};
  new G4PVPlacement(0, pos, fVolModuleLeg, fVolModuleLeg->GetName()+"_pos2", fVolLegContainer, false, 1);

  pos = {-1*legPosition[0]*cm, 0, -1*legPosition[2]*cm};
  new G4PVPlacement(0, pos, fVolModuleLeg, fVolModuleLeg->GetName()+"_pos3", fVolLegContainer, false, 2);

  pos = {legPosition[0]*cm, 0, -1*legPosition[2]*cm};
  new G4PVPlacement(0, pos, fVolModuleLeg, fVolModuleLeg->GetName()+"_pos4", fVolLegContainer, false, 3);

  // Stack the volumes
  std::vector<G4LogicalVolume*> geoms = {fVolBottomDummyFlange, fVolLegContainer, fVolModuleWall};
  std::vector<G4double> geomsDim = {((G4Box*)fVolBottomDummyFlange->GetSolid())->GetYHalfLength(),
                                    ((G4Box*)fVolLegContainer->GetSolid())->GetYHalfLength(),
                                    ((G4Box*)fVolModuleWall->GetSolid())->GetYHalfLength()};

  std::vector<G4double> moduleDim = {moduleWall->GetXHalfLength(),
                                     geomsDim[0]+geomsDim[1]+geomsDim[2], 
                                     moduleWall->GetZHalfLength()};

  G4Box* solModule = new G4Box("solModule",
                                moduleDim[0],
                                moduleDim[1],
                                moduleDim[2]);
  fVolModule = new G4LogicalVolume(solModule,
                                   matMan->FindMaterial("Air"),
                                   "volModule");

  
  std::vector<G4double> steps = util.Stack(geomsDim, moduleDim[1]);
  std::vector<G4ThreeVector> positions;
  std::cout << -1*moduleDim[1]/cm << std::endl;
  for (auto s : steps) std::cout << "Step = " << s/cm << std::endl;
  positions.resize(steps.size());
  for (unsigned s = 0; s < steps.size(); s++) {positions[s] = G4ThreeVector(0,steps[s],0); }

  util.Place(geoms, positions, fVolModule);
  geoms.clear(); geomsDim.clear(); positions.clear();


  geoms = {fVolModule};
  positions = {G4ThreeVector(0,0,0)};
  util.Place(geoms, positions, worldLV);
}

void ModuleVolume::ConstructSubVolumes()
{
  ConstructActiveVolume();
  ConstructBottomVolume();
  ConstructTopVolume();
}

void ModuleVolume::ConstructBottomVolume()
{
  if (!fVolModuleWall) return;

  //****
  // Build from inside out
  //****
  MaterialManager* matMan = MaterialManager::Instance();
  Configuration*   config = Configuration::Instance();

  // Get variables
  std::vector<G4double> modLegDim      = config->ModuleLegDim();
  std::vector<G4double> modLegFootDim  = config->ModuleLegFootDim();
  std::vector<G4double> dummyFlangeDim = config->BottomDummyFlangeDim();

  // Heat exchanger
 
  // Legs
  G4Box* solModuleLegShin = new G4Box("solModuleLegShin", 
                                      (modLegDim[0]/2.)*cm, 
                                      (modLegDim[1]/2.)*cm, 
                                      (modLegDim[2]/2.)*cm);
  G4Box* solModuleLegFoot = new G4Box("solModuleLegFoot", 
                                      (modLegFootDim[0]/2.)*cm, 
                                      (modLegFootDim[1]/2.)*cm, 
                                      (modLegFootDim[2]/2.)*cm); 
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

  // Dummy flange
  G4Box* solBottomDummyFlange = new G4Box("solBottomDummyFlange", 
                                          (dummyFlangeDim[0]/2.)*cm, 
                                          (dummyFlangeDim[1]/2.)*cm, 
                                          (dummyFlangeDim[2]/2.)*cm);
  fVolBottomDummyFlange = new G4LogicalVolume(solBottomDummyFlange, 
                                              matMan->FindMaterial("SSteel304"),
                                              "volBottomDummyFlange");

}

void ModuleVolume::ConstructTopVolume()
{
  if (!fVolModuleWall) return;

}

void ModuleVolume::ConstructActiveVolume()
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
                                  (activeLArDim[0]/4.)*cm, 
                                  (activeLArDim[1]/2.)*cm, 
                                  (activeLArDim[2]/2.)*cm);
  fVolActiveLAr = new G4LogicalVolume(solActiveLAr, 
                                      matMan->FindMaterial("LAr"),
                                      "volActiveLAr"); 
  // Light US plane
  G4Box* solLightUSPlane = new G4Box("solLightUSPlane", 
                                     solActiveLAr->GetXHalfLength(),
                                     solActiveLAr->GetYHalfLength(), 
                                     (lightDetDim[2]/2.)*cm);
  fVolLightUSPlane = new G4LogicalVolume(solLightUSPlane, 
                                         matMan->FindMaterial("PVT"),
                                         "volLightUSPlane"); 
  // Light DS plane
  G4Box* solLightDSPlane = new G4Box("solLightDSPlane", 
                                     solActiveLAr->GetXHalfLength(), 
                                     solActiveLAr->GetYHalfLength(), 
                                     (lightDetDim[2]/2.)*cm);
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
                                                solActiveLight->GetYHalfLength()+(fieldShellThickness/2.)*cm,
                                                solActiveLight->GetZHalfLength()+(fieldShellThickness/2.)*cm);
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
                                (cathodeThickness/2.)*cm,
                                solLeftSubModule->GetYHalfLength(),
                                solLeftSubModule->GetZHalfLength());
  fVolCathode = new G4LogicalVolume(solCathode, 
                                    matMan->FindMaterial("Copper"),
                                    "volCathode"); 
  // Pixel planes
  G4Box* solLeftPixelPlane = new G4Box("solLeftCathode",
                                        (pixelPlaneThickness/2.)*cm,
                                        solLeftSubModule->GetYHalfLength(),
                                        solLeftSubModule->GetZHalfLength());
  fVolLeftPixelPlane = new G4LogicalVolume(solLeftPixelPlane,
                                           matMan->FindMaterial("FR4"),
                                           "volLeftPixelPlane");
  G4Box* solRightPixelPlane = new G4Box("solRightCathode",
                                        (pixelPlaneThickness/2.)*cm,
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
                                    solActiveModule->GetXHalfLength()+(moduleWallThickness/2.)*cm,
                                    solActiveModule->GetYHalfLength()+(moduleWallThickness/2.)*cm,
                                    solActiveModule->GetZHalfLength()+(moduleWallThickness/2.)*cm);
  fVolModuleWall = new G4LogicalVolume(solModuleWall,
                                       matMan->FindMaterial("FR4"),
                                       "volModuleWall"); 
}

void ModuleVolume::PlaceSubVolumes()
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

  // LAr active and light planes
  geoms    = {fVolLightUSPlane, fVolActiveLAr, fVolLightDSPlane};
  geomsDim = {((G4Box*)fVolLightUSPlane->GetSolid())->GetZHalfLength(), 
              ((G4Box*)fVolActiveLAr->GetSolid())->GetZHalfLength(), 
              ((G4Box*)fVolLightDSPlane->GetSolid())->GetZHalfLength()};
  steps = util.Stack(geomsDim, ((G4Box*)fVolActiveLight->GetSolid())->GetZHalfLength());
  positions.resize(steps.size());
  for (unsigned s = 0; s < steps.size(); s++) positions[s] = G4ThreeVector(0,0,steps[s]); 
  util.Place(geoms, positions, fVolActiveLight);
  geoms.clear(); geomsDim.clear(); positions.clear(); steps.clear();

  // LAr active light and field shell
  geoms = {fVolActiveLight, fVolFieldShell};
  geomsDim = {((G4Box*)fVolActiveLight->GetSolid())->GetZHalfLength(), 
              ((G4Box*)fVolFieldShell->GetSolid())->GetZHalfLength()};
  positions = {zeroVec, zeroVec};
  util.Place(geoms, positions, fVolLeftSubModule);
  util.Place(geoms, positions, fVolRightSubModule);
  geoms.clear(); geomsDim.clear(); positions.clear();

  // Sub modules, pixel plane, and cathode
  geoms = {fVolLeftPixelPlane, fVolLeftSubModule, fVolCathode, fVolRightSubModule, fVolRightPixelPlane};
  geomsDim = {((G4Box*)fVolLeftPixelPlane->GetSolid())->GetXHalfLength(),
              ((G4Box*)fVolLeftSubModule->GetSolid())->GetXHalfLength(),
              ((G4Box*)fVolCathode->GetSolid())->GetXHalfLength(),
              ((G4Box*)fVolRightSubModule->GetSolid())->GetXHalfLength(),
              ((G4Box*)fVolRightPixelPlane->GetSolid())->GetXHalfLength()};
  steps = util.Stack(geomsDim, ((G4Box*)fVolActiveModule->GetSolid())->GetXHalfLength());
  positions.resize(steps.size());
  for (unsigned s = 0; s < steps.size(); s++) {positions[s] = G4ThreeVector(steps[s],0,0); }
  util.Place(geoms, positions, fVolActiveModule);
  geoms.clear(); geomsDim.clear(); positions.clear();

  // Active module
  geoms = {fVolActiveModule};
  positions = {zeroVec};
  util.Place(geoms, positions, fVolModuleWall);
}
}
