// 
// File: DiskVolume.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class for interface to stepping action.
//

#include "SteppingAction.h"

namespace majorana 
{

SteppingAction::SteppingAction()
 : m_opProcess(NULL)
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* theStep)
{
  G4Track* theTrack = theStep->GetTrack();
  //WLSUserTrackInformation* trackInformation = (WLSUserTrackInformation*)theTrack->GetUserInformation();

  G4StepPoint* thePrePoint  = theStep->GetPreStepPoint();
  G4StepPoint* thePostPoint = theStep->GetPostStepPoint();

  G4VPhysicalVolume* thePrePV  = thePrePoint->GetPhysicalVolume();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

  G4String thePrePVname  = " ";
  G4String thePostPVname = " "; 

  // Retrieve the status of the photon
  G4OpBoundaryProcessStatus theStatus = Undefined;
  G4ProcessManager* opManager = G4OpticalPhoton::OpticalPhoton()->GetProcessManager(); 

  if (opManager) 
  {
    G4int MAXofPostStepLoops = opManager->GetPostStepProcessVector()->entries();
    G4ProcessVector* fPostStepDoItVector = opManager->GetPostStepProcessVector(typeDoIt);

    if (thePostPV) 
    {
      
    //for ( G4int i=0; i<MAXofPostStepLoops; i++) 
    //{
      G4VProcess* fCurrentProcess = (*fPostStepDoItVector)[0];
      m_opProcess = dynamic_cast<G4OpBoundaryProcess*>(fCurrentProcess);
      if (m_opProcess) { theStatus = m_opProcess->GetStatus(); }//break;}
      
      thePrePVname  = thePrePV->GetName();
      thePostPVname = thePostPV->GetName();
      if (thePostPVname != "Disk" && thePostPVname != "World")
      { std::cout << thePrePVname << "  " 
                  << thePostPVname << "  " 
                  << theStatus 
                  << std::endl; 
      }
    } 

    //}
  } 
 
}
}
