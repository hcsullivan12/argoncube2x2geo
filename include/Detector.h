// 
// File: Detector.h
//
// Author: Hunter Sullivan
//
// Discription: Class to construct module volumes.
//

#ifndef DETECTOR_H
#define DETECTOR_H

#include "Module.h"

#include "G4LogicalVolume.hh"

namespace geo
{

class Detector 
{
  public:
    Detector();
    ~Detector();

    void ConstructVolume(G4LogicalVolume* volWorld, 
                         Module*          module);

    G4LogicalVolume* GetLV() { return fVolModuleContainer; };

  private: 
    void ConstructModules();

    // solids
    G4LogicalVolume* fVolModuleContainer;

};
}

#endif
