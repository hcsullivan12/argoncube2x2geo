// 
// File: Cryostat.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to construct cryostat volume.
//

#include "Cryostat.h"
#include "Configuration.h"
#include "MaterialManager.h"
#include "Utilities.h"

#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4UnionSolid.hh"

namespace geo
{

Cryostat::Cryostat()
{}

Cryostat::~Cryostat()
{}

void Cryostat::ConstructVolume(G4LogicalVolume* volWorld,
                               Detector*        detector)
{

  //****
  // Construct cryostat volumes
  //****
  ConstructCryostat(volWorld);
}

void Cryostat::ConstructCryostat(G4LogicalVolume* volWorld)
{
  // Get material manager and config
  MaterialManager* materialManager = MaterialManager::Instance();
  if (!materialManager) std::cout << "HEYYYYYY\n";
  Configuration* config = Configuration::Instance();
  arcutil::Utilities util;
  
  //**** 
  // 	Cryostat shape
  //****
  // Expecting radius, depth, and wall thickness
  // A bit of of hard-coding
  std::vector<G4double> cryostatDimOD = config->CryostatDimensions(); util.ConvertToUnits(cryostatDimOD);
  G4double cryoRMax = cryostatDimOD[0];
  G4double cryoRMin = cryoRMax - cryostatDimOD[2];
  G4double bowlRMax = 2*cryoRMax;
  G4double bowlRMin = bowlRMax - cryostatDimOD[2];
  G4double depth    = cryostatDimOD[1];
  G4double sinTheta = cryoRMax/bowlRMax;
  G4double cosTheta = std::sqrt(1 - sinTheta*sinTheta);
  G4double y        = bowlRMax*(1 - cosTheta);
  G4double h        = depth - y;
  G4double x        = bowlRMax + 0.5*h - depth;
  G4double theta    = std::abs(std::asin(sinTheta)*180/pi);

  std::cout << y << " " << h << " " << x << " " << depth << std::endl;
 
  G4Tubs* cryostatMiddle = new G4Tubs("cryostatMiddle",
                                      cryoRMin,
                                      cryoRMax,
                                      h/2.,
                                      0*degree, 360*degree);
  G4Sphere* cryostatBottom = new G4Sphere("cryostatBottom",
                                          bowlRMin,
                                          bowlRMax,
                                          0*degree, 360*degree,
                                          (180-theta)*degree, 180*degree);
  G4UnionSolid* cryostatODSolid = new G4UnionSolid("CryostatODSolid", 
                                      cryostatBottom, 
                                      cryostatMiddle,
                                      new G4RotationMatrix(0,0,0),
                                      G4ThreeVector(0,0,(-1*x-1.0)*cm));
  //cryostatODSolid->DumpInfo();
  G4Material* air = materialManager->FindMaterial("Air");
  if (!air) std::cout <<"asdfasfasfHEYYYY\n";
  fCryostatODLV = new G4LogicalVolume(cryostatODSolid,
                                      materialManager->FindMaterial("Air"),
                                      "CryostatODLV");                                    ;

//  new G4PVPlacement(0, G4ThreeVector(), fCryostatODLV, "CryostatODLV", volWorld, false, 1); 
  
                                  
  /*G4LogicalVolume* middleLV = new G4LogicalVolume(cryostatMiddle,
                                      materialManager->FindMaterial("G10"),
                                     "MiddleLV");                                    
  G4LogicalVolume* bottomLV = new G4LogicalVolume(cryostatBottom,
                                      materialManager->FindMaterial("G10"),
                                     "BottomLV");
  *///new G4PVPlacement(0, G4ThreeVector(0,0,(-1*x-1)*cm), middleLV, "temp1", volWorld, false, 1);
  //new G4PVPlacement(0, G4ThreeVector(), bottomLV, "temp2", volWorld, false, 1); 

  //G4PhysicalVolumeStore* thePVStore = G4PhysicalVolumeStore::GetInstance();
//  if((*thePVStore)[1]->CheckOverlaps(1000,0.,1)) std::cout << "asdf\n";
 // std::cout << (*thePVStore)[1]->GetName() << std::endl;
}

}
