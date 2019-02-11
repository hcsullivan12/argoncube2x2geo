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
{}

MaterialManager::~MaterialManager()
{}

void MaterialManager::ConstructMaterials()
{
  // based on Tyler https://github.com/tyleralion/duneggd
  G4Element* h  = new G4Element("hydrogen",   "H",  1,  1.00791*g/mole );
  G4Element* b  = new G4Element("boron",      "B",  5,  10.811*g/mole);
  G4Element* c  = new G4Element("carbon",     "C",  6,  12.0107*g/mole);
  G4Element* n  = new G4Element("nitrogen",   "N",  7,  14.0671*g/mole);
  G4Element* o  = new G4Element("oxygen",     "O",  8,  15.999*g/mole);
  G4Element* f  = new G4Element("fluorine",   "F",  9,  18.9984*g/mole);
  G4Element* na = new G4Element("sodium",     "Na", 11, 22.99*g/mole);
  G4Element* mg = new G4Element("magnesium",  "Mg", 12, 24.305*g/mole);
  G4Element* al = new G4Element("aluminum",   "Al", 13, 26.9815*g/mole);
  G4Element* si = new G4Element("silicon",    "Si", 14, 28.0855*g/mole);
  G4Element* p  = new G4Element("phosphorous", "P",  15, 30.973*g/mole);
  G4Element* s  = new G4Element("sulfur",     "S",  16, 32.065*g/mole);
  G4Element* ar = new G4Element("argon",      "Ar", 18, 39.948*g/mole);
  G4Element* k  = new G4Element("potassium",  "K",  19, 39.0983*g/mole);
  G4Element* ca = new G4Element("calcium",    "Ca", 20, 40.078*g/mole);
  G4Element* ti = new G4Element("titanium",   "Ti", 22, 47.867*g/mole);
  G4Element* cr = new G4Element("chromium",   "Cr", 24, 51.9961*g/mole);
  G4Element* mn = new G4Element("manganese",  "Mn", 25, 54.9380*g/mole);
  G4Element* fe = new G4Element("iron",       "Fe", 26, 55.8450*g/mole);
  G4Element* ni = new G4Element("nickel",     "Ni", 28, 58.6934*g/mole);


  G4Isotope* cu63 = new G4Isotope("copper63", 29, 63, 62.93*g/mole);
  G4Isotope* cu65 = new G4Isotope("copper65", 29, 65, 64.93*g/mole);
  G4Element* cu = new G4Element("copper", "Cu", 2);
  cu->AddIsotope(cu63, 0.6917);
  cu->AddIsotope(cu65,0.3083);
  
  G4Isotope* zn64 = new G4Isotope("zinc64", 30, 64, 63.93*g/mole);
  G4Element* zn = new G4Element("zinc", "Zn", 1);
  zn->AddIsotope(zn64, 1.0);  
  G4Element* pb = new G4Element("lead", "Pb", 82, 207.20*g/mole );  
  
  // Molecules for Rock 
  G4Material* SiO2  = new G4Material("SiO2",  2.2*g/cm3, 2);
  SiO2->AddElement(si, 1);
  SiO2->AddElement(o, 2);
  G4Material* FeO   = new G4Material("FeO",   5.745*g/cm3, 2);
  FeO->AddElement(fe, 1);
  FeO->AddElement(o, 1);
  G4Material* Al2O3 = new G4Material("Al2O3", 3.97*g/cm3, 2);
  Al2O3->AddElement(al, 2);
  Al2O3->AddElement(o, 3);
  G4Material* MgO   = new G4Material("MgO",   3.58*g/cm3, 2);
  MgO->AddElement(mg, 1);
  MgO->AddElement(o, 1);
  G4Material* CO2   = new G4Material("CO2",   1.562*g/cm3, 2);
  CO2->AddElement(c, 1);
  CO2->AddElement(o, 2);
  G4Material* CaO   = new G4Material("CaO",   3.35*g/cm3, 2);
  CaO->AddElement(ca, 1);
  CaO->AddElement(o, 1);
  G4Material* Na2O  = new G4Material("Na2O",  2.27*g/cm3, 2);
  Na2O->AddElement(na, 2);
  Na2O->AddElement(o, 1);
  G4Material* P2O5  = new G4Material("P2O5",  1.562*g/cm3, 2);
  P2O5->AddElement(p, 2);
  P2O5->AddElement(o, 5);
  G4Material* TiO2  = new G4Material("TiO2",  4.23*g/cm3,  2);
  TiO2->AddElement(ti, 1);
  TiO2->AddElement(o, 2);
  G4Material* Fe2O3 = new G4Material("Fe2O3", 5.24*g/cm3,  2);  
  Fe2O3->AddElement(fe, 2);
  Fe2O3->AddElement(o, 3);
  G4Material* rock  = new G4Material( "Rock", 2.82*g/cm3, 11);
  rock->AddMaterial(SiO2,   0.5267);
  rock->AddMaterial(FeO,    0.1174);
  rock->AddMaterial(Al2O3,  0.1025);
  rock->AddElement(o, 0.0771);
  rock->AddMaterial(MgO,    0.0473);
  rock->AddMaterial(CO2,    0.0422);
  rock->AddMaterial(CaO,    0.0382);
  rock->AddElement(c, 0.0240);
  rock->AddElement(s, 0.0186);
  rock->AddMaterial(Na2O,   0.0053);
  rock->AddMaterial(P2O5,   0.0007);
  G4Material* dirt  = new G4Material( "Dirt", 1.7*g/cm3, 6);
  dirt->AddElement(o,    0.438);
  dirt->AddElement(si,   0.257);
  dirt->AddElement(na,    0.222);
  dirt->AddElement(al,  0.049);
  dirt->AddElement(fe,      0.019);
  dirt->AddElement(k, 0.015);
  G4Material* air   = new G4Material( "Air", 0.001225*g/cm3, 3);
  air->AddElement(n, 0.781154);
  air->AddElement(o,   0.209476);
  air->AddElement(ar,  0.00934);
  
  // materials for the radiators and st planes following
  // WARNING! densities not right!
  G4Material* C3H6   = new G4Material("C3H6", 0.946*g/cm3, 2);
  C3H6->AddElement(c,   3);
  C3H6->AddElement(h, 6);

  G4double fracC3H6 = (25*0.946)/(25*0.946+125*0.001225);
  G4double dRad = 0.1586875*g/cm3;
  G4Material* RadBlend = new G4Material( "RadiatorBlend", dRad, 2);
  RadBlend->AddMaterial(air,  1-fracC3H6);
  RadBlend->AddMaterial(C3H6, fracC3H6);

  G4double densCO2 = 44.01/22.4*0.001; // molar mass / STP molar volume * conversion to g/cm3 from L
  G4double densAr  = 39.95/22.4*0.001;
  G4double densXe  = 131.3/22.4*0.001;
  G4double fracCO2 = .3;
  G4double densArCO2 = fracCO2 * densCO2 + (1-fracCO2) * densAr;
  G4double densXeCO2 = fracCO2 * densCO2 + (1-fracCO2) * densXe;
  G4double dArCO2 = densArCO2*g/cm3;
  G4double dXeCO2 = densXeCO2*g/cm3;
  
  // materials for the targets and st planes following
  G4Material* H2O      = new G4Material("Water",       1.0*g/cm3, 2);
  H2O->AddElement(o, 1);
  H2O->AddElement(h, 2);
  G4Material* ArTarget = new G4Material("ArgonTarget", 0.2297*g/cm3, 1); 
  ArTarget->AddElement(ar,1);
  G4Material* Aluminum = new G4Material("Aluminum",    2.70*g/cm3,  1); 
  Aluminum->AddElement(al,1);
  G4Material* Copper   = new G4Material("Copper",   8.96*g/cm3, 1); 
  Copper->AddElement(cu,1);
  G4Material* Kapton   = new G4Material("Kapton",   1.4*g/cm3, 3);  
  Kapton->AddElement(c, 22);
  Kapton->AddElement(o, 5);
  Kapton->AddElement(n, 2);
  G4Material* Iron     = new G4Material("Iron",     7.874*g/cm3, 1); 
  Iron->AddElement(fe,1);
  G4Material* Graphite = new G4Material("Graphite", 2.23*g/cm3, 1); 
  Graphite->AddElement(c,1);
  G4Material* Calcium  = new G4Material("Calcium",  1.55*g/cm3, 1); 
  Calcium->AddElement(ca,1);
  G4Material* Steel    = new G4Material( "Steel", 7.9300*g/cm3, 4);
  Steel->AddElement(fe,     0.7298);
  Steel->AddElement(cr, 0.1792);
  Steel->AddElement(ni,   0.0900);
  Steel->AddElement(c,   0.0010);
  G4Material* Polycate = new G4Material("polycate", 1.2*g/cm3, 3);
  Polycate->AddElement(c,16);
  Polycate->AddElement(h,6);
  Polycate->AddElement(o,3);

  // https://www.thebalance.com/type-304-and-304l-stainless-steel-2340261
  // fractional mass
  // density based on Table 8 DUNE-doc-6652-v5, jp
  G4Material* ssteel304 = new G4Material("SSteel304", 7.9*g/cm3, 9);
  ssteel304->AddElement(c,0.0008);
  ssteel304->AddElement(mg,0.02);
  ssteel304->AddElement(p,0.00045);
  ssteel304->AddElement(s,0.0003);
  ssteel304->AddElement(si,0.0075);
  ssteel304->AddElement(cr,0.18);
  ssteel304->AddElement(ni,0.08);
  ssteel304->AddElement(n,0.0010);
  ssteel304->AddElement(fe,0.70995);

  // https://www.azom.com/article.aspx?ArticleID=6117
  // density based on Table 8 DUNE-doc-6652-v5, jp
  G4Material* cSteel = new G4Material("cSteel", 7.9*g/cm3, 7);
  cSteel->AddElement(c,0.0030);
  cSteel->AddElement(cu,0.0025);
  cSteel->AddElement(fe,0.98);
  cSteel->AddElement(mg,0.0103);
  cSteel->AddElement(p,0.00090);
  cSteel->AddElement(si,0.00280);
  cSteel->AddElement(s,0.00050);

  // http://iti.northwestern.edu/cement/monograph/Monograph3_6.html
  // density based on Table 8 DUNE-doc-6652-v5, jp
  G4Material* reifConcrete = new G4Material("ReifConcrete", 2.5*g/cm3, 5);
  reifConcrete->AddMaterial(CaO,0.6661);
  reifConcrete->AddMaterial(SiO2,0.2345);
  reifConcrete->AddMaterial(Al2O3,0.0445);
  reifConcrete->AddMaterial(Fe2O3,0.0307);
  reifConcrete->AddMaterial(MgO,0.0242);

  // http://hepwww.rl.ac.uk/atlas-sct/engineering/material_budget/models/Endcap_Module/ATLAS_ECSCT_materials.pdf
  // table 6, fractional mass, jp
  G4Material* glass = new G4Material("Glass", 2.70*g/cm3, 8);
  glass->AddElement(si,0.2743);
  glass->AddElement(b,0.0166);
  glass->AddElement(al,0.0207);
  glass->AddElement(na,0.0449);
  glass->AddElement(k,0.0821);
  glass->AddElement(zn,0.0882);
  glass->AddElement(ti,0.0292);
  glass->AddElement(o,0.4440);

  // http://www.engineeringtoolbox.com/engineering-materials-properties-d_1225.html
  // http://hepwww.rl.ac.uk/atlas-sct/engineering/material_budget/models/Endcap_Module/ATLAS_ECSCT_materials.pdf
  // table 3 - C6 H6 O, jp
  G4Material* epoxy = new G4Material("Epoxy", 1.25*g/cm3, 3);
  epoxy->AddElement(c,6),
  epoxy->AddElement(h,6);
  epoxy->AddElement(o,1);

  // http://hepwww.rl.ac.uk/atlas-sct/engineering/material_budget/models/Endcap_Module/ATLAS_ECSCT_materials.pdf
  // table 6, fractional mass, jp
  // could be also G10
  G4Material* fr4 = new G4Material("FR4", 1.850*g/cm3, 2);
  fr4->AddMaterial(epoxy,0.206);
  fr4->AddMaterial(glass,0.794);

  // Radiation Physics and Chemistry 63 (2002) 89 92, jp
  // http://www.eljentechnology.com/products/wavelength-shifting-plastics/ej-280-ej-282-ej-284-ej-286?highlight=WyJwb2x5dmlueWx0b2x1ZW5lIl0=
  G4Material* pvt = new G4Material("PVT", 1.023*g/cm3, 2);
  pvt->AddElement(c,9);
  pvt->AddElement(h,10);

  // Scintillator:
  G4Material* Scintillator  = new G4Material("Scintillator", 1.05*g/cm3, 2);
  Scintillator->AddElement(c, 0.916);
  Scintillator->AddElement(h, 0.084);

  // Lead:
  G4Material* Lead  = new G4Material("Lead",  11.342*g/cm3, 1);
  Lead->AddElement(pb,1);

  // for LAr otion using this world:
  G4Material* LArTarget = new G4Material("LAr", 1.4*g/cm3, 1);
  LArTarget->AddElement(ar,1);
  G4Material* GArTarget = new G4Material("GAr", 1.784*0.001*g/cm3, 1);
  GArTarget->AddElement(ar,1);

  // Vacuum
  G4Material* vacuum =  new G4Material("Vacuum", 1.0E-25*g/cm3, 1);
  vacuum->AddMaterial(air, 1);
}

G4Material* MaterialManager::FindMaterial(const G4String& name)
{
  G4Material* material = G4Material::GetMaterial(name, true);
  return material;
}

}
