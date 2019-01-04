// 
// File: OpDetPhotonTable.cxx
//
// Author: Hunter Sullivan
//
// Discription: 
//

#include "OpDetPhotonTable.h"

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

}
