// 
// File: CryostatFT.h
//
// Author: Hunter Sullivan
//

#ifndef CRYOSTATFT_H
#define CRYOSTATFT_H

#include "G4LogicalVolume.hh"

namespace geo
{

class CryostatFT 
{
  public:
    CryostatFT();
    ~CryostatFT();

    void ConstructVolume();

    G4LogicalVolume* GetLV() { return fVolTopContainer; };

  private: 
    void ConstructSubVolumes();
    void PlaceSubVolumes();

    G4LogicalVolume* fVolTopContainer;
    G4LogicalVolume* fVolModuleTopWall;
    G4LogicalVolume* fVolModuleFTContainer;
    G4LogicalVolume* fVolModuleMedFT;
    G4LogicalVolume* fVolModuleSmallFT;
    G4LogicalVolume* fVolModuleFlange;
    G4LogicalVolume* fVolTopFlange;

};
}

#endif
