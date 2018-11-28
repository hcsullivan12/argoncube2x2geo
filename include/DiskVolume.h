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
#include "MPPCVolume.h"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Tubs.hh"

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

    void ConstructVolume(MaterialManager* materialManager,
                         G4LogicalVolume* worldLogicalVol);

    const G4double Radius()    const { return m_diskRadius; };
    const G4double Thickness() const { return m_diskThickness; }; 

  private: 
    G4double                m_diskRadius;
    G4double                m_diskThickness;
    unsigned                m_nMPPCs;
    G4VSolid*               m_diskSolid;
    G4LogicalVolume*        m_diskLogicalVol;
    std::vector<MPPCVolume> m_mppcVolVec;
};
}

#endif
