// C++ includes
#include <iostream>
#include <string>

// Majorana includes
#include "Configuration.h"
#include "DetectorConstruction.h"
#include "ActionInitialization.h"

// Geant4 includes
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "FTFP_BERT.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

// Visualization 
static bool showVis = false;

// Prototypes
void InitializeG4(const majorana::Configuration& config);
void HandleArgs(int argc, char **argv);

int main(int argc, char **argv)
{
  // Handle runtime args
  HandleArgs(argc, argv); 
  // Initialize configuration
  majorana::Configuration config;
  config.Initialize(std::string(argv[1]));
  // Handle G4
  InitializeG4(config);

  return 0;
}

void HandleArgs(int argc, char **argv)
{
  if (argc < 2 || argc > 4) 
  {
    std::cout << "\nUsage: ./simulate PATH_TO_CONFIG <Options>\n";
    std::cout << "Options:\n"
              << "  --vis ON/OFF (If ON, render visualization. Default is OFF.)\n";
    std::cout << std::endl;
    std::exit(1);
  }
  if (argc == 4)
  {
    if (std::string(argv[2]) == "--vis" && std::string(argv[3]) == "ON") showVis = true;    
  }
}

void InitializeG4(const majorana::Configuration& config)
{
  // Run manager initialization
  G4RunManager* runManager = new G4RunManager;
  // Step 1) Detector construction
  runManager->SetUserInitialization(new majorana::DetectorConstruction(config));
  // Step 2) Physics list construction
  runManager->SetUserInitialization(new FTFP_BERT(0));
  // Step 3 Action initialization 
  runManager->SetUserInitialization(new majorana::ActionInitialization);
  
  runManager->Initialize();
  
  // get the pointer to the UI manager and set verbosities
  G4UImanager* UI = G4UImanager::GetUIpointer();
  UI->ApplyCommand("/run/verbose 0");
  UI->ApplyCommand("/event/verbose 0");
  UI->ApplyCommand("/tracking/verbose 0");

  #ifdef G4VIS_USE
  G4VisManager* visManager = NULL;
  if (showVis) 
  {
    std::cout << "HEre\n";
    visManager = new G4VisExecutive();
    visManager->Initialize();
   	UI->ApplyCommand("/control/execute ../vis.mac");
  }
  #endif

  // Loop over events
  //unsigned nEvents = config.NEvents();
  runManager->BeamOn(1);

  std::cout << "Press enter to quit.\n";
  std::cin.get();
  
  #ifdef G4VIS_USE
  if (visManager) delete visManager;
  #endif
  if (runManager) delete runManager;
}

