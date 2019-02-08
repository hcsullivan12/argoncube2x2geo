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
#include "Utilities.h"

#include "G4SDManager.hh"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh"

namespace geo
{

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  fModule(NULL),
  fVolWorld(NULL)
{}

DetectorConstruction::~DetectorConstruction()
{
  if (fModule) delete fModule;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Construct the materials
  ConstructMaterials();
  
  // Construct our detector 
  ConstructDetector();

  if (!fPVWorld) 
  {
    G4cerr << "DetectorConstruction::Construct() "
           << "Error! World physical volume not initialized!" 
           << G4endl;
  }
  return fPVWorld;
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
  arcutil::Utilities util;

  //**** 
  // World
  //****
  std::vector<G4double> worldDim = config->WorldDim(); util.ConvertToUnits(worldDim);

  G4Box* solWorld = new G4Box("solWorld", 
                              worldDim[0]/2.,
                              worldDim[1]/2., 
                              worldDim[2]/2.);
  fVolWorld    = new G4LogicalVolume(solWorld, 
                                    materialManager->FindMaterial("Air"), 
                                    "volWorld");
  fPVWorld = new G4PVPlacement(0, 
                               G4ThreeVector(), 
                               fVolWorld, 
                               "World", 
                               0, 
                               false, 
                               0); 
  
  //****
  // Detector 
  //****
  fDetector = new Detector();
  fDetector->ConstructVolume();

  //****
  // Cryostat
  //****
  fCryostat = new Cryostat();
  fCryostat->ConstructVolume(fVolWorld, fDetector);
}

void DetectorConstruction::ConstructSDandField()
{}

}
