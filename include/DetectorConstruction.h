// 
// File: DetectorConstruction.h
//
// Author: Hunter Sullivan
//
// Discription: Class to construct our detector.
//

#ifndef DETECTORCONSTRUCTION_H
#define DETECTORCONSTRUCTION_H

#include "Configuration.h"
#include "MaterialManager.h"
#include "WheelVolume.h"
#include "MPPCSD.h"

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Cache.hh"

namespace majorana
{
class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void               ConstructSDandField();
   
    const WheelVolume* WheelGeometry() const { return m_wheelVolume; };

  private:
    void InitializeMaterials();
    void InitializeDetector();

    G4VSolid*          m_worldSolid;       
    G4LogicalVolume*   m_worldLV; 
    G4VPhysicalVolume* m_worldPV;
    WheelVolume*       m_wheelVolume;
    MaterialManager*   m_materialManager;
    G4Cache<MPPCSD*>   m_mppcSD;    

    unsigned           m_nMPPCs;
    float              m_mppcArea;
};
}

#endif
