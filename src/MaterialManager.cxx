// 
// File: MaterialManager.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to construct detector materials.
//

#include "MaterialManager.h"

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"

namespace geo
{

MaterialManager* MaterialManager::instance = 0;

MaterialManager* MaterialManager::Instance()
{
  if (instance == 0) 
  {
    static MaterialManager manager;
    instance = &manager;
  }
  return instance;
}

MaterialManager::MaterialManager()
 : fAir(NULL),
   fAcrylic(NULL),
   fLAr(NULL)
{}

MaterialManager::~MaterialManager()
{}

void MaterialManager::ConstructMaterials()
{
  // Define the tpb emission spectrum
  fTPBEmissionE.resize(15, 0);
  fTPBEmissionE = { 0.05,    1.0,   1.5,  2.25, 2.481, 
                    2.819,  2.952, 2.988, 3.024,   3.1, 
                     3.14, 3.1807,  3.54,   5.5, 50.39};
  fTPBEmissionSpect.resize(15, 0);
  fTPBEmissionSpect = {   0.0,   0.0, 0.0, 0.0588, 0.235, 
                         0.853,   1.0, 1.0, 0.9259, 0.704, 
                        0.0296, 0.011, 0.0,    0.0, 0.0};
  // Seperate the implentation here
  DefineAir();
  DefineAcrylic();
  DefineLAr();

}

void MaterialManager::DefineLAr()
{
  G4NistManager* man = G4NistManager::Instance();
  fLAr = man->FindOrBuildMaterial("G4_lAr");

}
void MaterialManager::DefineAir()
{ 
  G4NistManager* man = G4NistManager::Instance();
  fAir = man->FindOrBuildMaterial("G4_AIR");
 
  // Optical properties 
  const G4int nEntries = fTPBEmissionE.size();
  G4double photonEnergy[nEntries];
  G4double airRIndex[nEntries];
  G4double airSpect[nEntries];
  G4double airAbs[nEntries];
  G4double airRIndexImg[nEntries];
  G4double airRefl[nEntries];
  for (int i = 0; i < nEntries; i++) 
  {
    photonEnergy[i] = fTPBEmissionE[i]*eV;
    airRIndex[i]    = 1.0;
    airRIndexImg[i] = 0.0;
    airRefl[i]       = 0.97;
  }

  G4MaterialPropertiesTable *airMPT = new G4MaterialPropertiesTable();
  airMPT->AddProperty("RINDEX", photonEnergy, airRIndex, nEntries);
  airMPT->AddProperty("REALRINDEX", photonEnergy, airRIndex, nEntries);
  airMPT->AddProperty("IMAGINARYRINDEX", photonEnergy, airRIndexImg, nEntries);
  // For border surface
  // Assuming 97% reflectivity
  airMPT->AddProperty("REFLECTIVITY", photonEnergy, airRefl, nEntries);
  fAir->SetMaterialPropertiesTable(airMPT);
}

void MaterialManager::DefineAcrylic()
{
  //Define elements 
  G4int nel;
  G4double a, density, z;
  G4String name, symbol;
  a = 1.01*g/mole;
  G4Element* elH  = new G4Element(name="Hydrogen", symbol="H", z=1., a); 
  a = 12.01*g/mole;
  G4Element* elC  = new G4Element(name="Carbon",   symbol="C", z=6., a);
  a = 14.01*g/mole;
  G4Element* elN  = new G4Element(name="Nitrogen", symbol="N", z=7., a);
  a = 16.00*g/mole;
  G4Element* elO  = new G4Element(name="Oxygen",   symbol="O", z=8., a);

  density = 1.19*g/cm3;
  fAcrylic = new G4Material(name="Acrylic", density, nel=3);
  fAcrylic->AddElement(elC, 5);
  fAcrylic->AddElement(elH, 8);
  fAcrylic->AddElement(elO, 2);

  // Optical properties 
  // TPB Emission: LArSoft package (lardataalg)
  // Refractive index: arXiv:1101.3013v1
  // Absorption length --> infinity: ref therein arXiv:1307.6906v2
  const G4int nEntries = fTPBEmissionE.size();
  G4double photonEnergy[nEntries];
  G4double acrylicRIndex[nEntries];
  G4double acrylicRIndexImg[nEntries];
  G4double acrylicSpect[nEntries];
  G4double acrylicAbs[nEntries];
  for (int i = 0; i < nEntries; i++) 
  {
    photonEnergy[i]     = fTPBEmissionE[i]*eV;
    acrylicRIndex[i]    = 1.49;
    acrylicRIndexImg[i] = 0.0;
    acrylicSpect[i]     = fTPBEmissionSpect[i]; 
    acrylicAbs[i]       = 100*m;
  }

  G4MaterialPropertiesTable *acrylicMPT = new G4MaterialPropertiesTable();
  acrylicMPT->AddProperty("RINDEX",          photonEnergy, acrylicRIndex,    nEntries);
  acrylicMPT->AddProperty("REALRINDEX",      photonEnergy, acrylicRIndex,    nEntries);
  acrylicMPT->AddProperty("IMAGINARYRINDEX", photonEnergy, acrylicRIndexImg, nEntries);
  acrylicMPT->AddProperty("ABSLENGTH",       photonEnergy, acrylicAbs,       nEntries);
  acrylicMPT->AddProperty("EMISSIONSPECT",   photonEnergy, acrylicSpect,     nEntries);

  fAcrylic->SetMaterialPropertiesTable(acrylicMPT);
}

G4Material* MaterialManager::FindMaterial(const G4String& name)
{
  G4Material* material = G4Material::GetMaterial(name, true);
  return material;
}

}
