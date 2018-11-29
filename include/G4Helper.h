// 
// File: G4Helper.h
//
// Author: Hunter Sullivan
//
// Discription: Class to interface with G4.
//

#ifndef G4HELPER_H
#define G4HELPER_H

#include "Configuration.h"
#include "DetectorConstruction.h"
#include "PrimaryGeneratorAction.h"
#include "ActionInitialization.h"
#include "PhysicsList.h"

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
    G4Helper(const Configuration& config);
    ~G4Helper();

    using SourcePositions = std::vector<std::vector<float>>;

    void StartG4();

  private:
    void ConstructDetector();
    void InitializePhysics();
    void SetPhysicsList();
    void HandleVisualization();
    void ReadSteeringFile();
    void RunG4();
    void HandleVerbosities();

    G4RunManager*           m_runManager;
    G4UImanager*            m_uiManager;
    G4VisExecutive*         m_visManager;
    ActionInitialization*   m_actionInitialization;
    PrimaryGeneratorAction* m_generatorAction;
    DetectorConstruction*   m_detector;
    PhysicsList*            m_physicsList;
    SourcePositions         m_sourcePositions;
    unsigned                m_sourceMode;
    std::string             m_steeringFilePath;
    bool                    m_showVis;
    std::string             m_visMacroPath;
};
}

#endif
