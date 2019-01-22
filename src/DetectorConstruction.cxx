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

namespace geo
{

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  fWheelVolume(NULL),
  fWorldSolid(NULL),
  fWorldLV(NULL),
  fWorldPV(NULL)
{
  // Pass configuration to our other volumes
  Configuration* config = Configuration::Instance();
  fWheelVolume = new WheelVolume(8,
                                 0.01,
                                 14.5, 
                                 1.0);
}

DetectorConstruction::~DetectorConstruction()
{
  if (fWheelVolume) delete fWheelVolume;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  InitializeMaterials();
  InitializeDetector();

  if (!fWorldPV) 
  {
    G4cerr << "DetectorConstruction::Construct() Error! WorldPV not initialized!\n";
  }

  return fWorldPV;
}

void DetectorConstruction::InitializeMaterials()
{
  // Construct materials
  MaterialManager* materialManager = MaterialManager::Instance();

  if (!materialManager) 
  {
    G4cerr << "DetectorConstruction::InitializeMaterials() Error! Material manager not initialized!\n";
  }
  materialManager->ConstructMaterials();
}

void DetectorConstruction::InitializeDetector()
{
  if (!fWheelVolume) 
  {
    G4cerr << "DetectorConstruction::InitializeDetector() Error! Disk volume not initialized!\n";
  }
  // Get instance of material manager
  MaterialManager* materialManager = MaterialManager::Instance();

  //**** World
  G4double diskRadius    = fWheelVolume->Radius();
  G4double diskThickness = fWheelVolume->Thickness();
  fWorldSolid = new G4Box("WorldSolid", 
                           diskRadius*4*cm, 
                           diskRadius*4*cm, 
                           diskThickness*4*cm);
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
  G4Colour worldC(0,0,0);
  G4VisAttributes* worldVA = new G4VisAttributes(worldC);
  worldVA->SetForceWireframe(true);
  fWorldLV->SetVisAttributes(worldVA); 

  //**** Wheel
  fWheelVolume->ConstructVolume(fWorldPV, fWorldLV);
}

void DetectorConstruction::ConstructSDandField()
{}
}
