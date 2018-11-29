// 
// File: MPPCVolume.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to construct mppc volume.
//

#include "MPPCVolume.h"

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

namespace majorana
{

MPPCVolume::MPPCVolume(const G4double& mppcArea)
 : m_mppcArea(mppcArea)
{}

MPPCVolume::~MPPCVolume()
{}

void MPPCVolume::ConstructVolume(const G4String&  name,
                                 G4double&        r,
                                 const G4double&  thetaDeg,
                                 const G4double&  diskThickness,
                                 MaterialManager* materialManager,
                                 G4LogicalVolume* worldLogicalVol)
{
  //TODO: Add z length of MPPC?

  // Side lengths
  m_mppcSideLength = std::sqrt(m_mppcArea);
  // Convert r, theta to x,y coordinates
  G4double x(0), y(0), z(diskThickness/2.0);
  // Offset 
  r = r + (m_mppcSideLength/2.0);
  ConvertToCartesian(x, y, r, thetaDeg);
  // Construct mppc volume
  G4String nameLV  = name + "LV";
  m_mppcSolid      = new G4Box(name, m_mppcSideLength*cm, m_mppcSideLength*cm, (m_mppcSideLength/2.0)*cm);
  m_mppcLogicalVol = new G4LogicalVolume(m_mppcSolid, materialManager->FindMaterial("G4_Al"), nameLV);  
  // Apply rotation
  G4RotationMatrix* zRot = new G4RotationMatrix;
  zRot->rotateZ(thetaDeg*deg);  
  // Apply translation
  G4ThreeVector transVec(x*cm, y*cm, z*cm);
  // Vis effects
  G4Colour color(1,0,0);
  G4VisAttributes* visAttributes = new G4VisAttributes(color);
  m_mppcLogicalVol->SetVisAttributes(visAttributes); 
   
  G4PVPlacement* mppcPhysicalVol = new G4PVPlacement(zRot, transVec, m_mppcLogicalVol, name, worldLogicalVol, false, 0);
}

 


}
