// 
// File: CryostatBody.h
//
// Author: Hunter Sullivan
//

#ifndef CRYOSTATBODY_H
#define CRYOSTATBODY_H

#include "G4LogicalVolume.hh"
#include "G4UnionSolid.hh"

#include "Geometry/Detector/Detector.h"

namespace geo
{

class CryostatBody 
{
  public:
    CryostatBody();
    ~CryostatBody();

    void ConstructVolume(Detector* detector);

    G4LogicalVolume* GetLV() { return fVolCryoOuterWall; };
    G4double         GetOuterWallDepth() const { return fCryoOuterWallDepth; };

  private: 
    void ConstructSubVolumes();
    void PlaceSubVolumes(Detector* detector);
    G4UnionSolid* GetShape(const G4String& name, 
                           const G4double& cryoR,
                           const G4double& cryoD);

    G4LogicalVolume* fVolCryoInnerBath;
    G4LogicalVolume* fVolCryoInnerWall;
    G4LogicalVolume* fVolCryoOuterBath;
    G4LogicalVolume* fVolCryoOuterWall;
    G4LogicalVolume* fVolCryoLeg;
   
    G4double fCryoInnerBathDepth;
    G4double fCryoInnerWallDepth;
    G4double fCryoOuterBathDepth;
    G4double fCryoOuterWallDepth;
    G4double fVolCryoOuterWallR;

};
}

#endif
