// C++ includes
#include <iostream>
#include <string>

// Geo includes
#include "Configuration.h"
#include "G4Helper.h"

// ROOT includes
#include "TFile.h"

// Visualization 
static bool showVis = false;

// Prototypes
void HandleArgs(int argc, char **argv);
void InitializeFiles(const geo::Configuration*);

int main(int argc, char **argv)
{
  // Handle runtime args
  HandleArgs(argc, argv); 

  // Initialize configuration
  geo::Configuration* config = geo::Configuration::Instance();
  // Pass visualization
  config->SetVisualization(showVis);
  config->Initialize(std::string(argv[1]));

  // Initialize output files
  InitializeFiles(config);
  
  // Start detector construction
  geo::G4Helper g4Helper;
  g4Helper.ConstructDetector();

  return 0;
}

void HandleArgs(int argc, char **argv)
{
  if (argc < 2 || argc > 4 || argc == 3) 
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

void InitializeFiles(const geo::Configuration* config)
{
}


