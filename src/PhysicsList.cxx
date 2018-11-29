// 
// File: PhysicsLists.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to construct physics list.
//

#include "PhysicsList.h"

namespace majorana {

PhysicsList::PhysicsList() 
 : G4VModularPhysicsList(),
    m_opticalPhysics(NULL)
{
  G4VModularPhysicsList* phys = new FTFP_BERT(0);

 /* for (G4int i = 0; ; ++i) 
  {
    G4VPhysicsConstructor* elem = const_cast<G4VPhysicsConstructor*> (phys->GetPhysics(i));
    if (elem == NULL) break;
     G4cout << "RegisterPhysics: " << elem->GetPhysicsName() << G4endl;
     RegisterPhysics(elem);
  }*/
    
  m_opticalPhysics = new OpticalPhysics;
  //G4cout << "RegisterPhysics: " << "OpticalPhysics" << G4endl;
  RegisterPhysics(m_opticalPhysics);
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::ConstructParticle()
{
  G4VModularPhysicsList::ConstructParticle(); 
}

void PhysicsList::ConstructProcess()
{
  G4VModularPhysicsList::ConstructProcess();
}

}
