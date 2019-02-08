// 
// File: ActionInitialization.h
//
// Author: Hunter Sullivan
//

#ifndef ACTIONINITIALIZATION_H
#define ACTIONINITIALIZATION_H 

#include "PrimaryGeneratorAction.h"
 
#include "G4VUserActionInitialization.hh"

namespace geo
{

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization();
    virtual ~ActionInitialization();
    virtual void Build() const;

  private:
    PrimaryGeneratorAction* fGeneratorAction;
};

}

#endif
