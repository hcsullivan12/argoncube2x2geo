// 
// File: G4Helper.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to interface with G4
//

#include "G4Helper.h"
#include "Analyzer.h"
#include "OpDetPhotonTable.h"
#include "VoxelTable.h"

namespace majorana
{

G4Helper* G4Helper::instance = 0;

G4Helper* G4Helper::Instance()
{
  if (instance == 0)
  {
    static G4Helper g4Helper;
    instance = &g4Helper;
  }
  return instance;
}

G4Helper::G4Helper() 
 : m_runManager(NULL),
   m_visManager(NULL),
   m_detector(NULL),
   m_generatorAction(NULL),
   m_actionInitialization(NULL),
   m_physicsList(NULL)
{
  // Get config
  Configuration* config = Configuration::Instance();
  if (!config)
  {
    G4cout << "Error! Configuration not initialized!" << G4endl;
    std::exit(1);
  }
  // Visualization and outputs
  m_showVis            = config->ShowVis();
  m_visMacroPath       = config->VisMacroPath();
  m_simulateOutputPath = config->SimulateOutputPath();

  // Initialize managers
  m_runManager = new G4RunManager;
  m_uiManager  = G4UImanager::GetUIpointer();

  // Load the steering file 
  m_steeringTable.clear();
  m_steeringFilePath = config->SteeringFilePath();
  ReadSteeringFile(); 
  
  // Construct detector
  m_detector = new DetectorConstruction();

  // Update the run manager
  m_runManager->SetUserInitialization(m_detector);

  // Initialize physics list
  m_physicsList = new PhysicsList;
  m_runManager->SetUserInitialization(m_physicsList);

  // Initialize action initialization 
  m_actionInitialization = new ActionInitialization();

  // Get the pointer to the generator action
  m_generatorAction = m_actionInitialization->GetGeneratorAction();

  m_runManager->SetUserInitialization(m_actionInitialization);
  m_runManager->Initialize();
}

G4Helper::~G4Helper()
{
  //if (m_uiManager)  delete m_uiManager;
  #ifdef G4VIS_USE
  if (m_visManager) delete m_visManager;
  #endif 
  if (m_runManager) delete m_runManager;
}

void G4Helper::ReadSteeringFile()
{
  std::ifstream f(m_steeringFilePath.c_str()); 
  if (!f.is_open())
  {
    G4cerr << "PrimaryGeneratorAction::ReadSteeringFile() Error! Cannot open steering file!\n";
    exit(1);
  }
  G4cout << "\nReading light steering file..." << G4endl;
  
  // We have different modes here:
  //
  //   SteeringFile in voxel mode
  //        voxelID n
  //   SteeringFile in point mode
  //        r theta n or x y n
  Configuration* config = Configuration::Instance();
  if (config->SourceMode() == "voxel")
  {
    // First read top line 
    std::string string1, string2;
    std::getline(f, string1, ' ');
    std::getline(f, string2);
    if (string1 != "voxelID" || string2 != "n")
    {
      G4cout << "Error! LightSteeringFile in voxel mode must have " 
             << "\'voxelID n\' on the top row.\n"
             << G4endl;
      exit(1);
    }
    // Read the rest of the file
    while (std::getline(f, string1, ' '))
    {
      std::getline(f, string2);
      float voxelID = std::stof(string1);
      float n       = std::stof(string2);
      std::vector<float> vec = {voxelID, n};
      m_steeringTable.push_back(vec);
    }
  }
  else 
  {
    // First read top line for r,theta mode or x,y mode
    std::string string1, string2, string3;
    std::getline(f, string1, ' ');
    std::getline(f, string2, ' ');
    std::getline(f, string3);
  
    bool sourcePosXY;
    if (string1 == "r" && string2 == "theta")  sourcePosXY = false;
    else if (string1 == "x" && string2 == "y") sourcePosXY = true;
    else 
    {
      G4cout << "Error! LightSteeringFile in point mode must have "
             << "\"r theta n\" or \"x y n\" on top row.\n" 
             << G4endl;
      exit(1);
    }
    if (string3 != "n")
    {
      G4cout << "Error! LightSteeringFile in point mode must have "
             << "\"r theta n\" or \"x y n\" on top row.\n" 
             << G4endl;
      exit(1);

    }
   
    // Read the rest of the file
    while (std::getline(f, string1, ' '))
    {
      std::getline(f, string2, ' ');
      std::getline(f, string3);
      float value1     = std::stof(string1);
      float value2     = std::stof(string2);
      float n          = std::stof(string3);

      // We have primaries and some coordinates
      if (sourcePosXY)
      {
        float x(value1), y(value2);
        float r        = std::sqrt(x*x + y*y);
        float thetaDeg(0);
        if (r > 0.01) thetaDeg = TMath::ASin(std::abs(y/r))*180/pi;
        // Handle theta convention
        if (x <  0 && y >= 0) thetaDeg = 180 - thetaDeg;
        if (x <  0 && y <  0) thetaDeg = 180 + thetaDeg;
        if (x >= 0 && y <  0) thetaDeg = 360 - thetaDeg;

        std::vector<G4float> sourceVec = {r, thetaDeg, x, y, n};
        m_steeringTable.push_back(sourceVec);
      }
      else 
      {
        float r(value1), thetaDeg(value2);
        float x = r*std::cos(thetaDeg*pi/180);
        float y = r*std::sin(thetaDeg*pi/180);
        std::vector<G4float> sourceVec = {r, thetaDeg, x, y, n};
        m_steeringTable.push_back(sourceVec);
      }
    }
  }
}

void G4Helper::StartG4()
{
  // Set verbosities
  HandleVerbosities();
     
  // Handle visualization
  HandleVisualization();
 
  // Start main G4 loop
  RunG4();
}

void G4Helper::HandleVerbosities()
{
  m_uiManager->ApplyCommand("/run/verbose 0");      // max = 2
  m_uiManager->ApplyCommand("/event/verbose 0");    // max = 2
  m_uiManager->ApplyCommand("/tracking/verbose 0"); // max = 4
  G4HadronicProcessStore::Instance()->SetVerbose(0);
  m_physicsList->GetOpticalPhysics()->GetBoundaryProcess()->SetVerboseLevel(0); // max 1
}

void G4Helper::RunG4()
{
  // Loop over the events or positions
  unsigned nEvents = m_steeringTable.size();
  // Initialize photon table
  // This will help reduce overhead
  OpDetPhotonTable* photonTable = OpDetPhotonTable::Instance();
  // Get config
  Configuration* config = Configuration::Instance();
  // Initialize our anaylzer
  Analyzer analyzer(m_simulateOutputPath);
 
  std::cout << "\nPress enter to start running G4...\n";
  std::cin.get();
  for (unsigned e = 0; e < nEvents; e++)
  {
    G4cout << "\n****  EVENT #" << e << "  ****" << G4endl;
    // Reset the generator
    G4float r(0), thetaDeg(0), x(0), y(0), z(0), voxelSize(0);
    G4int   n(0);
    if (config->SourceMode() == "voxel")
    {
      // Get the voxel table
      VoxelTable* voxelTable = VoxelTable::Instance();
      Voxel voxel = voxelTable->GetVoxel(m_steeringTable[e][0]);

      r         = voxel.R();
      thetaDeg  = voxel.Theta();
      x         = voxel.X();
      y         = voxel.Y();
      z         = m_detector->WheelGeometry()->Thickness();
      n         = m_steeringTable[e][1];
      voxelSize = voxel.Size(); 
      std::cout << "voxelID = " << voxel.ID() << std::endl;
    }
    else 
    {
      r         = m_steeringTable[e][0];
      thetaDeg  = m_steeringTable[e][1]; 
      x         = m_steeringTable[e][2];
      y         = m_steeringTable[e][3]; 
      z         = m_detector->WheelGeometry()->Thickness();
      n         = m_steeringTable[e][4];
    }
    m_generatorAction->Reset(r, thetaDeg, x, y, z, n, voxelSize);
  
    // Start run!
    m_runManager->BeamOn(1);
    // Fill our tree
    analyzer.Fill(e);
    // Reconstruct?
    if (Configuration::Instance()->Reconstruct())
    {
      if (!VoxelTable::Instance())
      {
        G4cerr << "Error! VoxelTable not initialized! Canceling reconstruction." << G4endl;
      }
      else 
      {
        //  Reconstructor reconstructor;
      }
    }
    // Clear the photon table!
    photonTable->Reset();
  }
  std::cout << "\nDone! Press enter to exit...\n";
  std::cin.get();
}

void G4Helper::HandleVisualization()
{
  #ifdef G4VIS_USE
  if (m_showVis) 
  {
    m_visManager = new G4VisExecutive();
    m_visManager->SetVerboseLevel(0); // max = 6
    m_visManager->Initialize();
    std::string command = "/control/execute " + m_visMacroPath;
    m_uiManager->ApplyCommand(command);
  }
  #endif
}

}
