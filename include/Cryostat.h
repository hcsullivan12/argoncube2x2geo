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
#include "G4UnionSolid.hh"

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
    void ConstructCryostat(Detector* detector);
    G4UnionSolid* GetCryostatStructure(const std::vector<G4double>& dim, 
                                       const G4String&              name);

    G4LogicalVolume* fVolCryoInnerBath;
    G4LogicalVolume* fVolCryoInnerWall;
    G4LogicalVolume* fVolCryoOuterBath;
    G4LogicalVolume* fVolCryoOuterWall;

};
}

#endif
