// 
// File: G4Helper.h
//
// Author: Hunter Sullivan
//

#ifndef G4HELPER_H
#define G4HELPER_H

#include "DetectorConstruction.h"

// Geant4 includes
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"

namespace geo
{

class G4Helper 
{
  public:
    G4Helper(const std::string& gdmlFilePath);
    ~G4Helper();

    void ConstructDetector();
    void WriteGDML();
    DetectorConstruction* GetDetectorConstruction() const { return fDetector; };

  private:

    void HandleVisualization();
    void UselessInfo();
    void HandleVerbosities();

    G4RunManager*           fRunManager;
    G4UImanager*            fUIManager;
    DetectorConstruction*   fDetector;
    std::string             fGDMLOutputPath;
};
}

#endif
