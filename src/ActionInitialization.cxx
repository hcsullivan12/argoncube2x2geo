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

ActionInitialization::ActionInitialization(const Configuration& config)
 : G4VUserActionInitialization(),
   m_generatorAction(NULL)
{
  m_generatorAction = new PrimaryGeneratorAction(config);
}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::Build() const
{
  SetUserAction(m_generatorAction);
}

}
