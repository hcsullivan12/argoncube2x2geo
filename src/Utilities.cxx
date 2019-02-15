// 
// File: Utilities.cxx
//
// Author: Hunter Sullivan
//
// Discription: 
//

#include <assert.h>  

#include "Utilities.h"

#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

namespace arcutil
{

Utilities::Utilities()
{}

Utilities::~Utilities()
{}

void Utilities::Place(const std::vector<G4LogicalVolume*>&  geoms,
                      const std::vector<G4ThreeVector>&     positions,
                      const std::vector<G4RotationMatrix*>& rotations,
                      G4LogicalVolume*                      motherLV)
{
  // vecs must be same size
  unsigned size = geoms.size();
  assert(positions.size() == size);
  assert(rotations.size() == size);  

  for (unsigned g = 0; g < geoms.size(); g++)
  {
    new G4PVPlacement(rotations[g], positions[g], geoms[g], geoms[g]->GetName(), motherLV, false, 0);
  }
}

std::vector<G4double> Utilities::Stack(const std::vector<G4double>& geomsHalfLength,
                                       const G4double&              motherHalfLength)
{
  std::vector<G4double> steps;
  steps.resize(geomsHalfLength.size());
  G4double prevStep = -1*motherHalfLength;
  for (unsigned p = 0; p < steps.size(); p++)
  {
    G4double v = prevStep + geomsHalfLength[p];
    steps[p] = v;
    prevStep = v + geomsHalfLength[p];
  }
  return steps;
}
}
