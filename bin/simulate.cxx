// C++ includes
#include <iostream>
#include <string>

// Majorana includes
#include "Configuration.h"
#include "DetectorConstruction.h"

// Geant4 includes
#include "G4RunManager.hh"
#include "G4UImanager.hh"

// Prototypes
void HandleG4(const majorana::Configuration& config);

int main(int argc, char **argv)
{
  // HAndle runtime args
  if (argc < 2) 
  {
    std::cout << "Usage: ./simulate PATH_TO_CONFIG\n";
    std::exit(1);
  }

  // Initialize configuration
  majorana::Configuration config;
  config.Initialize(std::string(argv[1]));
  // Handle G4
  HandleG4(config);

  return 0;
}

void HandleG4(const majorana::Configuration& config)
{
  // Step 1) Detector construction
//  DetectorConstruction* detectorConstruction = new DetectorConstruction();

  // Step 2) Physics list construction

  // Step 3 Action initialization 

/*  // Run manager initialization
  G4RunManager* runManager = new G4RunManager;
  runManager->SetUserInitialization(new ExG4DetectorConstruction01);
  runManager->SetUserInitialization(new ExG4PhysicsList00);
  runManager->SetUserInitialization(new ExG4ActionInitialization01);
  runManager->Initialize();

  // Loop over events
  unsigned nEvents = config.NEvents();
  runManager->BeamOn(nEvents);

  delete detectorConstruction;
  delete runManager;*/
}
