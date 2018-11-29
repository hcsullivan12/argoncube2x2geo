// 
// File: DiskVolume.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to construct disk volume.
//

#include "DiskVolume.h"

#include "G4Colour.hh"
#include "G4VisAttributes.hh"

namespace majorana
{

DiskVolume::DiskVolume(const unsigned& nMPPCs, 
                       const G4double& mppcArea,
                       const G4double& diskRadius,
                       const G4double& diskThickness)
 : m_diskRadius(diskRadius), 
   m_diskThickness(diskThickness),
   m_nMPPCs(nMPPCs),
   m_opticalSurface(NULL)
{
  // Initialize mppc array
  m_mppcVolVec.resize(nMPPCs, MPPCVolume(mppcArea));
}

DiskVolume::~DiskVolume()
{}

void DiskVolume::ConstructVolume(MaterialManager* materialManager,
                                 G4LogicalVolume* worldLogicalVol)
{
  // Construct disk volume
  m_diskSolid      = new G4Tubs("Disk", 0, m_diskRadius*cm, (m_diskThickness/2.0)*cm, 0, twopi);
  // Translation vec
  G4ThreeVector transVec(0,0,(m_diskThickness/2.0)*cm);
  m_diskLogicalVol = new G4LogicalVolume(m_diskSolid, materialManager->FindMaterial("Acrylic"), "Disk");  

  // Boundary surface
  // TODO: Surface roughness
  m_opticalSurface = new G4OpticalSurface("AirAcrylicOS",
                                          glisur,
                                          ground,
                                          dielectric_dielectric,
                                          0.0);
  new G4LogicalSkinSurface("AirAcrylicBS",
                            m_diskLogicalVol,
                            m_opticalSurface);
  // Set the air surface properties
  G4MaterialPropertiesTable* surfaceProp = materialManager->FindMaterial("G4_AIR")->GetMaterialPropertiesTable();
  m_opticalSurface->SetMaterialPropertiesTable(surfaceProp);
 
  // Construct mppcs
  unsigned mppcID(1);
  for (auto mppcVol : m_mppcVolVec)
  {
    // Name and position
    G4String thisName         = "MPPC" + std::to_string(mppcID);
    G4double angSeperationDeg = 360.0/m_nMPPCs;
    G4double r        = m_diskRadius;
    G4double thetaDeg = (mppcID - 1)*angSeperationDeg;
    
    mppcVol.ConstructVolume(thisName, r, thetaDeg, m_diskThickness, materialManager, worldLogicalVol);
    mppcID++;
  }

  // Vis effects
  G4Colour color(0,1,0);
  G4VisAttributes* visAttributes = new G4VisAttributes(color);
  visAttributes->SetForceWireframe(true);
  m_diskLogicalVol->SetVisAttributes(visAttributes); 

  G4PVPlacement* diskPhysicalVol = new G4PVPlacement(0, transVec, m_diskLogicalVol, "Disk", worldLogicalVol, false, 0);
}
}
