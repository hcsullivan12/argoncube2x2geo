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

    G4LogicalVolume* GetLV() { return fVolCryoFlange; };

  private: 
    void ConstructSubVolumes();
    void PlaceSubVolumes();

    G4LogicalVolume* fVolCryoFlange;

};
}

#endif
