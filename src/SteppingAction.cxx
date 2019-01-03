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
  //**** 
  // We have to check if the photons have hit our mppcs!
  //****
  G4Track*           theTrack     = theStep->GetTrack();
  G4StepPoint*       thePrePoint  = theStep->GetPreStepPoint();
  G4StepPoint*       thePostPoint = theStep->GetPostStepPoint();
  G4VPhysicalVolume* thePostPV    = thePostPoint->GetPhysicalVolume();
  G4VPhysicalVolume* thePrePV     = thePrePoint->GetPhysicalVolume();

  if (!thePostPV) return;

  G4ParticleDefinition* particleType = theTrack->GetDefinition();
  if(particleType==G4OpticalPhoton::OpticalPhotonDefinition())
  {
    //if (thePrePV->GetName() == "Disk")
  }

  
 
}
}
