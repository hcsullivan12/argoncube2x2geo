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
  // Pass configuration to our other volumes
  m_diskVolume = new DiskVolume(config.NMPPCs(), 
                                config.MPPCArea(), 
                                config.DiskRadius(), 
                                config.DiskThickness());
  m_materialManager = NULL;
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
  G4double diskRadius    = m_diskVolume->Radius();
  G4double diskThickness = m_diskVolume->Thickness(); 
  m_worldSolid       = new G4Box("World", diskRadius*4*cm, diskRadius*4*cm, diskThickness*4*cm);
  m_worldLogicalVol  = new G4LogicalVolume(m_worldSolid, m_materialManager->FindMaterial("G4_AIR"), "World");
  m_worldPhysicalVol = new G4PVPlacement(0, G4ThreeVector(), m_worldLogicalVol, "World", 0, false, 0);

  // -----------
  // Disk
  // -----------
  m_diskVolume->ConstructVolume(m_materialManager, m_worldLogicalVol);
}
}
