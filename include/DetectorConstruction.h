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

namespace majorana
{
class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction(const Configuration& config);
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
   
    const DiskVolume* DiskGeometry() const { return m_diskVolume; };

  private:
    void InitializeMaterials();
    void InitializeDetector();

    G4VSolid*          m_worldSolid;       
    G4LogicalVolume*   m_worldLV; 
    G4VPhysicalVolume* m_worldPV;
    DiskVolume*        m_diskVolume;
    MaterialManager*   m_materialManager; 
};
}

#endif
