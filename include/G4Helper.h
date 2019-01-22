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
#include "PrimaryGeneratorAction.h"
#include "ActionInitialization.h"
#include "PhysicsList.h"

#include "TMath.h"

// Geant4 includes
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4StateManager.hh"
#include "G4HadronicProcessStore.hh"
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif


namespace majorana
{

class G4Helper 
{
  public:
    static G4Helper* Instance();
    ~G4Helper();

    using SteeringTable = std::vector<std::vector<float>>;

    void StartG4();
    void WriteGDML();
    ActionInitialization* GetActionInitialization() const { return m_actionInitialization; };
    DetectorConstruction* GetDetectorConstruction() const { return m_detector; };

  private:
    G4Helper();
    static G4Helper* instance;

    void HandleVisualization();
    void ReadSteeringFile();
    void RunG4();
    void HandleVerbosities();
    inline void ConvertToPolar(G4float& r, G4float& thetaDeg)
    {
      float x(r), y(thetaDeg);
      std::cout << x << " " << y << std::endl;
      r        = std::sqrt(x*x + y*y);
      thetaDeg = TMath::ASin(std::abs(y/r))*180/pi;
      std::cout << thetaDeg << std::endl;
 
      // Handle theta convention
      if (x < 0 && y > 0) thetaDeg = 180 - thetaDeg;
      if (x < 0 && y < 0) thetaDeg = 180 + thetaDeg;
      if (x > 0 && y < 0) thetaDeg = 360 - thetaDeg;
    }

    G4RunManager*           m_runManager;
    G4UImanager*            m_uiManager;
    G4VisExecutive*         m_visManager;
    ActionInitialization*   m_actionInitialization;
    PrimaryGeneratorAction* m_generatorAction;
    DetectorConstruction*   m_detector;
    PhysicsList*            m_physicsList;
    SteeringTable           m_steeringTable;
    std::string             m_steeringFilePath;
    bool                    m_showVis;
    std::string             m_visMacroPath;
    std::string             m_simulateOutputPath;
    std::string             m_recoAnaTreePath;
    std::string             m_gdmlOutputPath;
};
}

#endif
