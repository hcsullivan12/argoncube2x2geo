// 
// File: Module.h
//
// Author: Hunter Sullivan
//
// Discription: Class to construct module volumes.
//

#ifndef MODULE_H
#define MODULE_H

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
    void PlaceVolumes();
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
    G4LogicalVolume* fVolTopModule;
    G4LogicalVolume* fVolTopLAr;
    G4LogicalVolume* fVolTopGAr;
};
}

#endif
