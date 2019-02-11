// 
// File: ModuleActive.h
//
// Author: Hunter Sullivan
//

#ifndef MODULEACTIVE_H
#define MODULEACTIVE_H

#include "G4LogicalVolume.hh"

namespace geo
{

class ModuleActive 
{
  public:
    ModuleActive();
    ~ModuleActive();

    void ConstructVolume();

    G4LogicalVolume* GetLV() { return fVolActiveModule; };

  private: 
    void ConstructSubVolumes();
    void PlaceSubVolumes();

    // solids
    G4LogicalVolume* fVolActiveLAr;
    G4LogicalVolume* fVolLightUSPlane;
    G4LogicalVolume* fVolLightDSPlane;
    G4LogicalVolume* fVolActiveLight;
    G4LogicalVolume* fVolFieldShell;
    G4LogicalVolume* fVolRightSubModule;
    G4LogicalVolume* fVolLeftSubModule;
    G4LogicalVolume* fVolCathode;
    G4LogicalVolume* fVolLeftPixelPlane;
    G4LogicalVolume* fVolRightPixelPlane;
    G4LogicalVolume* fVolActiveContainer;
    G4LogicalVolume* fVolActiveModuleWall;
    G4LogicalVolume* fVolActiveModule;

    G4LogicalVolume* fVolModuleLeg;
    G4LogicalVolume* fVolBottomDummyFlange;
    G4LogicalVolume* fVolModuleLegContainer;
};
}

#endif
