// 
// File: ModuleVolume.h
//
// Author: Hunter Sullivan
//
// Discription: Class to construct module volumes.
//

#ifndef MODULEVOLUME_H
#define MODULEVOLUME_H

#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

namespace geo
{

class ModuleVolume 
{
  public:
    ModuleVolume();
    ~ModuleVolume();

    void ConstructVolume(G4VPhysicalVolume* worldPV,
                         G4LogicalVolume*   worldLV);

  private: 
    void ConstructModules(G4LogicalVolume* worldLV);
    void ConstructSubVolumes();
    void PlaceVolumes(G4LogicalVolume* worldLV);
    void PlaceSubVolumes();
    void ConstructActiveVolume();
    void ConstructBottomVolume();
    void ConstructTopVolume();

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
    G4LogicalVolume* fVolActiveModule;
    G4LogicalVolume* fVolModuleWall;
    G4LogicalVolume* fVolModuleLeg;
    G4LogicalVolume* fVolBottomDummyFlange;
    G4LogicalVolume* fVolModule;
    G4LogicalVolume* fVolLegContainer;
};
}

#endif
