//
// File: OpDetPhotonTable.h
//
// Author: Hunter Sullivan
//
// Discription:
//

#ifndef OPDETPHOTONTABLE_HH
#define OPDETPHOTONTABLE_HH

#include <vector>
#include <map>

namespace majorana {

class Photon
{
  public:
    Photon(const std::vector<float>& photonVertex);
    ~Photon();

    const std::vector<float>& Vertex() const { return m_vertex; };
    
  private:
    std::vector<float> m_vertex;
};

class OpDetPhotonTable
{
  using PhotonsDetected = std::map<unsigned, std::vector<Photon>>;

  public:
    static OpDetPhotonTable* Instance();
    ~OpDetPhotonTable();

    void AddPhoton(const unsigned& opchannel, const Photon& photon);
    inline void Clear() { m_photonsDetected.clear(); };

    const PhotonsDetected& GetPhotonsDetected() const { return m_photonsDetected; };

  private:
    OpDetPhotonTable();
    static OpDetPhotonTable* instance;

    PhotonsDetected m_photonsDetected;
};
}
#endif
