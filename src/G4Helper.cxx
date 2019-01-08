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
  m_showVis            = config->ShowVis();
  m_visMacroPath       = config->VisMacroPath();
  m_simulateOutputPath = config->SimulateOutputPath();

  // Initialize managers
  m_runManager = new G4RunManager;
  m_uiManager  = G4UImanager::GetUIpointer();
  // Initialize configuration
  m_sourcePositions.clear();
  m_sourceMode = config->SourceMode();
  
  // In source mode == 0, we will randomly pick a position in the detector
  // Otherwise, read from steering file
  if (m_sourceMode == 0)
  {
    float r        = config->DiskRadius()*G4UniformRand();
    float thetaDeg = twopi*G4UniformRand();

    std::vector<float> pos = {r, thetaDeg};
    m_sourcePositions.push_back(pos);
  }
  else if (m_sourceMode == 1)
  {
    m_steeringFilePath = config->SteeringFilePath();
    ReadSteeringFile();
  }
  else 
  {
    G4cerr << "PrimaryGeneratorAction::PrimaryGeneratorAction() Error! Source mode invalid!\n";
    exit(1);
  }

  // Construct detector
  m_detector = new DetectorConstruction();
  // Initialize physics
  //InitializePhysics();

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
  
  std::string rString, thetaString;
  while (std::getline(f, rString, ' '))
  {
    std::getline(f, thetaString);
    float r        = std::stof(rString);
    float thetaDeg = std::stof(thetaString);

    std::vector<G4float> pos = {r, thetaDeg};
    m_sourcePositions.push_back(pos);
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
  unsigned nEvents = m_sourcePositions.size();
  // Initialize photon table
  // This will help reduce overhead
  OpDetPhotonTable* photonTable = OpDetPhotonTable::Instance();
  // Initialize our anaylzer
  Analyzer analyzer(m_simulateOutputPath);
 
  std::cout << "\nPress enter to start running G4...\n";
  std::cin.get();
  for (unsigned e = 0; e < nEvents; e++)
  {
    G4cout << "****  EVENT #" << e << "  ****" << G4endl;
    // Reset the generator
    G4float r        = m_sourcePositions[e][0];
    G4float thetaDeg = m_sourcePositions[e][1]; 
    m_generatorAction->Reset(r, thetaDeg, m_detector->WheelGeometry()->Thickness());
   
    // Start run!
    //photonTable->Print();
    m_runManager->BeamOn(1);
    // Fill our tree
    analyzer.Fill(e);
    // Clear the photon table!
    //photonTable->Print();
    photonTable->Reset();
    //photonTable->Print();
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
