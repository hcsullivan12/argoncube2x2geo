// 
// File: DiskVolume.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to construct disk volume.
//

#include "DiskVolume.h"

#include "G4SystemOfUnits.hh"

namespace majorana
{

DiskVolume::DiskVolume(MaterialManager* materialManager)
{
  this->m_materialManager = materialManager;
}

DiskVolume::~DiskVolume()
{}

void DiskVolume::ConstructVolume()
{

}

G4LogicalVolume* DiskVolume::LogicalVolume()
{

}

}
