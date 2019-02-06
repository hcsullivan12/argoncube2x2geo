// 
// File: Detector.h
//
// Author: Hunter Sullivan
//

#ifndef DETECTOR_H
#define DETECTOR_H

#include "Geometry/Module/Module.h"

#include "G4LogicalVolume.hh"

namespace geo
{

class Detector 
{
  public:
    Detector();
    ~Detector();

    void ConstructVolume();

    G4LogicalVolume* GetLV() { return fVolModuleContainer; };

  private: 
    void ConstructSubVolumes();
    void PlaceSubVolumes();

    Module*          fModule;
    G4LogicalVolume* fVolModuleContainer;

};
}

#endif
