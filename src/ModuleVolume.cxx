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

#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4PhysicalVolumeStore.hh"

namespace geo
{

ModuleVolume::ModuleVolume()
:  fTPCSolid(NULL),
   fTPCLV(NULL),
   fModuleSolid(NULL),
   fModuleLV(NULL)
{}

ModuleVolume::~ModuleVolume()
{}

void ModuleVolume::ConstructVolume(G4VPhysicalVolume* worldPV,
                                   G4LogicalVolume*   worldLV)
{

  //****
  // Construct module volumes
  //****
  ConstructModules(worldLV);
   
  // Handle vis effects
  HandleVisAttributes();
}

void ModuleVolume::ConstructModules(G4LogicalVolume* worldLV)
{
  // Get material manager and config
  MaterialManager* materialManager = MaterialManager::Instance();
  Configuration* config = Configuration::Instance();
  
  //**** 
  // Module solids
  //****
  ConstructVolumes();

  //****
  // Module volumes
  //****
  ConstructVolumes();

}

void ModuleVolume::ConstructVolumes()
{
  //****
  // Build from inside out
  //****
  
  // Active LAr
  G4Box* solActiveLAr = new G4Box("solActiveLAr", 
                                  (activeLArDim[0]/2.)*cm, 
                                  (activeLArDim[1]/2.)*cm, 
                                  (activeLArDim[2]/2.)*cm);
  fVolActiveLAr = new G4LogicalVolume(solActiveLAr, 
                                      matMan->FindMaterial("G4_lAr"),
                                      "volActiveLAr"); 
  // Light US plane
  G4Box* solLightUSPlane = new G4Box("solLightUSPlane", 
                                     (activeLArDim[0]/2.)*cm, 
                                     (activeLArDim[1]/2.)*cm, 
                                     (lightDetDim[2]/2.)*cm);
  fVolLightUSPlane = new G4LogicalVolume(solLightUSPlane, 
                                         matMan->FindMaterial("G4_lAr"),
                                         "volLightUSPlane"); 
  // Light DS plane
  G4Box* solLightDSPlane = new G4Box("solLightDSPlane", 
                                     (activeLArDim[0]/2.)*cm, 
                                     (activeLArDim[1]/2.)*cm, 
                                     (lightDetDim[2]/2.)*cm);
  fVolLightDSPlane = new G4LogicalVolume(solLightDSPlane, 
                                         matMan->FindMaterial("G4_lAr"),
                                         "volLightDSPlane"); 
  // Active light
  G4Box* solActiveLight = new G4Box("solActiveLight", 
                                    (activeLArDim[0]/2.)*cm, 
                                    (activeLArDim[1]/2.)*cm, 
                                    2*solLightDSPlane->GetDZ()+solModuleActive->GetDZ());
  fVolActiveLight = new G4LogicalVolume(solActiveLight, 
                                        matMan->FindMaterial("G4_lAr"),
                                        "volActiveLight"); 
  // Field shell solid
  G4Box* solResistFieldShell_whole = new G4Box("solResistFieldShellSide_whole",
                                                solActiveLight->GetDX(),
                                                solActiveLight->GetDy()+(fieldShellThickness/2.)*cm,
                                                solActiveLight->GetDZ()+(fieldShellThickness/2.)*cm);
  // Field shell, subtract pixel/cathode walls
  G4Box* solResistFieldShell_subtract = new G4Box("solResistFieldShellSide_subtract",
                                                   2*solActiveLight->GetDX(),
                                                   solActiveLight->GetDY(),
                                                   solActiveLight->GetDZ());
  // Field shell
  G4Box* solFieldShell = new G4SubtractionSolid("FieldShell", solResistFieldShell_whole, solResistFieldShell_subtract);
  fVolFieldShell = new G4LogicalVolume(solFieldShell, 
                                       matMan->FindMaterial("G4_lAr"),
                                       "volFieldShell"); 
  // Right sub module
  Double_t subModuleDY = solActiveLight-GetDY()+2*(solResistFieldShell_whole->GetDY()-solResistFieldShell_subtract->GetDY())
  Double_t subModuleDZ = solActiveLight-GetDZ()+2*(solResistFieldShell_whole->GetDZ()-solResistFieldShell_subtract->GetDZ())
  G4Box* solRightSubModule = new G4Box("solRightSubModule",
                                        solActiveLight-GetDX(),
                                        subModuleDY,
                                        subModuleDZ);
  fVolRightSubModule = new G4LogicalVolume(solRightSubModule, 
                                           matMan->FindMaterial("G4_lAr"),
                                           "volRightSubModule"); 
  // Left sub module
  G4Box* solLeftSubModule = new G4Box("solLeftSubModule",
                                       solActiveLight-GetDX(),
                                       subModuleDY,
                                       subModuleDZ);
  fVolLeftSubModule = new G4LogicalVolume(solLeftSubModule, 
                                          matMan->FindMaterial("G4_lAr"),
                                          "volLeftSubModule"); 
  // Cathode
  G4Box* solCathode = new G4Box("solCathode",
                                (fCathodeThickness/2.)*cm,
                                solLeftSubModule->GetDY(),
                                solLeftSubModule->GetDZ());
  fVolCathode = new G4LogicalVolume(solCathode, 
                                    matMan->FindMaterial("G4_lAr"),
                                    "volCathode"); 
  // Pixel planes
  G4Box* solLeftPixelPlane = new G4Box("solLeftCathode",
                                        (fPixelPlaneThickness/2.)*cm,
                                        solLeftSubModule->GetDY(),
                                        solLeftSubModule->GetDZ());
  fVolLeftPixelPlane = new G4LogicalVolume(volLeftPixelPlane,
                                           matMan->FindMaterial("G4_lAr"),
                                           "volLeftPixelPlane"); 
  // Active module
  Double_t modDX = 2*solLeftSubModule->GetDX()+2*solLeftPixelPlane->GetDX()+solCathode->GetDX();
  G4Box* solActiveModule = new G4Box("solActiveModule",
                                      modDX,
                                      solLeftSubModule->GetDY(),
                                      solLeftSubModule->GetDZ());
  fVolActiveModule = new G4LogicalVolume(volActiveModule, 
                                        matMan->FindMaterial("G4_lAr"),
                                        "volActiveModule"); 
  // Module wall
  G4Box* solModuleWall = new G4Box("solModuleWall", 
                                    solActiveModule->GetDX()+(fModuleWallThickness/2.)*cm,
                                    solActiveModule->GetDY()+(fModuleWallThickness/2.)*cm,
                                    solActiveModule->GetDZ()+(fModuleWallThickness/2.)*cm);
  fSolModuleWall = new G4LogicalVolume(solModuleWall 
                                       matMan->FindMaterial("G4_lAr"),
                                       "volModuleWall"); 



  
  // Place them 
  // future: Reference to cyrostat center
  double l = moduleDimOD[0]/2. + moduleClearance/2.;
  G4ThreeVector translation(l*cm, l*cm, 0);
  std::string modName = "modules/module";
  std::string tpcName = "tpcs/tpc";
 
  // Modules
  new G4PVPlacement(0, translation, fModuleLV, modName+std::to_string(1), worldLV, false, 0);
  translation[0] = -1*l*cm;
  new G4PVPlacement(0, translation, fModuleLV, modName+std::to_string(2), worldLV, false, 2);
  translation[1] = -1*l*cm;
  new G4PVPlacement(0, translation, fModuleLV, modName+std::to_string(3), worldLV, false, 3);
  translation[0] = l*cm; translation[1] = -1*l*cm;
  new G4PVPlacement(0, translation, fModuleLV, modName+std::to_string(4), worldLV, false, 4); 

  // TPCs
  new G4PVPlacement(0, G4ThreeVector(), fTPCLV, tpcName+std::to_string(1), fModuleLV, false, 0);
}

void ModuleVolume::HandleVisAttributes()
{
  // Module
  G4VisAttributes* moduleVA = new G4VisAttributes(G4Colour(0,1,0));
  moduleVA->SetForceWireframe(true);
  fModuleLV->SetVisAttributes(moduleVA); 

  // TPC
  G4VisAttributes* tpcVA = new G4VisAttributes(G4Colour(1,1,1));
  tpcVA->SetForceWireframe(false);
  fTPCLV->SetVisAttributes(tpcVA); 
}

}
