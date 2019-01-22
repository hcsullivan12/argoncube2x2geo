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

    G4SubtractionSolid* fModuleSolid;
    G4LogicalVolume*    fModuleLV;
    G4Box*              fTPCSolid;
    G4LogicalVolume*    fTPCLV;
    G4OpticalSurface*   m_opticalSurface; 
};
}

#endif
