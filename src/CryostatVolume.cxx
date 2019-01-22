// 
// File: CryostatVolume.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to construct cryostat volume.
//

#include "CryostatVolume.h"
#include "Configuration.h"
#include "MaterialManager.h"

#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"

namespace geo
{

CryostatVolume::CryostatVolume()
:  fCryostatSolid(NULL),
   fCryostatLV(NULL)
{}

CryostatVolume::~CryostatVolume()
{}

void CryostatVolume::ConstructVolume(G4VPhysicalVolume* worldPV,
                                     G4LogicalVolume*   worldLV)
{

  //****
  // Construct cryostat volumes
  //****
  ConstructCryostat(worldLV);
   
  // Handle vis effects
  HandleVisAttributes();
}

void CryostatVolume::ConstructCryostat(G4LogicalVolume* worldLV)
{
  // Get material manager and config
  MaterialManager* materialManager = MaterialManager::Instance();
  Configuration* config = Configuration::Instance();
  
  //**** 
  // 	Cryostat shape
  //****
  // Expecting radius, depth, and wall thickness
  std::vector<double> cryostatDimOD = config->CryostatDimensions();
  double rMax = cryostatDimOD[0];
  double rMin = rMax - cryostatDimOD[2];
  double dz   = cryostatDimOD[1];
  // A bit of of hard-coding
  double dz2  = 20.0; 
  G4Tubs* cryostatMiddle = new G4Tubs("cryostatMiddle",
                                      rMin*cm,
                                      rMax*cm,
                                      (dz-dz2)/2.*cm,
                                      0*degree, 360*degree);
  G4Sphere* cryostatBottom = new G4Sphere("cryostatBottom",
                                          2*rMin*cm,
                                          2*rMax*cm,
                                          0*degree, 360*degree,
                                          150*degree, 180*degree);
/*  fCryostatODSolid = new G4UnionSolid("CryostatODSolid", 
                                      cryostatMiddle, 
                                      cryostatBottom,
                                      0,
  */
                                      
  G4LogicalVolume* middleLV = new G4LogicalVolume(cryostatMiddle,
                                      materialManager->FindMaterial("G10"),
                                     "MiddleLV");                                    
  G4LogicalVolume* bottomLV = new G4LogicalVolume(cryostatBottom,
                                      materialManager->FindMaterial("G10"),
                                     "BottomLV");
 // new G4PVPlacement(0, G4ThreeVector(0,0,-590), middleLV, "temp1", worldLV, false, 1);
  new G4PVPlacement(0, G4ThreeVector(), bottomLV, "temp2", worldLV, false, 1); 

  bottomLV->SetVisAttributes(G4Colour(1,0,0));
}

void CryostatVolume::HandleVisAttributes()
{
  // TPC
  G4VisAttributes* tpcVA = new G4VisAttributes(G4Colour(1,1,1));
  tpcVA->SetForceWireframe(false);
  //fTPCLV->SetVisAttributes(tpcVA); 

}

}
