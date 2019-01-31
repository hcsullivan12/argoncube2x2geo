// 
// File: DetectorConstruction.h
//
// Author: Hunter Sullivan
//
// Discription: Class to construct our detector.
//

#ifndef DETECTORCONSTRUCTION_H
#define DETECTORCONSTRUCTION_H

#include "Module.h"
#include "Cryostat.h"
#include "Detector.h"

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
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
    Module*            fModule;
    Detector*          fDetector;
    Cryostat*          fCryostat;
};
}

#endif
