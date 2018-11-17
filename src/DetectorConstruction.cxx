// 
// File: DetectorConstruction.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to construct our detector.
//

#include "DetectorConstruction.h"

namespace majorana
{

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{}

G4PhysicalVolume* DetectorConstruction::Construct()
{
  // Construct materials
  ConstructMaterials();

  // Construct the world 
  G4LogicalVolume* worldLogicalVol = ConstructLogicalWorld();

  // Define the world volume
  G4Double worldX = ;
  G4ThreeVector worldPos = {0,0,0};
  G4Material* worldMaterial = nistManager->FindOrBuildMaterial("G4_AIR");

  G4Box* worldSolid                   = new G4Box("World", 0.5*worldX, 0.5*worldX, 0.5*worldX);
  G4LogicalVolume* worldLogicalVol    = new G4LogicalVolume(worldSolid, worldMaterial, "World");
  G4VPhysicalVolume* worldPhysicalVol = new G4PVPlacement(0, worldPos, worldLogicalVol, "World", 0, false, 0, true);

  // Define the disk volume
  G4Double worldX = ;
  G4ThreeVector diskPos = {0,0,0};
  G4Material* diskMaterial = nistManager->FindOrBuildMaterial("G4_AIR");

  G4Box* worldSolid                   = new G4Box("World", 0.5*worldX, 0.5*worldX, 0.5*worldX);
  G4LogicalVolume* worldLogicalVol    = new G4LogicalVolume(worldSolid, worldMaterial, "World");
  G4VPhysicalVolume* worldPhysicalVol = new G4PVPlacement(0, worldPos, worldLogicalVol, "World", 0, false, 0, true);

 

}

void DetectorConstruction::ConstructMaterials()
{
  // Material manager
  G4NistManager* nistManager = G4NistMAnager::Instance();

  // Air 
  nistManager->FindOrBuildMaterial("G4_AIR");

  // We'l just use 
  
  /*// Acrylic (Taken from ultre_detector_construction)
  density = 1.19*g/cm3;
  G4Material* acrylic = new G4Material(name="Acrylic", density, nel=3);
  acrylic->AddElement(elC, 5);
  acrylic->AddElement(elH, 8);
  acrylic->AddElement(elO, 2);

  // ACRYLIC Optical properties 
  // Refractive index 
  const G4int NENTRIES = 11 ;
  G4double LAMBDA_ACRYLIC[NENTRIES]; 
  G4double RINDEX_ACRYLIC[NENTRIES];
  G4double ENERGY_ACRYLIC[NENTRIES];

  // Parameterization for refractive index of High Grade PMMA 
  G4double bParam[4] = {1760.7010,-1.3687,2.4388e-3,-1.5178e-6} ; 
  for(G4int i=0;i<NENTRIES; i++)
  {
    LAMBDA_ACRYLIC[i] = lambda_min + i*(lambda_max-lambda_min)/float(NENTRIES-1) ;
    RINDEX_ACRYLIC[i] = 0.0 ;
    for (G4int jj=0 ; jj<4 ; jj++)
    {
      RINDEX_ACRYLIC[i] +=  (bParam[jj]/1000.0)*std::pow(LAMBDA_ACRYLIC[i]/nm,jj) ; 
    }
    
    ENERGY_ACRYLIC[i] =   h_Planck*c_light/LAMBDA_ACRYLIC[i] ;  // Convert from wavelength to energy ;
    //  G4cout << ENERGY_ACRYLIC[i]/eV << " " << LAMBDA_ACRYLIC[i]/nm << " " << RINDEX_ACRYLIC[i] << G4endl ;
  }
  G4MaterialPropertiesTable *MPT_Acrylic = new G4MaterialPropertiesTable();
  MPT_Acrylic->AddProperty("RINDEX", ENERGY_ACRYLIC, RINDEX_ACRYLIC, NENTRIES);
  
  // Absorption
  const G4int NENT = 25;
  G4double LAMBDAABS[NENT] = 
  {
    100.0,
    246.528671, 260.605103, 263.853516, 266.019104, 268.726105,    
    271.433136, 273.598724, 276.305725, 279.554138, 300.127380,    
    320.159241, 340.191101, 360.764343, 381.337585, 399.745239,    
    421.401276, 440.891724, 460.382172, 480.414001, 500.987274,    
    520.477722, 540.509583, 559.458618,
    700.0    
  };
  
  G4double ABS[NENT] =   // Transmission (in %) of  3mm thick PMMA 
  { 
    0.0000000,
    0.0000000,  5.295952,  9.657321, 19.937695, 29.283491, 
    39.252335, 48.598133, 58.255451, 65.109039, 79.439247,
    85.669785, 89.719627, 91.277260, 91.588783, 91.900307,
    91.588783, 91.277260, 91.277260, 91.588783, 91.588783,
    91.900307, 91.900307, 91.588783,
    91.5
  };

  MPT_Acrylic->AddProperty("ABSLENGTH", new G4MaterialPropertyVector());
  for(G4int i=0;i<NENT; i++)
  {
    G4double energy = h_Planck*c_light/(LAMBDAABS[i]*nm);
    G4double abslength ;
    if (ABS[i] <= 0.0) abslength = 1.0/kInfinity;
    else abslength = -3.0*mm/(std::log(ABS[i]/100.0)) ;
    
    MPT_Acrylic->AddEntry("ABSLENGTH", energy, abslength);
  }
  
  Acrylic->SetMaterialPropertiesTable(MPT_Acrylic);
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;*/
}
}
