// 
// File: Cryostat.cxx
//
// Author: Hunter Sullivan
//

#include "Geometry/Cryostat/Cryostat.h"
#include "Configuration.h"
#include "MaterialManager.h"
#include "Utilities.h"

#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"

namespace geo
{

Cryostat::Cryostat()
{}

Cryostat::~Cryostat()
{}

void Cryostat::ConstructVolume(G4LogicalVolume* volWorld,
                               Detector*        detector)
{
  ConstructSubVolumes(detector);
  PlaceSubVolumes(volWorld);
}

void Cryostat::ConstructSubVolumes(Detector* detector)
{
  fCryostatBody = new CryostatBody();
  fCryostatBody->ConstructVolume(detector);

  fCryostatLegs = new CryostatLegs();
  fCryostatLegs->ConstructVolume();

  fCryostatFlange = new CryostatFlange();
  fCryostatFlange->ConstructVolume();
}

void Cryostat::PlaceSubVolumes(G4LogicalVolume* volWorld)
{
  G4LogicalVolume* volCryoBody = fCryostatBody->GetLV();
  G4LogicalVolume* volCryoLegs = fCryostatLegs->GetLV();
  //G4LogicalVolume* volCryoFlange = fCryostatBody->GetLV();
  
  G4double zLen = ((G4Tubs*)volCryoLegs->GetSolid())->GetZHalfLength();
  G4double shift = ((G4Tubs*)volCryoLegs->GetSolid())->GetZHalfLength() - fCryostatBody->GetOuterWallDepth()/2.;

  G4RotationMatrix* xRot2 = new G4RotationMatrix;
  xRot2->rotateX(pi/2);
  new G4PVPlacement(0, G4ThreeVector(0,0,shift), volCryoBody, volCryoBody->GetName()+"_pos", volCryoLegs, false, 0);
  new G4PVPlacement(xRot2, G4ThreeVector(), volCryoLegs, volCryoBody->GetName()+"_pos", volWorld, false, 0);  
}
}
