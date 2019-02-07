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
#include "Geometry/Cryostat/CryostatFlange.h"

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
    
    CryostatBody* fCryostatBody;
    CryostatLeg*  fCryostatLeg;
    //CryostatFlange* fCryostatFlange;
    ModuleFlange* fModuleFlange;

    G4LogicalVolume* fVolCryotatContainer;
    G4LogicalVolume* fVolCryotatTop;
    G4LogicalVolume* fVolCryostatFlange;

};
}

#endif
