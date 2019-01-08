#include "MPPCSD.h"
#include "OpDetPhotonTable.h"
#include "G4Helper.h"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"

namespace majorana
{

MPPCSD::MPPCSD(G4String name)
  : G4VSensitiveDetector(name)
{}

MPPCSD::~MPPCSD() 
{}

void MPPCSD::Initialize(G4HCofThisEvent* hitsCE)
{}

G4bool MPPCSD::ProcessHits(G4Step* aStep, G4TouchableHistory* )
{
  G4Track* theTrack = aStep->GetTrack();

  // If this isn't an optical photon, exit
  if(theTrack->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) return false;
 
  // Get mppc ID
  G4int mppcNumber           = aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber();

  /*auto pos = aStep->GetPostStepPoint()->GetPosition();
  float r        = std::sqrt(pos[0]*pos[0] + pos[1]*pos[1]);
  float thetaDeg = TMath::ASin(std::abs(pos[1]/pos[0]))*180/pi;
  if (pos[0] < 0 && pos[1] > 0) thetaDeg = 180 - thetaDeg;
  if (pos[0] < 0 && pos[1] < 0) thetaDeg = 180 + thetaDeg;
  if (pos[0] > 0 && pos[1] < 0) thetaDeg = 360 - thetaDeg;

  std::cout << pos[0] << "  " << pos[1] << "  " <<  r/10.0 << " " << thetaDeg << "  " << mppcNumber << std::endl;*/

  // We want to store the information for this event
  G4Helper* g4Helper = G4Helper::Instance();
  if (!g4Helper) 
  { 
    G4cout << "\nG4Helper not initialized..." << G4endl;
    return false;
  }
  PrimaryGeneratorAction* genAction = g4Helper->GetActionInitialization()->GetGeneratorAction();
  if (!genAction) 
  {
    G4cout << "\nGenAction not initialized..." << G4endl;
    return false;
  }

  const std::vector<float> photonVertex = { genAction->GetSourcePositionRTZ()[0], 
                                            genAction->GetSourcePositionRTZ()[1],
                                            genAction->GetSourcePositionRTZ()[2]};

  // Create the photon
  Photon thePhoton(photonVertex);
  // Add to table 
  OpDetPhotonTable* thePhotonTable = OpDetPhotonTable::Instance();
  thePhotonTable->AddPhoton(mppcNumber, thePhoton);
 
  theTrack->SetTrackStatus(fStopAndKill);  
  return true;
}

void MPPCSD::EndOfEvent(G4HCofThisEvent* ) {}

void MPPCSD::clear() {}

void MPPCSD::DrawAll() {}

void MPPCSD::PrintAll() {}

}
