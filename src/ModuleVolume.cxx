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
  // Module structure
  //****
  // Module walls
  std::vector<double> moduleDimOD = config->ModuleDimensions();
  double moduleG10WallThickness   = config->ModuleG10WallThickness();
  double moduleClearance          = config->ModuleClearance();
  std::vector<double> moduleDimID = {moduleDimOD[0] - 2*moduleG10WallThickness,
                                     moduleDimOD[1] - 2*moduleG10WallThickness,
                                     moduleDimOD[2] - 2*moduleG10WallThickness};
  G4Box* moduleSolidOD = new G4Box("ModuleSolidOD", 
                                   (moduleDimOD[0]/2.)*cm, 
                                   (moduleDimOD[1]/2.)*cm, 
                                   (moduleDimOD[2]/2.)*cm);
  G4Box* moduleSolidID = new G4Box("ModuleSolidID", 
                                   (moduleDimID[0]/2.)*cm, 
                                   (moduleDimID[1]/2.)*cm, 
                                   (moduleDimID[2]/2.)*cm);
  fModuleSolid = new G4SubtractionSolid("ModuleSolid", moduleSolidOD, moduleSolidID);
  fModuleLV    = new G4LogicalVolume(fModuleSolid,
                                     materialManager->FindMaterial("Acrylic"),
                                     "ModuleLV");

  //****
  // Active volume
  //****
  fTPCSolid = new G4Box("TPCSolid", 
                        (moduleDimID[0]/2.)*cm, 
                        (moduleDimID[1]/2.)*cm, 
                        (moduleDimID[2]/2.)*cm);
  fTPCLV    = new G4LogicalVolume(fTPCSolid, 
                                  materialManager->FindMaterial("G4_lAr"),
                                  "TPCLV");  
  
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
