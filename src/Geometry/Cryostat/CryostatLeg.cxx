// 
// File: CryostatLeg.cxx
//
// Author: Hunter Sullivan
//

#include "Geometry/Cryostat/CryostatLeg.h"
#include "QuantityStore.h"
#include "MaterialManager.h"
#include "Utilities.h"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

namespace geo
{

CryostatLeg::CryostatLeg()
{}

CryostatLeg::~CryostatLeg()
{}

void CryostatLeg::ConstructVolume()
{
  ConstructSubVolumes();
}

void CryostatLeg::ConstructSubVolumes()
{    
  // Get material manager and qStore
  MaterialManager* matMan = MaterialManager::Instance();
  QuantityStore* qStore   = QuantityStore::Instance();

  std::vector<G4double> cryostatLegDim = qStore->kCryostatLegDim; 
  std::vector<G4double> cryostatLegFootDim = qStore->kCryostatLegFootDim; 

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

  qStore->kCryoLegHeight = cryostatLegDim[2];
  qStore->kCryoLegShinR  = cryostatLegDim[1];
}
}
