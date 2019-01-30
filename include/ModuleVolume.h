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
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"

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
    void HandleSurfaces(G4VPhysicalVolume* worldPV);
    void HandleVisAttributes();

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
    G4LogicalVolume* fVolActiveModule;
    G4LogicalVolume* fVolModuleWall;
};
}

#endif
