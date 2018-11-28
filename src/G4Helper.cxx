// 
// File: G4Helper.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to interface with G4
//

#include "G4Helper.h"

#include "G4StateManager.hh"
#include "G4HadronicProcessStore.hh"

namespace majorana
{

G4Helper::G4Helper(const Configuration& config) 
 : m_runManager(NULL),
   m_visManager(NULL),
   m_detector(NULL),
   m_generatorAction(NULL),
   m_actionInitialization(NULL),
   m_showVis(config.ShowVis()),
   m_visMacroPath(config.VisMacroPath())
{
  // Initialize managers
  m_runManager = new G4RunManager;
  m_uiManager  = G4UImanager::GetUIpointer();
  // Initialize configuration
  m_sourcePositions.clear();
  m_sourceMode  = config.SourceMode();
  
  // In source mode == 0, we will randomly pick a position in the detector
  // Otherwise, read from steering file
  if (m_sourceMode == 0)
  {
    float r        = config.DiskRadius()*G4UniformRand();
    float thetaDeg = twopi*G4UniformRand();

    std::vector<float> pos = {r, thetaDeg};
    m_sourcePositions.push_back(pos);
  }
  else if (m_sourceMode == 1)
  {
    m_steeringFilePath = config.SteeringFilePath();
    ReadSteeringFile();
  }
  else 
  {
    G4cerr << "PrimaryGeneratorAction::PrimaryGeneratorAction() Error! Source mode invalid!\n";
    exit(1);
  }

  // Construct detector
  m_detector = new DetectorConstruction(config);
  // Initialize physics
  InitializePhysics();

  // Update the run manager
  m_runManager->SetUserInitialization(m_detector);
  m_runManager->SetUserInitialization(new FTFP_BERT(0));
  // Initialize action initialization 
  m_actionInitialization = new ActionInitialization(config);
  // Get the pointer to the generator action
  m_generatorAction = m_actionInitialization->GeneratorAction();
  m_runManager->SetUserInitialization(m_actionInitialization);
  m_runManager->Initialize();
}

G4Helper::~G4Helper()
{}

void G4Helper::Clean()
{
  //if (m_uiManager)  delete m_uiManager;
  #ifdef G4VIS_USE
  if (m_visManager) delete m_visManager;
  #endif

  /*G4StateManager* pStateManager = G4StateManager::GetStateManager();
  std::cout << pStateManager->GetPreviousState() << std::endl;
  std::cout << pStateManager->GetCurrentState()  << std::endl;*/

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
  // Get the pointer to the UI manager and set verbosities
  m_uiManager->ApplyCommand("/run/verbose 2");
  m_uiManager->ApplyCommand("/event/verbose 5");
  m_uiManager->ApplyCommand("/tracking/verbose 5");
  G4HadronicProcessStore* g = G4HadronicProcessStore::Instance();
  g->SetVerbose(0);
 
  // Start main G4 loop
  RunG4();

  // Handle visualization
  HandleVisualization();
}

void G4Helper::RunG4()
{
  std::cout << "Running G4!!\n";
  // Loop over the events or positions
  unsigned nEvents = m_sourcePositions.size();
  for (unsigned e = 0; e < nEvents; e++)
  {
    // Reset the generator
    G4float r        = m_sourcePositions[e][0];
    G4float thetaDeg = m_sourcePositions[e][1]; 
    m_generatorAction->Reset(r, thetaDeg, m_detector->DiskGeometry()->Thickness());

    // Start run!
    m_runManager->BeamOn(1);
  }
}

void G4Helper::HandleVisualization()
{
  #ifdef G4VIS_USE
  if (m_showVis) 
  {
    m_visManager = new G4VisExecutive();
    m_visManager->Initialize();
    std::string command = "/control/execute " + m_visMacroPath;
    m_uiManager->ApplyCommand(command);
  }
  #endif

  std::cout << "Press enter to quit.\n";
  std::cin.get();
}

void G4Helper::ConstructDetector()
{
}

void G4Helper::InitializePhysics()
{
}

void G4Helper::SetPhysicsList()
{
}

}
