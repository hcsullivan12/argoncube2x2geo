// 
// File: ModuleBottom.h
//
// Author: Hunter Sullivan
//

#ifndef MODULEBOTTOM_H
#define MODULEBOTTOM_H

#include "G4LogicalVolume.hh"

namespace geo
{

class ModuleBottom 
{
  public:
    ModuleBottom();
    ~ModuleBottom();

    void ConstructVolume();

    G4LogicalVolume* GetLV() { return fVolModuleBottom; };

  private: 
    void ConstructModules();
    void ConstructSubVolumes();
    void PlaceSubVolumes();

    G4LogicalVolume* fVolModuleBottom;
    G4LogicalVolume* fVolBottomDummyFlange;
    G4LogicalVolume* fVolModuleLegContainer;
    G4LogicalVolume* fVolModuleLeg;
};
}

#endif
