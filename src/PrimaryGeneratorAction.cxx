// 
// File: PrimaryGeneratorAction.cxx
//
// Author: Hunter Sullivan
//
// Discription: 
//

#include "PrimaryGeneratorAction.h"
#include "Configuration.h"

#include "G4PrimaryVertex.hh"
#include "G4SystemOfUnits.hh"

namespace geo {

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleTable(NULL)
{}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{ 
  // Add this vertex
  G4PrimaryVertex* vertex = new G4PrimaryVertex(0*cm, 0*cm, 0*cm, 0);
  event->AddPrimaryVertex(vertex);

  if (fParticleTable == 0)
  {
    fParticleTable = G4ParticleTable::GetParticleTable();
  }

  G4ParticleDefinition* particleDefinition = fParticleTable->FindParticle("proton");
  G4PrimaryParticle* g4Particle = new G4PrimaryParticle(particleDefinition,
                                                        1*eV,
                                                        0*eV,
                                                        0*eV);
  vertex->SetPrimary(g4Particle);
}
}
