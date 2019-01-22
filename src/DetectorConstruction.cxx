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

namespace majorana
{

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  m_wheelVolume(NULL),
  m_worldSolid(NULL),
  m_worldLV(NULL),
  m_worldPV(NULL)
{
  // Pass configuration to our other volumes
  Configuration* config = Configuration::Instance();
  m_wheelVolume = new WheelVolume(config->NMPPCs(),
                                  config->MPPCArea(),
                                  config->DiskRadius(), 
                                  config->DiskThickness());
}

DetectorConstruction::~DetectorConstruction()
{
  if (m_wheelVolume) delete m_wheelVolume;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  InitializeMaterials();
  InitializeDetector();

  if (!m_worldPV) 
  {
    G4cerr << "DetectorConstruction::Construct() Error! WorldPV not initialized!\n";
  }

  return m_worldPV;
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
  if (!m_wheelVolume) 
  {
    G4cerr << "DetectorConstruction::InitializeDetector() Error! Disk volume not initialized!\n";
  }
  // Get instance of material manager
  MaterialManager* materialManager = MaterialManager::Instance();

  //**** World
  G4double diskRadius    = m_wheelVolume->Radius();
  G4double diskThickness = m_wheelVolume->Thickness();
  m_worldSolid = new G4Box("WorldSolid", 
                           diskRadius*4*cm, 
                           diskRadius*4*cm, 
                           diskThickness*4*cm);
  m_worldLV    = new G4LogicalVolume(m_worldSolid, 
                                     materialManager->FindMaterial("G4_AIR"), 
                                     "WorldLV");
  m_worldPV = new G4PVPlacement(0, 
                                G4ThreeVector(), 
                                m_worldLV, 
                                "World", 
                                0, 
                                false, 
                                0);
  // vis
  G4Colour worldC(0,0,0);
  G4VisAttributes* worldVA = new G4VisAttributes(worldC);
  worldVA->SetForceWireframe(true);
  m_worldLV->SetVisAttributes(worldVA); 

  //**** Wheel
  m_wheelVolume->ConstructVolume(m_worldPV, m_worldLV);
}

void DetectorConstruction::ConstructSDandField()
{
  if (!m_wheelVolume) return;
 
  // MPPC SD
  if (!m_mppcSD.Get()) 
  {
    //G4cout << "Construction mppcSD..." << G4endl;
    MPPCSD* mppcSD = new MPPCSD("mppcSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(mppcSD);
    m_mppcSD.Put(mppcSD);
  }
  
  SetSensitiveDetector("MPPCLV", m_mppcSD.Get(), true);
}
}
