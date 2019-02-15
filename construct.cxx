// C++ includes
#include <iostream>
#include <string>

// Geo includes
#include "QuantityStore.h"
#include "G4Helper.h"

// Prototypes
void HandleArgs(int argc, char **argv);

int main(int argc, char **argv)
{
  // Handle runtime args
  HandleArgs(argc, argv); 

  // Initialize configuration
  geo::QuantityStore* qStore = geo::QuantityStore::Instance();
  qStore->Initialize();
  
  // Start detector construction
  std::string filepath = std::string(argv[1]);
  geo::G4Helper g4Helper(filepath);
  g4Helper.ConstructDetector();

  return 0;
}

void HandleArgs(int argc, char **argv)
{
  if (argc != 2) 
  {
    std::cout << "\nUsage: ./construct GDML_OUTPUT_FILEPATH\n";
    std::cout << std::endl;
    std::exit(1);
  }
}


