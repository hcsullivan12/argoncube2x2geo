// 
// File: ModuleTop.h
//
// Author: Hunter Sullivan
//

#ifndef MODULETOP_H
#define MODULETOP_H

#include "G4LogicalVolume.hh"

namespace geo
{

class ModuleTop 
{
  public:
    ModuleTop();
    ~ModuleTop();

    void ConstructVolume();

    G4LogicalVolume* GetLV() { return fVolModuleTop; };

  private: 
    void ConstructSubVolumes();
    void PlaceSubVolumes();

    G4LogicalVolume* fVolModuleTop;
    G4LogicalVolume* fVolTopGAr;
    G4LogicalVolume* fVolTopLAr;
};
}

#endif
