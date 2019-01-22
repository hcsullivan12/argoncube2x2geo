// 
// File: PrimaryGeneratorAction.h
//
// Author: Hunter Sullivan
//
// Discription: 
//

#ifndef PRIMARYGENERATORACTION_H
#define PRIMARYGENERATORACTION_H

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"

class G4Event;

namespace geo
{

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();    
    virtual ~PrimaryGeneratorAction();
    virtual void GeneratePrimaries(G4Event*);         

  private:
    G4ParticleTable* fParticleTable;
};

}
#endif
