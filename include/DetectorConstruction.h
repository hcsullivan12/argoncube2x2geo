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
#include "DiskVolume.h"

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4Box.hh"

//class G4VPhysicalVolume;
//class G4LogicalVolume;

namespace majorana
{
class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction(const Configuration& config);
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

  private:
    void InitializeMaterials();
    void InitializeDetector();

    G4VSolid*          m_worldSolid;       
    G4LogicalVolume*   m_worldLogicalVol; 
    G4VPhysicalVolume* m_worldPhysicalVol;
    DiskVolume*        m_diskVolume;
    MaterialManager*   m_materialManager; 
};
}

#endif
