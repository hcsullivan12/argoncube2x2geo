#include "MPPCSD.h"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"

MPPCSD::MPPCSD(G4String name)
  : G4VSensitiveDetector(name),
    m_mppcPositionsX(0),
    m_mppcPositionsY(0),
    m_mppcPositionsZ(0)
{}

MPPCSD::~MPPCSD() 
{}

void MPPCSD::SetMPPCPositions(const std::vector<G4ThreeVector>& positions)
{
  for (G4int i=0; i<G4int(positions.size()); ++i)
  {
    if(m_mppcPositionsX) m_mppcPositionsX->push_back(positions[i].x());
    if(m_mppcPositionsY) m_mppcPositionsY->push_back(positions[i].y());
    if(m_mppcPositionsZ) m_mppcPositionsZ->push_back(positions[i].z());
  }
}

void MPPCSD::Initialize(G4HCofThisEvent* hitsCE)
{}

G4bool MPPCSD::ProcessHits(G4Step* aStep, G4TouchableHistory* )
{
  // If this isn't an optical photon, exit
  if(aStep->GetTrack()->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) return false;
 
  //User replica number 1 since photocathode is a daughter volume
  //to the pmt which was replicated
  G4int mppcNumber           = aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber(1);
  G4VPhysicalVolume* physVol = aStep->GetPostStepPoint()->GetTouchable()->GetVolume(1);

 
  std::cout << "HERERERE\n";
 
  /*if(!LXeDetectorConstruction::GetSphereOn()){
    hit->SetDrawit(true);
    //If the sphere is disabled then this hit is automaticaly drawn
  }
  else{//sphere enabled
    LXeUserTrackInformation* trackInfo=
      (LXeUserTrackInformation*)aStep->GetTrack()->GetUserInformation();
    if(trackInfo->GetTrackStatus()&hitSphere)
      //only draw this hit if the photon has hit the sphere first
      hit->SetDrawit(true);
  }*/

  return true;
}

void MPPCSD::EndOfEvent(G4HCofThisEvent* ) {}

void MPPCSD::clear() {}

void MPPCSD::DrawAll() {}

void MPPCSD::PrintAll() {}
