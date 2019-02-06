// 
// File: ModuleFlange.h
//
// Author: Hunter Sullivan
//

#ifndef MODULEFLANGE_H
#define MODULEFLANGE_H

#include "G4LogicalVolume.hh"

namespace geo
{

class ModuleFlange 
{
  public:
    ModuleFlange();
    ~ModuleFlange();

    void ConstructVolume();

    G4LogicalVolume* GetLV() { return fVolModuleFlange; };

  private:

    G4LogicalVolume* fVolModuleFlange;
    
};
}

#endif
