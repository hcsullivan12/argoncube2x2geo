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
    void ConstructBody();
    void ConstructLegs();
    void ConstructFlange();
    void PlaceVolumes(G4LogicalVolume* volWorld, 
                      Detector*          module);
    G4UnionSolid* CryostatShape(const G4String& name, 
                                const G4double& cryoR,
                                const G4double& cryoD);
    
    G4LogicalVolume* fVolCryoInnerBath;
    G4LogicalVolume* fVolCryoInnerWall;
    G4LogicalVolume* fVolCryoOuterBath;
    G4LogicalVolume* fVolCryoOuterWall;
    G4LogicalVolume* fVolCryoLeg;
    G4LogicalVolume* fVolCryoLegContainer;
   
    G4double fCryoInnerBathDepth;
    G4double fCryoInnerWallDepth;
    G4double fCryoOuterBathDepth;
    G4double fCryoOuterWallDepth;
    G4double fVolCryoOuterWallR;

};
}

#endif
