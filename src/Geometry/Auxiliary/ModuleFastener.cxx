// 
// File: ModuleFastener.cxx
//
// Author: Hunter Sullivan
//

#include "Geometry/Auxiliary/ModuleFastener.h"
#include "MaterialManager.h"

#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4RotationMatrix.hh"
#include "G4PhysicalConstants.hh"

namespace geo
{

ModuleFastener::ModuleFastener()
{}

ModuleFastener::~ModuleFastener()
{}

G4LogicalVolume* ModuleFastener::ConstructVolume(const std::string& name,
                                                 const std::string& material)
{
  MaterialManager* matMan       = MaterialManager::Instance();
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  G4Box* solModuleTopWall       = (G4Box*)lvStore->GetVolume("volModuleTopWall")->GetSolid();

  // Rectangular component
  G4Box* solModuleFastenerRect = new G4Box("solModuleFastenerRect",
                                            solModuleTopWall->GetXHalfLength(),
                                            (5.0/2.)*cm,
                                            (1.0/2.)*cm);
  // Edges
  G4double x = (std::sqrt(2)/2)*solModuleFastenerRect->GetYHalfLength();
  G4Box* solModuleFastenerEdge = new G4Box("solModuleFastenerEdge",
                                            x,
                                            x,
                                            solModuleFastenerRect->GetZHalfLength());
  // Union
  G4RotationMatrix *rot = new G4RotationMatrix;
  rot->rotateZ(pi/4);
  G4ThreeVector transl(solModuleFastenerRect->GetXHalfLength(),0,0);

  G4UnionSolid* solModuleFastenerTemp = new G4UnionSolid("solModuleFastenerTemp",
                                                          solModuleFastenerRect,
                                                          solModuleFastenerEdge,
                                                          rot,
                                                          transl);
  G4UnionSolid* solModuleFastener = new G4UnionSolid("solModuleFastener",
                                                      solModuleFastenerTemp,
                                                      solModuleFastenerEdge,
                                                      rot,
                                                      -1*transl);  

  return new G4LogicalVolume(solModuleFastener,
                              matMan->FindMaterial(material),
                              "vol"+name);

}
}
