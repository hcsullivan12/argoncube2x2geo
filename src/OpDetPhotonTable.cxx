// 
// File: OpDetPhotonTable.cxx
//
// Author: Hunter Sullivan
//
// Discription: 
//

#include "OpDetPhotonTable.h"
#include "Configuration.h"

#include "globals.hh"

#include <iostream> 

namespace majorana 
{

Photon::Photon(const std::vector<float>& photonVertex)
 : m_vertex(photonVertex)
{}

Photon::~Photon()
{}

OpDetPhotonTable* OpDetPhotonTable::instance = 0;

OpDetPhotonTable* OpDetPhotonTable::Instance()
{
  if (instance == 0)
  {
    static OpDetPhotonTable opDetPhotonTable;
    instance = &opDetPhotonTable;
  }
  return instance;
}

OpDetPhotonTable::OpDetPhotonTable()
{
  m_photonsDetected.clear();
  Initialize();
}

OpDetPhotonTable::~OpDetPhotonTable()
{}

void OpDetPhotonTable::AddPhoton(const unsigned& opchannel, const Photon& photon)
{ 
  if (m_photonsDetected.find(opchannel) == m_photonsDetected.end())
  {
    m_photonsDetected.emplace(opchannel, std::vector<Photon>());
  }
  m_photonsDetected.find(opchannel)->second.push_back(photon);
}

void OpDetPhotonTable::Print()
{
  for (const auto& k : m_photonsDetected)
  {
    G4cout << "MPPC" << k.first << " detected " << k.second.size() << " photons" << G4endl;
  }
}

void OpDetPhotonTable::Initialize()
{
  Configuration* config = Configuration::Instance();
  std::vector<Photon> vec;
  for (unsigned m = 1; m <= config->NMPPCs(); m++)
  {
    m_photonsDetected.emplace(m, vec);
  }
}

}
