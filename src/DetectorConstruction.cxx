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

DetectorConstruction::DetectorConstruction(const Configuration& config)
: G4VUserDetectorConstruction()
{
  m_nMPPCs          = config.NMPPCs();
  m_mppcArea        = config.MPPCArea();
  m_diskRadius      = config.DiskRadius();
  m_diskThickness   = config.DiskThickness();
  m_materialManager = NULL;
  m_diskVolume      = NULL;
}

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
  m_worldSolid       = new G4Box("World", m_diskRadius*4, m_diskRadius*4, m_diskThickness*4);
  m_worldLogicalVol  = new G4LogicalVolume(m_worldSolid, m_materialManager->FindMaterial("G4_AIR"), "World");
  m_worldPhysicalVol = new G4PVPlacement(0, G4ThreeVector(), m_worldLogicalVol, "World", 0, false, 0);

  // -----------
  // Disk
  // -----------
  m_diskVolume = new DiskVolume();
  m_diskVolume->ConstructVolume(m_nMPPCs, m_mppcArea, m_diskRadius, m_diskThickness, m_materialManager, m_worldLogicalVol);
}
}
