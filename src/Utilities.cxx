// 
// File: Utilities.cxx
//
// Author: Hunter Sullivan
//
// Discription: 
//

#include "Utilities.h"

#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

namespace arcutil
{

Utilities::Utilities()
{}

Utilities::~Utilities()
{}

void Utilities::Place(std::vector<G4LogicalVolume*>     geoms,
                      const std::vector<G4ThreeVector>& positions,
                      G4LogicalVolume*                  motherLV)
{
  for (unsigned g = 0; g < geoms.size(); g++)
  {
    new G4PVPlacement(0, positions[g], geoms[g], geoms[g]->GetName()+"_pos", motherLV, false, 0);
  }
}

/*// Place them 
  // future: Reference to cyrostat center
  G4double l = moduleDimOD[0]/2. + moduleClearance/2.;
  G4ThreeVector translation(l*cm, l*cm, 0);
  std::string modName = "modules/module";
  std::string tpcName = "tpcs/tpc";
 
  // Modules
  new G4PVPlacement(0, translation, fModuleLV, modName+std::to_string(1), worldLV, false, 0);
  translation[0] = -1*l*cm;
  new G4PVPlacement(0, translation, fModuleLV, modName+std::to_string(2), worldLV, false, 2);
  translation[1] = -1*l*cm;
  new G4PVPlacement(0, translation, fModuleLV, modName+std::to_string(3), worldLV, false, 3);
  translation[0] = l*cm; translation[1] = -1*l*cm;
  new G4PVPlacement(0, translation, fModuleLV, modName+std::to_string(4), worldLV, false, 4); 

  // TPCs
  new G4PVPlacement(0, G4ThreeVector(), fTPCLV, tpcName+std::to_string(1), fModuleLV, false, 0);
*/

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
