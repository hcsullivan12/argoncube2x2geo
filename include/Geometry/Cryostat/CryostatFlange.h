// 
// File: CryostatFlange.h
//
// Author: Hunter Sullivan
//

#ifndef CRYOSTATFLANGE_H
#define CRYOSTATFLANGE_H

#include "G4LogicalVolume.hh"

namespace geo
{

class CryostatFlange 
{
  public:
    CryostatFlange();
    ~CryostatFlange();

    void ConstructVolume();

    G4LogicalVolume* GetLV() { return fVolTopContainer; };

  private: 
    void ConstructSubVolumes();
    void PlaceSubVolumes();

    G4LogicalVolume* fVolTopContainer;
    G4LogicalVolume* fVolModuleTopWall;
    G4LogicalVolume* fVolModuleFTContainer;
    G4LogicalVolume* fVolModuleMedFT;
    G4LogicalVolume* fVolModuleFlange;
    G4LogicalVolume* fVolTopFlange;

};
}

#endif
