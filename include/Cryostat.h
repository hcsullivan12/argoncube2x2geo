// 
// File: Cryostat.h
//
// Author: Hunter Sullivan
//
// Discription: Class to construct cryostat volume.
//

#ifndef CRYOSTAT_H
#define CRYOSTAT_H

#include "G4LogicalVolume.hh"
#include "Detector.h"

namespace geo
{

class Cryostat 
{
  public:
    Cryostat();
    ~Cryostat();

    void ConstructVolume(G4LogicalVolume* volWorld, 
                         Detector*          module);
  private: 
    void ConstructCryostat(G4LogicalVolume* worldLV);

    G4LogicalVolume* fCryostatODLV;
};
}

#endif
