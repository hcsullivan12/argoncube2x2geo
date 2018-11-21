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

namespace majorana
{

class DiskVolume 
{
  public:
    DiskVolume(MaterialManager* materialManager);
    ~DiskVolume();

    void             ConstructVolume();
    G4LogicalVolume* LogicalVolume();

  private:
 
    MaterialManager*  m_materialManager;
    G4LogicalVolume*  m_diskLogicalVolume;
};
}

#endif
