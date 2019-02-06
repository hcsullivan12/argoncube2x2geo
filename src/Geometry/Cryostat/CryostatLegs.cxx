// 
// File: CryostatLegs.cxx
//
// Author: Hunter Sullivan
//

#include "Geometry/Cryostat/CryostatLegs.h"
#include "Configuration.h"
#include "MaterialManager.h"
#include "Utilities.h"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

namespace geo
{

CryostatLegs::CryostatLegs()
{}

CryostatLegs::~CryostatLegs()
{}

void CryostatLegs::ConstructVolume()
{
  ConstructSubVolumes();
}

void CryostatLegs::ConstructSubVolumes()
{    
  // Get material manager and config
  MaterialManager* matMan = MaterialManager::Instance();
  Configuration* config = Configuration::Instance();
  arcutil::Utilities util;

  std::vector<G4double> cryostatLegDim = config->CryostatLegDim(); util.ConvertToUnits(cryostatLegDim);
  std::vector<G4double> cryostatLegFootDim = config->CryostatLegFootDim(); util.ConvertToUnits(cryostatLegFootDim);
  std::vector<G4double> cryostatDim = config->CryostatDim(); util.ConvertToUnits(cryostatDim);
  G4double cryoInnerR             = cryostatDim[0];
  G4double cryoInnerDepth         = cryostatDim[1];
  G4double cryoInnerWallThickness = cryostatDim[2];
  G4double cryoWallGap            = cryostatDim[3];
  G4double cryoOuterR             = cryostatDim[4];
  G4double cryoOuterDepth         = cryostatDim[5];
  G4double cryoOuterWallThickness = cryostatDim[6];
  G4double cryoOuterWallR     = cryoOuterR + cryoOuterWallThickness;
  G4double cryoOuterWallDepth = cryoOuterDepth + cryoOuterWallThickness;



  G4Tubs* solCryoLegShin = new G4Tubs("solCryoLegShin",
                                       0,
                                       cryostatLegDim[1],
                                       cryostatLegDim[2]/2.,
                                       0*degree, 360*degree);
                                         
  G4Tubs* solCryoLegFoot = new G4Tubs("solCryoLegFoot",
                                       0,
                                       cryostatLegFootDim[1],
                                       cryostatLegFootDim[2]/2.,
                                       0*degree, 360*degree);

  G4ThreeVector transl(0, 0, solCryoLegShin->GetZHalfLength()-solCryoLegFoot->GetZHalfLength());
  G4UnionSolid* solCryoLeg = new G4UnionSolid("solCryoLeg",
                                               solCryoLegShin,
                                               solCryoLegFoot,
                                               0,
                                               -1*transl);

  fVolCryoLeg = new G4LogicalVolume(solCryoLeg,
                                    matMan->FindMaterial("SSteel304"),
                                    "volCryoLeg");
                                               

  // Place all four legs in container
  G4double legHeight = 2*(solCryoLegShin->GetZHalfLength()+solCryoLegFoot->GetZHalfLength());
  G4double offset = 10*cm; // this will do for now?
  G4Tubs* solCryoLegContainer = new G4Tubs("solCryoLegContainer",
                                           0,
                                           cryoOuterWallR+2*solCryoLegShin->GetRMax()+offset,
                                           cryoOuterWallDepth/2.0+legHeight/2.0 - offset,
                                           0*degree, 360*degree);
  fVolCryoLegContainer = new G4LogicalVolume(solCryoLegContainer,
                                             matMan->FindMaterial("Air"),
                                             "volCryoLegContainer");

  unsigned nLegs(5);
  G4double r = cryoOuterWallR+solCryoLegShin->GetRMax();
  G4double z = -1*solCryoLegContainer->GetZHalfLength()+legHeight/2.;
  for (unsigned l = 1; l <= nLegs; l++)
  {
    G4double thetaDiff = (360/nLegs)*degree;
    G4double theta = (l-1)*thetaDiff;

    G4double x = r*std::cos(theta);
    G4double y = r*std::sin(theta);

    G4ThreeVector pos(x,y,z);
    new G4PVPlacement(0, pos, fVolCryoLeg, fVolCryoLeg->GetName()+"_pos"+std::to_string(l), fVolCryoLegContainer, false, l-1);
  }
}
}
