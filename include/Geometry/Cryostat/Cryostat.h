// 
// File: Cryostat.h
//
// Author: Hunter Sullivan
//

#ifndef CRYOSTAT_H
#define CRYOSTAT_H

#include "G4LogicalVolume.hh"

#include "Geometry/Detector/Detector.h"
#include "Geometry/Cryostat/CryostatBody.h"
#include "Geometry/Cryostat/CryostatLeg.h"
#include "Geometry/Cryostat/CryostatFT.h"
#include "Geometry/Auxiliary/Feedthrough.h"
#include "Geometry/Auxiliary/ModuleFastener.h"

namespace geo
{

class Cryostat 
{
  public:
    Cryostat();
    ~Cryostat();

    void ConstructVolume(G4LogicalVolume* volDetEnclosure, 
                         Detector*        module);

  private: 
    void ConstructSubVolumes(Detector* detector);
    void PlaceSubVolumes(G4LogicalVolume* volDetEnclosure);
    
    CryostatLeg*  fCryostatLeg;
    CryostatBody* fCryostatBody;
    CryostatFT*   fCryostatFT;

    G4LogicalVolume* fVolCryostat;
    G4LogicalVolume* fVolCryostatFlangeWrap;
    G4LogicalVolume* fVolCryostatFlange;
    G4LogicalVolume* fVolCryoMedFt;
    G4LogicalVolume* fVolCryoLgFt;
    G4LogicalVolume* fVolModFastener;

    G4double fCryoFTHeight;
    G4double fCryostatFlangeThickness;

};
}

#endif
