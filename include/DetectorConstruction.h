// 
// File: DetectorConstruction.h
//
// Author: Hunter Sullivan
//
// Discription: Class to construct our detector.
//

#ifndef DETECTORCONSTRUCTION_H
#define DETECTORCONSTRUCTION_H

#include "WheelVolume.h"

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4Cache.hh"

namespace geo
{
class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void               ConstructSDandField();
   
    const WheelVolume* WheelGeometry() const { return fWheelVolume; };

  private:
    void InitializeMaterials();
    void InitializeDetector();

    G4VSolid*          fWorldSolid; 
    G4LogicalVolume*   fWorldLV; 
    G4VPhysicalVolume* fWorldPV;
    WheelVolume*       fWheelVolume;

    unsigned           fNMPPCs;
    float              fMPPCArea;
};
}

#endif
