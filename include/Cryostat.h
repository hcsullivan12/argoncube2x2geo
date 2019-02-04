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
    //void ConstructCryostat(Detector* detector);
    void ConstructTubs();
    void ConstructCaps();
    void ConstructFlange();
    void PlaceVolumes();
    
    G4LogicalVolume* fVolCryoInnerBathTub;
    G4LogicalVolume* fVolCryoInnerWallTub;
    G4LogicalVolume* fVolCryoOuterBathTub;
    G4LogicalVolume* fVolCryoOuterWallTub;
    G4LogicalVolume* fVolCryoInnerBathCap;


};
}

#endif
