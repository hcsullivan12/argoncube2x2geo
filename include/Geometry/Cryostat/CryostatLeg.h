// 
// File: CryostatLeg.h
//
// Author: Hunter Sullivan
//

#ifndef CRYOSTATLEG_H
#define CRYOSTATLEG_H

#include "Geometry/Module/Module.h"

#include "G4LogicalVolume.hh"

namespace geo
{

class CryostatLeg 
{
  public:
    CryostatLeg();
    ~CryostatLeg();

    void ConstructVolume();

    G4LogicalVolume* GetLV() { return fVolCryoLeg; };
    G4double         GetHeight() const { return fLegHeight; };
    G4double         GetShinR()  const { return fShinR; }

  private: 
    void ConstructSubVolumes();

    G4LogicalVolume* fVolCryoLeg; 

    G4double fLegHeight;
    G4double fShinR;

};
}

#endif
