// 
// File: DetectorConstruction.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to construct our detector.
//

#include "DetectorConstruction.h"

namespace majorana
{

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{
  if (m_materialManager) delete m_materialManager;
  if (m_diskVolume)      delete m_diskVolume;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  InitializeMaterials();
  InitializeDetector();

  return m_worldPhysicalVol;
}

void DetectorConstruction::InitializeMaterials()
{
  // Construct materials
  m_materialManager = new MaterialManager();
  m_materialManager->ConstructMaterials();
}

void DetectorConstruction::InitializeDetector()
{
  // -----------
  // World
  // -----------
  // TODO: config parameters
  m_worldSolid       = new G4Box("World", 10, 10, 10);
  m_worldLogicalVol  = new G4LogicalVolume(m_worldSolid, m_materialManager->FindMaterial("G4_Air"), "World");
  m_worldPhysicalVol = new G4PVPlacement(0, G4ThreeVector(), m_worldLogicalVol, "World", 0, false, 0);

  // -----------
  // Disk
  // -----------
  m_diskVolume = new DiskVolume(m_materialManager);
  m_diskVolume->ConstructVolume();
  new G4PVPlacement(0, G4ThreeVector(), m_diskVolume->LogicalVolume(), "Disk", m_worldLogicalVol, false, 0);
}
}
