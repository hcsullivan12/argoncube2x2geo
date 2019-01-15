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
  majorana::Configuration* config = majorana::Configuration::Instance();
  // Pass visualization
  config->SetVisualization(showVis);
  config->Initialize(std::string(argv[1]));
  // Initialize source configuration
  if (config->SourceMode() == "voxel")
  {
    // Initialize voxels so we can make the reference table
    VoxelTable* voxelTable = VoxelTable::Instance();
    voxelTable->Initialize(config->VoxelizationPath());
  }
  // If we're wanting to reconstruct in point mode, it's 
  // easier if we still initialize voxels to load reference table
  // in voxel table 
  if (config->SourceMode() == "point" && config->Reconstruct())
  {
    VoxelTable* voxelTable = VoxelTable::Instance();
    voxelTable->Initialize(config->VoxelizationPath());
    voxelTable->LoadReferenceTable(config->ReferenceTablePath());
  }
  // Start G4
  majorana::G4Helper* g4Helper = majorana::G4Helper::Instance();
  g4Helper->StartG4();

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


