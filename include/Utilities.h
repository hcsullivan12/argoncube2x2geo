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
#include "G4SystemOfUnits.hh"

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

    inline void ConvertToUnits(std::vector<G4double>& vec) 
    {
      for (auto& v : vec) v=v*cm;
    }
    inline void ConvertToUnits(G4double& v) 
    {
      v=v*cm;
    }
};

}

#endif
