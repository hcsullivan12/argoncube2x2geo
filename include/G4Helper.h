// 
// File: G4Helper.h
//
// Author: Hunter Sullivan
//
// Discription: Class to interface with G4.
//

#ifndef G4HELPER_H
#define G4HELPER_H

#include "DetectorConstruction.h"

#include "TMath.h"

// Geant4 includes
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

namespace geo
{

class G4Helper 
{
  public:
    G4Helper();
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
    G4VisExecutive*         fVisManager;
    DetectorConstruction*   fDetector;
    bool                    fShowVis;
    std::string             fVisMacroPath;
    std::string             fGDMLOutputPath;
};
}

#endif
