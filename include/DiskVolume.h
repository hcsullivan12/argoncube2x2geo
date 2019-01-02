// 
// File: DiskVolume.h
//
// Author: Hunter Sullivan
//
// Discription: Class to construct disk volume.
//

#ifndef DISKVOLUME_H
#define DISKVOLUME_H

#include "MaterialManager.h"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"

namespace majorana
{

class DiskVolume 
{
  public:
    DiskVolume(const unsigned& nMPPCs,                                   
               const G4double& mppcArea,
               const G4double& diskRadius,
               const G4double& diskThickness);
    ~DiskVolume();

    void ConstructVolume(G4VPhysicalVolume* worldPV,
                         G4LogicalVolume*   worldLV);

    const G4double   Radius()    const { return m_diskRadius; };
    const G4double   Thickness() const { return m_diskThickness; }; 
    G4LogicalVolume* MPPCLV()    const { return m_mppcLV; };

  private: 
    void HandleSurfaces(G4VPhysicalVolume* worldPV);
    void HandleVisAttributes();
    void PlaceMPPC(G4LogicalVolume* worldLV,
                   const G4double&  r,
                   const G4double&  thetaDeg,
                   const unsigned&  m);
    inline void ConvertToCartesian(G4double&       x, 
                                   G4double&       y, 
                                   const G4double& r, 
                                   const G4double& thetaDeg)
    {
      x = r*std::cos(thetaDeg*pi/180);
      y = r*std::sin(thetaDeg*pi/180);
    };



    G4double           m_diskRadius;
    G4double           m_diskThickness;
    unsigned           m_nMPPCs;
    G4VSolid*          m_diskSolid;
    G4LogicalVolume*   m_diskLV;
    G4PVPlacement*     m_diskPV;
    G4VSolid*          m_mppcSolid;
    G4LogicalVolume*   m_mppcLV;
    G4OpticalSurface*  m_opticalSurface;
    G4double           m_mppcArea;
    G4double           m_mppcSideLength;

};
}

#endif
