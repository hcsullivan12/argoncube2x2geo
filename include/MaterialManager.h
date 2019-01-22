// 
// File: MatieralManager.h
//
// Author: Hunter Sullivan
//
// Discription: Class to construct detector materials.
//

#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include "G4Material.hh"

namespace majorana
{

class MaterialManager
{
  public:
    static MaterialManager* Instance();
    ~MaterialManager();

    void        ConstructMaterials();
    G4Material* FindMaterial(const G4String& name);

    std::vector<float> GetPhotonEnergies() const { return m_tpbEmissionE; };

  private:
    MaterialManager();
    static MaterialManager* instance;

    void DefineAir();
    void DefineAcrylic();
    void DefineMPPCMaterial();
    void DefineMPPCSurface();
 
    std::vector<float> m_tpbEmissionE;
    std::vector<float> m_tpbEmissionSpect; 

    G4Material* m_air;
    G4Material* m_si;
    G4Material* m_acrylic;
};
}

#endif
