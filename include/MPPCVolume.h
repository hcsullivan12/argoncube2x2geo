// 
// File: MPPCVolume.h
//
// Author: Hunter Sullivan
//
// Discription: Class to construct MPPC volume.
//

#ifndef MPPCVOLUME_H
#define MPPCVOLUME_H

#include "MaterialManager.h"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

namespace majorana
{

class MPPCVolume 
{
  public:
    MPPCVolume(const G4double& mppcArea);
    ~MPPCVolume();

    void ConstructVolume(const G4String&  name,
                         G4double&        r,
                         const G4double&  thetaDeg,
                         const G4double&  diskThickness, 
                         MaterialManager* materialManager,
                         G4LogicalVolume* diskLogicalVol);

  private:
    inline void ConvertToCartesian(G4double&       x, 
                                   G4double&       y, 
                                   const G4double& r, 
                                   const G4double& thetaDeg)
    {
      x = r*std::cos(thetaDeg*pi/180);
      y = r*std::sin(thetaDeg*pi/180);
    };


    G4double         m_mppcArea;
    G4double         m_mppcSideLength;
    G4VSolid*        m_mppcSolid;
    G4LogicalVolume* m_mppcLogicalVol;
};
}

#endif
