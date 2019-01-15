// 
// File: DiskVolume.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class for interface to stepping action.
//

#include "SteppingAction.h"
#include "Configuration.h"

namespace majorana 
{

SteppingAction::SteppingAction()
 : m_opProcess(NULL)
{
  time_t seed = time( NULL );
  m_randomEngine.setSeed(static_cast<long>(seed+1));
}

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

  // We will handle surface absorption here
  G4ParticleDefinition* particleType = theTrack->GetDefinition();
  if(particleType==G4OpticalPhoton::OpticalPhotonDefinition())
  {
    // Make sure we're at the acrylic/air boundary
    G4String preName  = thePrePV->GetName();
    G4String postName = thePostPV->GetName();
    if (preName == "Disk" && postName == "World")
    {
      Configuration* config = Configuration::Instance();
      float surfaceAbs = config->SurfaceAbsorption();
      CLHEP::RandFlat   flat(m_randomEngine);
      float u = flat.fire();
      if (u < surfaceAbs) theTrack->SetTrackStatus(fStopAndKill);
    }
  }
}
}
