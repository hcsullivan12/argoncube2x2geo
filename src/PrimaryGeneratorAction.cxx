// 
// File: PrimaryGeneratorAction.cxx
//
// Author: Hunter Sullivan
//
// Discription: 
//

#include "PrimaryGeneratorAction.h"

namespace majorana {

PrimaryGeneratorAction::PrimaryGeneratorAction(const Configuration& config)
: G4VUserPrimaryGeneratorAction(),
  m_nPrimaries(config.NPrimaries()),
  m_sourcePosSigma(config.SourcePosSigma()),
  m_sourcePeakE(config.SourcePeakE()),
  m_sourcePeakESigma(config.SourcePeakESigma())
{
  m_particleTable = 0;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{}

void PrimaryGeneratorAction::Reset(const G4float& r,
                                   const G4float& thetaDeg,
                                   const G4float& z)
{
  m_sourcePosition.clear();
  G4float x = r*std::cos(thetaDeg*twopi/180);
  G4float y = r*std::sin(thetaDeg*twopi/180);
  m_sourcePosition.push_back(x);
  m_sourcePosition.push_back(y);
  m_sourcePosition.push_back(z);
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  // Initialize gaussian generator
  time_t seed = time( NULL );
  CLHEP::HepJamesRandom randomEngine(static_cast<long>(seed));
  CLHEP::RandGaussQ gauss(randomEngine);
  CLHEP::RandFlat   flat(randomEngine);

  // Loop over the primaries  
  for (unsigned primary = 0; primary < m_nPrimaries; primary++)
  {
    // Smear position of this photon
    G4float x = gauss.fire(m_sourcePosition[0], m_sourcePosSigma);
    G4float y = gauss.fire(m_sourcePosition[1], m_sourcePosSigma);
    G4float z = m_sourcePosition[2];
    // Sample the momentum
    float p = gauss.fire(m_sourcePeakE, m_sourcePeakESigma);
    // Keep generating until pZ < 0
    float pX(0), pY(0), pZ(1);
    while (pZ >= 0)
    {
      float cosTheta = 2*flat.fire() - 1;
      float sinTheta = pow(1-pow(cosTheta,2),0.5);
      float phi      = 2*M_PI*flat.fire();

      pX = p*sinTheta*cos(phi);
      pY = p*sinTheta*sin(phi);
      pZ = p*cosTheta;
    }
    
    // For the polarization vector, sample a new random direction
    // and cross it with the momentum direction
    float u1 = flat.fire();
    float u2 = flat.fire();
    float u3 = flat.fire();
    TVector3 momentum(pX, pY, pZ);
    TVector3 temp(u1, u2, u3);
    TVector3 polarization = momentum.Cross(temp);
    float mag = polarization*polarization;
    polarization = polarization*std::sqrt(1/mag);

    // Add this vertex
    G4PrimaryVertex* vertex = new G4PrimaryVertex(x*cm, y*cm, z*cm, 0);
    event->AddPrimaryVertex(vertex);

    if (m_particleTable == 0)
    {
      m_particleTable = G4ParticleTable::GetParticleTable();
    }

    G4ParticleDefinition* particleDefinition = m_particleTable->FindParticle("opticalphoton");
    G4PrimaryParticle* g4Particle = new G4PrimaryParticle(particleDefinition,
                                                          momentum[0]*eV,
                                                          momentum[1]*eV,
                                                          momentum[2]*eV);
    g4Particle->SetCharge(0);
    g4Particle->SetPolarization(polarization[0],
                                polarization[1],
                                polarization[2]);
    vertex->SetPrimary(g4Particle);
  }
}
}
