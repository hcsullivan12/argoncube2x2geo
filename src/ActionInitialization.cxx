// 
// File: ActionInitialization.cxx
//
// Author: Hunter Sullivan
//
// Discription: 
//

#include "ActionInitialization.h"

namespace majorana
{

ActionInitialization::ActionInitialization()
 : G4VUserActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction);
}

}
