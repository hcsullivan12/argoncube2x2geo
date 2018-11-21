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
 
#include "G4VUserActionInitialization.hh"

namespace majorana
{

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization();
    virtual ~ActionInitialization();

    //virtual void BuildForMaster() const;
    virtual void Build() const;
};

}

#endif
