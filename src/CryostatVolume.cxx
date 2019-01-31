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

#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"

namespace geo
{

Cryostat::Cryostat()
:  fCryostatODSolid(NULL),
   fCryostatODLV(NULL)
{}

Cryostat::~Cryostat()
{}

void Cryostat::ConstructVolume(G4VPhysicalVolume* worldPV,
                                     G4LogicalVolume*   worldLV)
{

  //****
  // Construct cryostat volumes
  //****
  ConstructCryostat(worldLV);
   
  // Handle vis effects
  HandleVisAttributes();
}

void Cryostat::ConstructCryostat(G4LogicalVolume* worldLV)
{
  // Get material manager and config
  MaterialManager* materialManager = MaterialManager::Instance();
  if (!materialManager) std::cout << "HEYYYYYY\n";
  Configuration* config = Configuration::Instance();
  
  //**** 
  // 	Cryostat shape
  //****
  // Expecting radius, depth, and wall thickness
  // A bit of of hard-coding
  std::vector<double> cryostatDimOD = config->CryostatDimensions();
  double cryoRMax = cryostatDimOD[0];
  double cryoRMin = cryoRMax - cryostatDimOD[2];
  double bowlRMax = 2*cryoRMax;
  double bowlRMin = bowlRMax - cryostatDimOD[2];
  double depth    = cryostatDimOD[1];
  double sinTheta = cryoRMax/bowlRMax;
  double cosTheta = std::sqrt(1 - sinTheta*sinTheta);
  double y        = bowlRMax*(1 - cosTheta);
  double h        = depth - y;
  double x        = bowlRMax + 0.5*h - depth;
  double theta    = std::abs(std::asin(sinTheta)*180/pi);

  std::cout << y << " " << h << " " << x << " " << depth << std::endl;
 
  G4Tubs* cryostatMiddle = new G4Tubs("cryostatMiddle",
                                      cryoRMin*cm,
                                      cryoRMax*cm,
                                      (h/2.)*cm,
                                      0*degree, 360*degree);
  G4Sphere* cryostatBottom = new G4Sphere("cryostatBottom",
                                          bowlRMin*cm,
                                          bowlRMax*cm,
                                          0*degree, 360*degree,
                                          (180-theta)*degree, 180*degree);
  fCryostatODSolid = new G4UnionSolid("CryostatODSolid", 
                                      cryostatBottom, 
                                      cryostatMiddle,
                                      new G4RotationMatrix(0,0,0),
                                      G4ThreeVector(0,0,(-1*x-1.0)*cm));
  //fCryostatODSolid->DumpInfo();
  G4Material* air = materialManager->FindMaterial("Air");
  if (!air) std::cout <<"asdfasfasfHEYYYY\n";
  fCryostatODLV = new G4LogicalVolume(fCryostatODSolid,
                                      materialManager->FindMaterial("Air"),
                                      "CryostatODLV");                                    ;

//  new G4PVPlacement(0, G4ThreeVector(), fCryostatODLV, "CryostatODLV", worldLV, false, 1); 
  
                                  
  /*G4LogicalVolume* middleLV = new G4LogicalVolume(cryostatMiddle,
                                      materialManager->FindMaterial("G10"),
                                     "MiddleLV");                                    
  G4LogicalVolume* bottomLV = new G4LogicalVolume(cryostatBottom,
                                      materialManager->FindMaterial("G10"),
                                     "BottomLV");
  *///new G4PVPlacement(0, G4ThreeVector(0,0,(-1*x-1)*cm), middleLV, "temp1", worldLV, false, 1);
  //new G4PVPlacement(0, G4ThreeVector(), bottomLV, "temp2", worldLV, false, 1); 

  //G4PhysicalVolumeStore* thePVStore = G4PhysicalVolumeStore::GetInstance();
//  if((*thePVStore)[1]->CheckOverlaps(1000,0.,1)) std::cout << "asdf\n";
 // std::cout << (*thePVStore)[1]->GetName() << std::endl;
}

void Cryostat::HandleVisAttributes()
{
  // Cryostat
  G4VisAttributes* cryoVA = new G4VisAttributes(G4Colour(1,1,1));
  cryoVA->SetForceWireframe(false);
  fCryostatODLV->SetVisAttributes(cryoVA);
}

}
