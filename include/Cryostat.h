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
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Box.hh"
#include "G4UnionSolid.hh"

namespace geo
{

class Cryostat 
{
  public:
    Cryostat();
    ~Cryostat();

    void ConstructVolume(G4VPhysicalVolume* worldPV,
                         G4LogicalVolume*   worldLV);

  private: 
    void ConstructCryostat(G4LogicalVolume* worldLV);
    void HandleSurfaces(G4VPhysicalVolume* worldPV);
    void HandleVisAttributes();

    G4UnionSolid*    fCryostatODSolid;
    G4LogicalVolume* fCryostatODLV;
};
}

#endif
