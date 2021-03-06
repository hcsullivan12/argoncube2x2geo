// 
// File: Utilities.h
//
// Author: Hunter Sullivan
//

#ifndef UTILITIES_H
#define UTILITIES_H 

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

namespace arcutil
{

class Utilities 
{
  public:
    Utilities();
    ~Utilities();

    void Place(const std::vector<G4LogicalVolume*>&  geoms, 
               const std::vector<G4ThreeVector>&     positions,
               const std::vector<G4RotationMatrix*>& rotations,
               G4LogicalVolume*                      motherLV);

    std::vector<G4double> Stack(const std::vector<G4double>& geomsDim,
                                const G4double&              motherHalfLength);
};

}

#endif
