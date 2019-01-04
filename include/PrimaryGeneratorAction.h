// 
// File: PrimaryGeneratorAction.h
//
// Author: Hunter Sullivan
//
// Discription: 
//

#ifndef PRIMARYGENERATORACTION_H
#define PRIMARYGENERATORACTION_H

#include "Configuration.h"

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4PrimaryVertex.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"

#include "CLHEP/Random/JamesRandom.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGaussQ.h"

#include "TVector3.h"

class G4Event;

namespace majorana {

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(const Configuration& config);    
    virtual ~PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event*);         

    void Reset(const float& r,
               const float& thetaDeg,
               const float& z);
    void Append();
    std::vector<float> GetSourcePositionRTZ() const { return m_sourcePositionRTZ; };
    std::vector<float> GetSourcePositionXYZ() const { return m_sourcePositionXYZ; };
    unsigned           GetNPrimaries()        const { return m_nPrimaries; };

  private:
    G4ParticleTable*   m_particleTable;
    unsigned           m_nPrimaries;
    std::vector<float> m_sourcePositionRTZ;
    std::vector<float> m_sourcePositionXYZ;
    float              m_sourcePosSigma;
    float              m_sourcePeakE;
    float              m_sourcePeakESigma;
};

}
#endif
