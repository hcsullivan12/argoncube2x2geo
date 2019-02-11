// 
// File: MatieralManager.h
//
// Author: Hunter Sullivan
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

  private:
    MaterialManager();
    static MaterialManager* instance;
};
}

#endif
