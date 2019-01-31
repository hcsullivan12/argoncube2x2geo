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

namespace geo
{

class MaterialManager
{
  public:
    static MaterialManager* Instance();
    ~MaterialManager();

    void        ConstructMaterials();
    G4Material* FindMaterial(const G4String& name);

    std::vector<float> GetPhotonEnergies() const { return fTPBEmissionE; };

  private:
    MaterialManager();
    static MaterialManager* instance;

    void DefineAir();
    void DefineAcrylic();
    void DefineLAr();
 
    std::vector<float> fTPBEmissionE;
    std::vector<float> fTPBEmissionSpect; 

    G4Material* fAir;
    G4Material* fAcrylic;
    G4Material* fLAr;
};
}

#endif
