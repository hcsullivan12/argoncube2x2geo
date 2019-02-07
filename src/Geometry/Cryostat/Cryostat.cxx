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

  fCryostatLeg = new CryostatLeg();
  fCryostatLeg->ConstructVolume();

  //fCryostatFlange = new CryostatFlange();
  //fCryostatFlange->ConstructVolume();

  MaterialManager* matMan = MaterialManager::Instance();
  Configuration* config = Configuration::Instance();
  arcutil::Utilities util;

  G4double cryoLegOffset = config->CryostatLegOffset(); util.ConvertToUnits(cryoLegOffset);
  G4double bodyH   = fCryostatBody->GetDepth();
  G4double bodyR   = fCryostatBody->GetOuterWallR();
  G4double legH    = fCryostatLeg->GetHeight();
  //G4double flangeH = fCryostatFlange->GetHeight();
  
  // Create flange object to wrap around 
  G4Tubs* solCryotatTop = new G4Tubs("solCryotatTop",
                                      bodyR,
                                      bodyR+10*cm,
                                      10*cm,
                                      0*degree, 360*degree);

  fVolCryotatTop = new G4LogicalVolume(solCryotatTop,
                                       matMan->FindMaterial("SSteel304"),
                                       "volCryotatTop");

  G4Tubs* solCryostatFlange = new G4Tubs("solCryostatFlange",
                                     0*m,
                                     solCryotatTop->GetRMax(),
                                     0.5*cm,
                                     0*degree, 360*degree);
  fVolCryostatFlange = new G4LogicalVolume(solCryostatFlange,
                                           matMan->FindMaterial("SSteel304"),
                                           "volTopFlange");                                       

  G4Tubs* solCryotatContainer = new G4Tubs("solCryostatContainer",
                                            0,
                                            1.5*m,
                                            (bodyH+legH+2*solCryostatFlange->GetZHalfLength()-cryoLegOffset)/2.,
                                            0*degree, 360*degree);

  fVolCryotatContainer = new G4LogicalVolume(solCryotatContainer,
                                              matMan->FindMaterial("LAr"),
                                              "volCryotatContainer");                                              
}

void Cryostat::PlaceSubVolumes(G4LogicalVolume* volWorld)
{
  Configuration* config = Configuration::Instance();
  arcutil::Utilities util;

  G4double cryoLegOffset = config->CryostatLegOffset(); util.ConvertToUnits(cryoLegOffset);

  // Compute shifts
  //G4LogicalVolume* volCryoFlange = fCryostatFlange->GetLV();
  G4double containerZLen = 2*((G4Tubs*)fVolCryotatContainer->GetSolid())->GetZHalfLength();
  std::vector<G4double> shift = { containerZLen/2. - ((G4Tubs*)fVolCryostatFlange->GetSolid())->GetZHalfLength(),
                                  containerZLen/2. - 2*((G4Tubs*)fVolCryostatFlange->GetSolid())->GetZHalfLength() - fCryostatBody->GetOuterWallTubDepth()/2. ,
                                -1*containerZLen/2. + fCryostatLeg->GetHeight()/2.,
                                  containerZLen/2. - ((G4Tubs*)fVolCryotatTop->GetSolid())->GetZHalfLength() };

  // Place flange
  new G4PVPlacement(0, G4ThreeVector(0,0,shift[0]), fVolCryostatFlange, fVolCryostatFlange->GetName()+"_pos", fVolCryotatContainer, false, 0);
  // Place body
  new G4PVPlacement(0, G4ThreeVector(0,0,shift[1]), fCryostatBody->GetLV(), fCryostatBody->GetLV()->GetName()+"_pos", fVolCryotatContainer, false, 0);                                          

  // Place all legs in the container
  unsigned nLegs(5);
  G4double r = fCryostatBody->GetOuterWallR()+fCryostatLeg->GetShinR();
  for (unsigned l = 1; l <= nLegs; l++)
  {
    G4double thetaDiff = (360/nLegs)*degree;
    G4double theta = (l-1)*thetaDiff;

    G4double x = r*std::cos(theta);
    G4double y = r*std::sin(theta);

    G4ThreeVector pos(x,y,shift[2]);
    new G4PVPlacement(0, pos, fCryostatLeg->GetLV(), fCryostatLeg->GetLV()->GetName()+"_pos"+std::to_string(l), fVolCryotatContainer, false, l-1);
  }
  
  //G4double zLen = ((G4Tubs*)fVolCryotatContainer->GetSolid())->GetZHalfLength() + ((G4Tubs*)volCryoFlange->GetSolid())->GetZHalfLength();
  //G4double shift = ((G4Tubs*)volCryoLegs->GetSolid())->GetZHalfLength() - fCryostatBody->GetOuterWallDepth()/2.;

  G4RotationMatrix* xRot2 = new G4RotationMatrix;
  xRot2->rotateX(pi/2);
  new G4PVPlacement(xRot2, G4ThreeVector(), fVolCryotatContainer, fVolCryotatContainer->GetName()+"_pos", volWorld, false, 0);
  new G4PVPlacement(xRot2, G4ThreeVector(0,shift[3],0), fVolCryotatTop, fVolCryotatTop->GetName()+"_pos", volWorld, false, 0);
  //new G4PVPlacement(xRot2, G4ThreeVector(0,zLen,0), volCryoFlange, volCryoFlange->GetName()+"_pos", volWorld, false, 0);

}
}
