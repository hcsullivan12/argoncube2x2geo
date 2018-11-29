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
   m_mppcArea(mppcArea),
   m_opticalSurface(NULL),
   m_diskSolid(NULL),
   m_diskLV(NULL),
   m_mppcSolid(NULL),
   m_mppcLV(NULL)
{
  // Initialize mppc array
 // m_mppcVolVec.resize(nMPPCs, MPPCVolume(mppcArea));
}

DiskVolume::~DiskVolume()
{}

void DiskVolume::ConstructVolume(MaterialManager* materialManager,
                                 G4LogicalVolume* worldLV)
{
  // Construct disk volume
  m_diskSolid      = new G4Tubs("DiskSolid", 
                                 0, 
                                 m_diskRadius*cm, 
                                 (m_diskThickness/2.0)*cm, 
                                 0, 
                                 twopi);
  m_diskLV = new G4LogicalVolume(m_diskSolid, 
                                 materialManager->FindMaterial("Acrylic"), 
                                "DiskLV");  
  G4ThreeVector transVec(0,0,(m_diskThickness/2.0)*cm);
  G4PVPlacement* diskPV = new G4PVPlacement(0, 
                                            transVec, 
                                            m_diskLV, 
                                           "Disk", 
                                            worldLV, 
                                            false, 
                                            0);

  // Construct mppcs
  m_mppcSideLength = std::sqrt(m_mppcArea);  
  m_mppcSolid      = new G4Box("mppcSolid", 
                               m_mppcSideLength*cm, 
                               m_mppcSideLength*cm, 
                               (m_mppcSideLength/2.0)*cm);
  m_mppcLV = new G4LogicalVolume(m_mppcSolid, 
                                 materialManager->FindMaterial("G4_Al"), 
                                 "mppcLV"); 
  // Place mppcs around the disk
  for (unsigned m = 1; m <= m_nMPPCs; m++)
  {
    G4double angSepDeg = 360.0/m_nMPPCs;
    G4double r         = m_diskRadius;
    G4double thetaDeg  = (m - 1)*angSepDeg;
    // Offset 
    r = r + (m_mppcSideLength/2.0);
    PlaceMPPC(worldLV, r, thetaDeg, m-1);
  }

  // Handle surfaces
 // HandleSurfaces();
  // Handle vis effects
  HandleVisAttributes();
}

void DiskVolume::PlaceMPPC(G4LogicalVolume* worldLV,
                           const G4double&  r,
                           const G4double&  thetaDeg,
                           const unsigned&  m)
{
  G4double x(0), y(0), z(m_diskThickness/2.0);
  ConvertToCartesian(x, y, r, thetaDeg);

  // Apply rotation
  G4RotationMatrix* zRot = new G4RotationMatrix;
  zRot->rotateZ(thetaDeg*deg);  
  // Apply translation
  G4ThreeVector transVec(x*cm, y*cm, z*cm);
    
  G4PVPlacement* mppcPV = new G4PVPlacement(zRot, 
                                            transVec, 
                                            m_mppcLV, 
                                            "mppc", 
                                            worldLV, 
                                            false, 
                                            m);
}


void DiskVolume::HandleSurfaces()
{
  MaterialManager* materialManager = MaterialManager::Instance();

  // Boundary surface
  // TODO: Surface roughness
  m_opticalSurface = new G4OpticalSurface("AirAcrylicOS",
                                          glisur,
                                          ground,
                                          dielectric_dielectric,
                                          0.0);
  new G4LogicalSkinSurface("AirAcrylicBS",
                            m_diskLV,
                            m_opticalSurface);
  // Set the air surface properties
  G4MaterialPropertiesTable* surfaceProp = materialManager->FindMaterial("G4_AIR")->GetMaterialPropertiesTable();
  m_opticalSurface->SetMaterialPropertiesTable(surfaceProp);
}

void DiskVolume::HandleVisAttributes()
{
  // Disk
  G4VisAttributes* diskVA = new G4VisAttributes(G4Colour(1,1,1));
  diskVA->SetForceWireframe(true);
  m_diskLV->SetVisAttributes(diskVA); 
  // MPPC
  G4VisAttributes* mppcVA = new G4VisAttributes(G4Colour(1,0,0));
  mppcVA->SetForceSolid(true);
  m_mppcLV->SetVisAttributes(mppcVA); 
}

}
