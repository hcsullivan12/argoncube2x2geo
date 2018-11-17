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
void HandleRuntimeArgs(majorana::Configuration& config, int argc, char **argv);
void InitializeG4(const majorana::Configuration& config);

int main(int argc, char **argv)
{
  // Create our configuration object
  majorana::Configuration config;
  // Handle runtime arguments
  HandleRuntimeArgs(config, argc, argv);
  // Initialize the rest of the configuration
  config.Initialize();
  // Handle G4
  HandleG4(config);

  return 0;
}

void HandleG4(const Configurtion& config)
{
  // Step 1) Detector construction
  DetectorConstruction* detectorConstruction = new DetectorConstruction();

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

void HandleRuntimeArgs(majorana::Configuration& config, int argc, char **argv)
{
  if (argc < 5) 
  {
    std::cout << "Usage: ./simulate --configPath PATH_TO_CONFIG --mode MODE_TYPE\n";
    std::cout << "MODE_TYPE:\n";
    std::cout << "  input  (Path to text file containing two columns of \"r thetaDeg\" source positions)\n";
    std::cout << "  random (Randomize source positions)\n";

    std::cout << "\nIf MODE_TYPE = random, must also specify --nEvents N_EVENTS";
    std::cout << "\nIf MODE_TYPE = input, must also specify --inputPath PATH_TO_INPUT\n";
    std::exit(1);
  }
  
  std::string mode;
  std::string inputPath;
  unsigned nEvents(0);
  for (int i = 1; i < argc; i++)
  {
    // Config path
    if (std::string(argv[i]) == "--configPath") 
    {
      if ( (i+1) < argc) config.SetConfigPath(std::string(argv[i+1]));
      else { std::cerr << "--configPath option requires one argument." << std::endl; exit(1); }
    }
    // Mode
    if (std::string(argv[i]) == "--mode") 
    {
      if ( (i+1) < argc) mode = std::string(argv[i+1]);
      else { std::cerr << "--mode option requires one argument." << std::endl; exit(1); }
    }
    // Optional nEvents
    if (std::string(argv[i]) == "--nEvents") 
    {
      if ( (i+1) < argc) nEvents = std::stoi(std::string(argv[i+1]));
      else { std::cerr << "--nEvents option requires one argument." << std::endl; exit(1); }
    }
    // Input path
    if (std::string(argv[i]) == "--inputPath") 
    {
      if ( (i+1) < argc) inputPath = std::string(argv[i+1]);
      else { std::cerr << "--inputPath option requires one argument." << std::endl; exit(1); }
    }
  }
 
  // Check mode
  if (mode == "random" && nEvents == 0)    { std::cerr << "random mode requires N_EVENTS > 0." << std::endl; exit(1); }
  if (mode == "input"  && inputPath == "") { std::cerr << "input mode requires --inputPath." << std::endl; exit(1); }
  if      (mode == "random") config.SetMode("random", nEvents);
  else if (mode == "input")  { config.SetMode("input", 0); config.SetInputPath(inputPath); }
  else { std::cout << "MODE_TYPE must be \"input\" or \"random\"." << std::endl; exit(1); }
}
