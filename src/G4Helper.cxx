// 
// File: G4Helper.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to interface with G4
//

#include "G4Helper.h"
#include "Configuration.h"
#include "ActionInitialization.h"

#include "G4PhysListFactory.hh"
#include "G4HadronicProcessStore.hh"
#ifdef G4_GDML
#include "G4GDMLParser.hh"
#include "G4PhysicalVolumeStore.hh"
#endif

namespace geo
{

G4Helper::G4Helper() 
 : fRunManager(NULL),
   fVisManager(NULL),
   fDetector(NULL)
{
  // Get config
  Configuration* config = Configuration::Instance();
  if (!config)
  {
    G4cout << "Error! Configuration not initialized!" << G4endl;
    std::exit(1);
  }
  // Visualization and outputs
  fShowVis            = config->ShowVis();
  fVisMacroPath       = config->VisMacroPath();
  fGDMLOutputPath     = config->GDMLOutputPath();
}

G4Helper::~G4Helper()
{
  //if (fUIManager)  delete fUIManager;
  #ifdef G4VIS_USE
  if (fVisManager) delete fVisManager;
  #endif 
  if (fRunManager) delete fRunManager;
}

void G4Helper::ConstructDetector()
{
  // Initialize managers
  fRunManager = new G4RunManager;
  fUIManager  = G4UImanager::GetUIpointer();
  
  // Construct detector
  fDetector = new DetectorConstruction();
  UselessInfo();
  fRunManager->Initialize(); 
   
  // Handle visualization
  HandleVisualization();
 
  G4cout << "Detector construction finished!" << G4endl;
  G4cout << "\nDone! Press enter to exit..." << G4endl;
  std::cin.get();

  // Output GDML
  WriteGDML();
}

void G4Helper::UselessInfo()
{
  // Update the run manager
  fRunManager->SetUserInitialization(fDetector);
  G4PhysListFactory plf;
  plf.SetVerbose(0);
  G4VModularPhysicsList* pl = plf.GetReferencePhysList("QGSP_BERT");
  pl->SetVerboseLevel(0);
  fRunManager->SetUserInitialization(pl);
  fRunManager->SetUserInitialization(new ActionInitialization);

  // Set verbosities
  HandleVerbosities(); 
}

void G4Helper::HandleVerbosities()
{
  fUIManager->ApplyCommand("/run/verbose 0");      // max = 2
  fUIManager->ApplyCommand("/event/verbose 0");    // max = 2
  fUIManager->ApplyCommand("/tracking/verbose 0"); // max = 4
  fUIManager->ApplyCommand("/process/em/verbose 0");
  G4HadronicProcessStore::Instance()->SetVerbose(0);
}

void G4Helper::HandleVisualization()
{
  #ifdef G4VIS_USE
  if (fShowVis) 
  {
    fVisManager = new G4VisExecutive();
    fVisManager->SetVerboseLevel(0); // max = 6
    fVisManager->Initialize();
    std::string command = "/control/execute " + fVisMacroPath;
    fUIManager->ApplyCommand(command);
  }
  #endif
}

void G4Helper::WriteGDML()
{
  #ifdef G4_GDML
  G4cout << "Writing geometry to gdml file..." << G4endl;
  G4Navigator* nav =
    G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();

  G4VPhysicalVolume* w = nav->GetWorldVolume();
  G4PhysicalVolumeStore* volStore = G4PhysicalVolumeStore::GetInstance();
  /*for (auto it = volStore->begin(); it != volStore->end(); it++)
  {
    std::cout << (*it)->GetName() << std::endl;
  }*/
 // auto s = volStore->GetVolume("mppcs/mppc1");

  G4GDMLParser parser;
  parser.Write(fGDMLOutputPath, w, false);
  #endif
}

}
