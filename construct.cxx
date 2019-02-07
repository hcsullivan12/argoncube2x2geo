// C++ includes
#include <iostream>
#include <string>

// Geo includes
#include "Configuration.h"
#include "G4Helper.h"

// Prototypes
void HandleArgs(int argc, char **argv);
void InitializeFiles(const geo::Configuration*);

int main(int argc, char **argv)
{
  // Handle runtime args
  HandleArgs(argc, argv); 

  // Initialize configuration
  geo::Configuration* config = geo::Configuration::Instance();
  config->Initialize(std::string(argv[1]), std::string(argv[2]));
  
  // Start detector construction
  geo::G4Helper g4Helper;
  g4Helper.ConstructDetector();

  return 0;
}

void HandleArgs(int argc, char **argv)
{
  if (argc != 3) 
  {
    std::cout << "\nUsage: ./construct PATH_TO_CONFIG GDML_OUTPUT_FILEPATH\n";
    std::cout << std::endl;
    std::exit(1);
  }
}


