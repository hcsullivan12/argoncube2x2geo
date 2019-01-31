// 
// File: Utilities.h
//
// Author: Hunter Sullivan
//
// Discription: 
//


#ifndef UTILITIES_H
#define UTILITIES_H 

#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"

namespace arcutil
{

class Utilities 
{
  public:
    Utilities();
    ~Utilities();

    void Place(std::vector<G4LogicalVolume*>     geoms, 
               const std::vector<G4ThreeVector>& positions,
               G4LogicalVolume*                  motherLV);
    std::vector<G4double> Stack(const std::vector<G4double>& geomsDim,
                                const G4double&              motherHalfLength);
};

}

#endif
