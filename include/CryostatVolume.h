// 
// File: CryostatVolume.h
//
// Author: Hunter Sullivan
//
// Discription: Class to construct cryostat volume.
//

#ifndef CRYOSTATVOLUME_H
#define CRYOSTATVOLUME_H

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"

namespace geo
{

class CryostatVolume 
{
  public:
    CryostatVolume();
    ~CryostatVolume();

    void ConstructVolume(G4VPhysicalVolume* worldPV,
                         G4LogicalVolume*   worldLV);

  private: 
    void ConstructCryostat(G4LogicalVolume* worldLV);
    void HandleSurfaces(G4VPhysicalVolume* worldPV);
    void HandleVisAttributes();

    G4SubtractionSolid* fCryostatSolid;
    G4LogicalVolume*    fCryostatLV;
};
}

#endif
