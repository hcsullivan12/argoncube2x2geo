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

  private: 
    void ConstructSubVolumes();

    G4LogicalVolume* fVolCryoLeg; 

};
}

#endif
