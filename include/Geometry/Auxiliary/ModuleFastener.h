// 
// File: ModuleFastener.h
//
// Author: Hunter Sullivan
//

#ifndef MODULEFASTENER_H
#define MODULEFASTENER_H

#include "G4LogicalVolume.hh"

namespace geo
{

class ModuleFastener 
{
  public:
    ModuleFastener();
    ~ModuleFastener();

    G4LogicalVolume* ConstructVolume(const std::string& name,
                                     const std::string& material);
};
}

#endif
