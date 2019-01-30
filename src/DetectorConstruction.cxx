// 
// File: DetectorConstruction.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to construct our detector.
//

#include "DetectorConstruction.h"
#include "Configuration.h"
#include "MaterialManager.h"

#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4Box.hh"

namespace geo
{

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  fModuleVolume(NULL),
  fWorldSolid(NULL),
  fWorldLV(NULL),
  fWorldPV(NULL)
{}

DetectorConstruction::~DetectorConstruction()
{
  if (fModuleVolume) delete fModuleVolume;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Construct the materials
  ConstructMaterials();
  
  // Construct our detector 
  ConstructDetector();

  if (!fWorldPV) 
  {
    G4cerr << "DetectorConstruction::Construct() "
           << "Error! World physical volume not initialized!" 
           << G4endl;
  }
  return fWorldPV;
}

void DetectorConstruction::ConstructMaterials()
{
  // Get the material manager
  MaterialManager* materialManager = MaterialManager::Instance();

  if (!materialManager) 
  {
    G4cerr << "DetectorConstruction::InitializeMaterials() "
           << "Error! Material manager not initialized!" << G4endl;
  }
  materialManager->ConstructMaterials();
}

void DetectorConstruction::ConstructDetector()
{
  // Get instance of material manager and configuration
  MaterialManager* materialManager = MaterialManager::Instance();
  Configuration* config = Configuration::Instance();

  //**** World
  std::vector<double> worldDim = config->WorldDimensions();
  fWorldSolid = new G4Box("WorldSolid", 
                          (worldDim[0]/2.)*cm,
                          (worldDim[1]/2.)*cm, 
                          (worldDim[2]/2.)*cm);
  fWorldLV    = new G4LogicalVolume(fWorldSolid, 
                                    materialManager->FindMaterial("G4_AIR"), 
                                    "WorldLV");
  fWorldPV = new G4PVPlacement(0, 
                               G4ThreeVector(), 
                               fWorldLV, 
                               "World", 
                               0, 
                               false, 
                               0);
  // vis
  G4Colour worldC(0.5,0.5,0.5);
  G4VisAttributes* worldVA = new G4VisAttributes(worldC);
  worldVA->SetForceWireframe(true);
  fWorldLV->SetVisAttributes(worldVA); 

  //****
  // Cryostat
  //****
  fCryostatVolume = new CryostatVolume();
  fCryostatVolume->ConstructVolume(fWorldPV, fWorldLV);

  //**** 
  // Modules
  //****
  fModuleVolume = new ModuleVolume();
  fModuleVolume->ConstructVolume(fWorldPV, fWorldLV);
}

void DetectorConstruction::ConstructSDandField()
{}

}
