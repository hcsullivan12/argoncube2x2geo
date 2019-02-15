// 
// File: DetectorConstruction.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to construct our detector.
//

#include "DetectorConstruction.h"
#include "QuantityStore.h"
#include "MaterialManager.h"

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
  MaterialManager* matMan = MaterialManager::Instance();

  if (!matMan) 
  {
    G4cerr << "DetectorConstruction::InitializeMaterials() "
           << "Error! Material manager not initialized!" << G4endl;
  }
  matMan->ConstructMaterials();
}

void DetectorConstruction::ConstructDetector()
{
  // Get instance of material manager and qStoreuration
  MaterialManager* matMan = MaterialManager::Instance();
  QuantityStore* qStore = QuantityStore::Instance();

  //**** 
  // World
  //****
  std::vector<G4double> worldDim = qStore->kWorldDim; 

  G4Box* solWorld = new G4Box("solWorld", 
                              worldDim[0]/2.,
                              worldDim[1]/2., 
                              worldDim[2]/2.);
  fVolWorld    = new G4LogicalVolume(solWorld, 
                                    matMan->FindMaterial("Air"), 
                                    "volWorld"); 
  //****
  // Placement
  //****
  fPVWorld = new G4PVPlacement(0,
                               G4ThreeVector(),
                               fVolWorld,
                               fVolWorld->GetName(),
                               0,
                               false,
                               0);   

  //****
  // DetEnclosure
  //****
  G4Box* solDetEnclosure = new G4Box("solDetEnclosure", 
                                      worldDim[0]/4.,
                                      worldDim[1]/4., 
                                      worldDim[2]/4.);
  fVolDetEnclosure    = new G4LogicalVolume(solDetEnclosure, 
                                            matMan->FindMaterial("Air"), 
                                            "volDetEnclosure");

  //****
  // Detector 
  //****
  fDetector = new Detector();
  fDetector->ConstructVolume();

  //****
  // Cryostat
  //****
  fCryostat = new Cryostat();
  fCryostat->ConstructVolume(fVolDetEnclosure, fDetector); 
}

void DetectorConstruction::ConstructSDandField()
{}

}
