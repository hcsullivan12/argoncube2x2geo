// 
// File: DiskVolume.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to construct disk volume.
//

#include "DiskVolume.h"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Tubs.hh"

namespace majorana
{

DiskVolume::DiskVolume()
{}

DiskVolume::~DiskVolume()
{}

void DiskVolume::ConstructVolume(const unsigned&  nMPPCs, 
                                 const float&     mppcArea,
                                 const float&     diskRadius,
                                 const float&     diskThickness,
                                 MaterialManager* materialManager,
                                 G4LogicalVolume* worldLogicalVol)
{
  m_diskSolid      = new G4Tubs("Disk", 0, diskRadius, diskThickness/2.0, 0, twopi);
  m_diskLogicalVol = new G4LogicalVolume(m_diskSolid, materialManager->FindMaterial("Acrylic"), "Disk");  
  G4PVPlacement* diskPhysicalVol = new G4PVPlacement(0, G4ThreeVector(), m_diskLogicalVol, "Disk", worldLogicalVol, false, 0);
}

G4LogicalVolume* DiskVolume::LogicalVolume()
{
  return m_diskLogicalVol;
}

}
