// 
// File: Cryostat.h
//
// Author: Hunter Sullivan
//

#ifndef CRYOSTAT_H
#define CRYOSTAT_H

#include "G4LogicalVolume.hh"

#include "Geometry/Detector/Detector.h"
#include "Geometry/Module/ModuleFlange.h"
#include "Geometry/Cryostat/CryostatBody.h"
#include "Geometry/Cryostat/CryostatLeg.h"
#include "Geometry/Cryostat/CryostatFT.h"
#include "Geometry/Auxiliary/Feedthrough.h"

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
    void ConstructSubVolumes(Detector* detector);
    void PlaceSubVolumes(G4LogicalVolume* volWorld);
    
    CryostatLeg*  fCryostatLeg;
    CryostatBody* fCryostatBody;
    CryostatFT*   fCryostatFT;

    G4LogicalVolume* fVolCryotatContainer;
    G4LogicalVolume* fVolCryostatFlangeWrap;
    G4LogicalVolume* fVolCryostatFlange;
    G4LogicalVolume* fVolCryoMedFt;
    G4LogicalVolume* fVolCryoLgFt;

    G4double fCryoFTHeight;

};
}

#endif
