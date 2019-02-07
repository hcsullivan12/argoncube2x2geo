// 
// File: Feedthrough.cxx
//
// Author: Hunter Sullivan
//

#include "Geometry/Auxiliary/Feedthrough.h"
#include "Configuration.h"
#include "MaterialManager.h"
#include "Utilities.h"
#include "MaterialManager.h"

#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnionSolid.hh"

namespace geo
{

Feedthrough::Feedthrough()
{}

Feedthrough::~Feedthrough()
{}

G4LogicalVolume* Feedthrough::ConstructVolume(const std::string& name,
                                              const G4double&    minR,
                                              const G4double&    maxR,
                                              const G4double&    height,
                                              const std::string& material)
{
  MaterialManager* matMan = MaterialManager::Instance();
  G4Tubs* ftTub1 = new G4Tubs("sol"+name+"Tub1_",
                              minR,
                              maxR,
                              height/2.,
                              0*degree, 360*degree);
  G4Tubs* ftTub2 = new G4Tubs("sol"+name+"Tub2_",
                               0,
                               maxR+2*cm,
                               2*cm,
                               0*degree, 360*degree); 

  G4UnionSolid* solModuleMedFT = new G4UnionSolid("sol"+name,
                                                  ftTub1,
                                                  ftTub2,
                                                  0,
                                                  G4ThreeVector(0,0,ftTub1->GetZHalfLength()-ftTub2->GetZHalfLength()) );
  return new G4LogicalVolume(solModuleMedFT,
                             matMan->FindMaterial(material),
                             "vol"+name);
}
}
