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
    MaterialManager();
    ~MaterialManager();

    void        ConstructMaterials();
    G4Material* FindMaterial(const G4String& name);

  private:
    void DefineAir();
    void DefineAcrylic();
 
    std::vector<float> m_tpbEmissionE;
    std::vector<float> m_tpbEmissionSpect; 

    G4Material* m_air;
    G4Material* m_acrylic;
};
}

#endif