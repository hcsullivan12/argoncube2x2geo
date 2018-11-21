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

namespace majorana
{

class DiskVolume 
{
  public:
    DiskVolume();
    ~DiskVolume();

    void             ConstructVolume(const unsigned&  nMPPCs, 
                                     const float&     mppcArea,
                                     const float&     diskRadius,
                                     const float&     diskThickness,
                                     MaterialManager* materialManager,
                                     G4LogicalVolume* worldLogicalVol);
    G4LogicalVolume* LogicalVolume();

  private:
    G4VSolid*         m_diskSolid;
    G4LogicalVolume*  m_diskLogicalVol;
};
}

#endif
