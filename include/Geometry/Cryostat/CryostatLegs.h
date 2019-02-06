// 
// File: CryostatLegs.h
//
// Author: Hunter Sullivan
//

#ifndef CRYOSTATLEGS_H
#define CRYOSTATLEGS_H

#include "Geometry/Module/Module.h"

#include "G4LogicalVolume.hh"

namespace geo
{

class CryostatLegs 
{
  public:
    CryostatLegs();
    ~CryostatLegs();

    void ConstructVolume();

    G4LogicalVolume* GetLV() { return fVolCryoLegContainer; };

  private: 
    void ConstructSubVolumes();

    G4LogicalVolume* fVolCryoLeg;
    G4LogicalVolume* fVolCryoLegContainer;

};
}

#endif
