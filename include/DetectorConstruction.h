// 
// File: DetectorConstruction.h
//
// Author: Hunter Sullivan
//

#ifndef DETECTORCONSTRUCTION_H
#define DETECTORCONSTRUCTION_H

#include "Geometry/Module/Module.h"
#include "Geometry/Cryostat/Cryostat.h"
#include "Geometry/Detector/Detector.h"

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

namespace geo
{
class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void               ConstructSDandField();
    
  private:
    void ConstructMaterials();
    void ConstructDetector();
 
    G4LogicalVolume*   fVolWorld;
    G4VPhysicalVolume* fPVWorld; 
    G4LogicalVolume*   fVolDetEnclosure;

    Module*            fModule;
    Detector*          fDetector;
    Cryostat*          fCryostat;
};
}

#endif
