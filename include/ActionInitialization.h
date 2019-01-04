// 
// File: ActionInitialization.h
//
// Author: Hunter Sullivan
//
// Discription: 
//


#ifndef ACTIONINITIALIZATION_H
#define ACTIONINITIALIZATION_H 

#include "PrimaryGeneratorAction.h"
#include "Configuration.h"
#include "SteppingAction.h"
 
#include "G4VUserActionInitialization.hh"

namespace majorana
{

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization(const Configuration& config);
    virtual ~ActionInitialization();

    virtual void Build() const;

    PrimaryGeneratorAction* GetGeneratorAction() const { return m_generatorAction; };

  private:
    PrimaryGeneratorAction*  m_generatorAction;
    SteppingAction*          m_steppingAction;
};

}

#endif
