// 
// File: Module.h
//
// Author: Hunter Sullivan
//

#ifndef MODULE_H
#define MODULE_H

#include "Geometry/Module/ModuleActive.h"
#include "Geometry/Module/ModuleBottom.h"
#include "Geometry/Module/ModuleTop.h"

#include "G4LogicalVolume.hh"

namespace geo
{

class Module 
{
  public:
    Module();
    ~Module();

    void ConstructVolume();

    G4LogicalVolume* GetLV() { return fVolModule; };

  private: 
    void ConstructModules();
    void ConstructSubVolumes();
    void PlaceSubVolumes();

    ModuleActive* fModuleActive;
    ModuleBottom* fModuleBottom;
    ModuleTop*    fModuleTop;

    G4LogicalVolume* fVolModule;
    G4LogicalVolume* fVolModuleMiddleFrame;
    G4LogicalVolume* fVolModuleBottomWall;

    G4double fModuleMiddleFrameYHalfL;

};
}

#endif
