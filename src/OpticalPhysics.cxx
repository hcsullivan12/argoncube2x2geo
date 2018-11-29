// 
// File: OpticalPhysics.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to construct optical physics.
//

#include "OpticalPhysics.h"

namespace majorana {

OpticalPhysics::OpticalPhysics()
 : G4VPhysicsConstructor("Optical"),
   m_boundaryProcess(NULL),
   m_absorptionProcess(NULL)
{
  m_absorptionProcess = new G4OpAbsorption();
  m_boundaryProcess   = new G4OpBoundaryProcess();
}

OpticalPhysics::~OpticalPhysics() 
{}

void OpticalPhysics::ConstructParticle()
{
  G4OpticalPhoton::OpticalPhotonDefinition();
}

void OpticalPhysics::ConstructProcess()
{  
  G4ProcessManager* pManager = G4OpticalPhoton::OpticalPhoton()->GetProcessManager();

  if (!pManager) 
  {
     std::ostringstream o;
     o << "Optical Photon without a Process Manager";
     G4Exception("WLSOpticalPhysics::ConstructProcess()", "", FatalException,o.str().c_str());
  }
  pManager->AddDiscreteProcess(m_absorptionProcess); 
  pManager->AddDiscreteProcess(m_boundaryProcess);
}

}
