// C++ includes
#include <iostream>
#include <string>

// Majorana includes
#include "Configuration.h"
#include "G4Helper.h"

// Visualization 
static bool showVis = false;

// Prototypes
void HandleArgs(int argc, char **argv);

int main(int argc, char **argv)
{
  // Handle runtime args
  HandleArgs(argc, argv); 
  // Initialize configuration
  majorana::Configuration config;
  config.Initialize(std::string(argv[1]));
  // Pass visualization
  config.SetVisualization(showVis);
  // Handle G4
  majorana::G4Helper g4Helper(config);
  g4Helper.StartG4();
  g4Helper.Clean();

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


