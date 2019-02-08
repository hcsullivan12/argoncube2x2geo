// 
// File: Feedthrough.h
//
// Author: Hunter Sullivan
//

#ifndef FEEDTHROUGH_H
#define FEEDTHROUGH_H

#include "G4LogicalVolume.hh"

namespace geo
{

class Feedthrough 
{
  public:
    Feedthrough();
    ~Feedthrough();

    G4LogicalVolume* ConstructVolume(const std::string& name,
                                     const G4double&    minR,
                                     const G4double&    maxR,
                                     const G4double&    height,
                                     const std::string& material);
};
}

#endif
