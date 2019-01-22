// 
// File: ActionInitialization.cxx
//
// Author: Hunter Sullivan
//
// Discription: 
//

#include "ActionInitialization.h"

namespace geo
{

ActionInitialization::ActionInitialization()
 : G4VUserActionInitialization(),
   fGeneratorAction(NULL)
{
  fGeneratorAction = new PrimaryGeneratorAction();
}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::Build() const
{
  SetUserAction(fGeneratorAction);
}

}
