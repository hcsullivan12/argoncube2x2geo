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

namespace majorana
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
{}

MaterialManager::~MaterialManager()
{}

void MaterialManager::ConstructMaterials()
{
  // Define the tpb emission spectrum
  m_tpbEmissionE.resize(15, 0);
  m_tpbEmissionE = { 0.05,    1.0,   1.5,  2.25, 2.481, 
                    2.819,  2.952, 2.988, 3.024,   3.1, 
                     3.14, 3.1807,  3.54,   5.5, 50.39};
  m_tpbEmissionSpect.resize(15, 0);
  m_tpbEmissionSpect = {   0.0,   0.0, 0.0, 0.0588, 0.235, 
                         0.853,   1.0, 1.0, 0.9259, 0.704, 
                        0.0296, 0.011, 0.0,    0.0, 0.0};
  // Seperate the implentation here
  DefineAir();
  DefineAcrylic();
  DefineMPPCSurface();;

  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

void MaterialManager::DefineAir()
{
  G4NistManager* man = G4NistManager::Instance();
  //man->SetVerbose(1);
  m_air = man->FindOrBuildMaterial("G4_AIR");
  G4Material* m_al  = man->FindOrBuildMaterial("G4_Al");
  
  // Optical properties 
  const G4int nEntries = m_tpbEmissionE.size();
  G4double photonEnergy[nEntries];
  G4double airRIndex[nEntries];
  G4double airSpect[nEntries];
  G4double airAbs[nEntries];
  G4double airRIndexImg[nEntries];
  for (int i = 0; i < nEntries; i++) 
  {
    photonEnergy[i]  = m_tpbEmissionE[i]*eV;
    airRIndex[i] = 1.0;
    airRIndexImg[i] = 0.0;
  }

  G4MaterialPropertiesTable *airMPT = new G4MaterialPropertiesTable();
  airMPT->AddProperty("RINDEX", photonEnergy, airRIndex, nEntries);
  airMPT->AddProperty("REALRINDEX", photonEnergy, airRIndex, nEntries);
  airMPT->AddProperty("IMAGINARYRINDEX", photonEnergy, airRIndexImg, nEntries);

  m_air->SetMaterialPropertiesTable(airMPT);
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
  m_acrylic = new G4Material(name="Acrylic", density, nel=3);
  m_acrylic->AddElement(elC, 5);
  m_acrylic->AddElement(elH, 8);
  m_acrylic->AddElement(elO, 2);

  // Optical properties 
  // TPB Emission: LArSoft package (lardataalg)
  // Refractive index: arXiv:1101.3013v1
  // Absorption length --> infinity: ref therein arXiv:1307.6906v2
  const G4int nEntries = m_tpbEmissionE.size();
  G4double photonEnergy[nEntries];
  G4double acrylicRIndex[nEntries];
  G4double acrylicRIndexImg[nEntries];
  G4double acrylicSpect[nEntries];
  G4double acrylicAbs[nEntries];
  for (int i = 0; i < nEntries; i++) 
  {
    photonEnergy[i]     = m_tpbEmissionE[i]*eV;
    acrylicRIndex[i]    = 1.49;
    acrylicRIndexImg[i] = 0.0;
    acrylicSpect[i]     = m_tpbEmissionSpect[i]; 
    acrylicAbs[i]       = 100*m;
  }

  G4MaterialPropertiesTable *acrylicMPT = new G4MaterialPropertiesTable();
  acrylicMPT->AddProperty("RINDEX",          photonEnergy, acrylicRIndex,    nEntries);
  acrylicMPT->AddProperty("REALRINDEX",      photonEnergy, acrylicRIndex,    nEntries);
  acrylicMPT->AddProperty("IMIGINARYRINDEX", photonEnergy, acrylicRIndexImg, nEntries);
  acrylicMPT->AddProperty("ABSLENGTH",       photonEnergy, acrylicAbs,       nEntries);
  acrylicMPT->AddProperty("EMISSIONSPECT",   photonEnergy, acrylicSpect,     nEntries);

  m_acrylic->SetMaterialPropertiesTable(acrylicMPT);
}

G4Material* MaterialManager::FindMaterial(const G4String& name)
{
  G4Material* material = G4Material::GetMaterial(name, true);
  return material;
}

void MaterialManager::DefineMPPCSurface()
{
  /*// PhotonDet Surface Properties
  G4OpticalSurface* mppcDetSurface = new G4OpticalSurface("MPPCDetSurface",
                                                           glisur,
                                                           ground,
                                                           dielectric_metal,
                                                           fMPPCPolish);
  G4MaterialPropertiesTable* mppcDetSurfaceProperty =
                                                new G4MaterialPropertiesTable();
  G4double p_mppc[] = {2.00*eV, 3.47*eV};
  const G4int nbins = sizeof(p_mppc)/sizeof(G4double);
  G4double refl_mppc[] = {fMPPCReflectivity,fMPPCReflectivity};
  assert(sizeof(refl_mppc) == sizeof(p_mppc));
  G4double effi_mppc[] = {1, 1};
  assert(sizeof(effi_mppc) == sizeof(p_mppc));
 
  photonDetSurfaceProperty->AddProperty("REFLECTIVITY",p_mppc, refl_mppc, nbins);
  photonDetSurfaceProperty->AddProperty("EFFICIENCY",  p_mppc, effi_mppc, nbins);

  photonDetSurface->SetMaterialPropertiesTable(photonDetSurfaceProperty);
  new G4LogicalSkinSurface("PhotonDetSurface",logicPhotonDet,photonDetSurface);
*/
}

}
