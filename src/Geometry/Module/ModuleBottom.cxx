// 
// File: ModuleBottom.cxx
//
// Author: Hunter Sullivan
//

#include "Geometry/Module/ModuleBottom.h"
#include "QuantityStore.h"
#include "MaterialManager.h"
#include "Utilities.h"

#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolumeStore.hh"

namespace geo
{

ModuleBottom::ModuleBottom()
{}

ModuleBottom::~ModuleBottom()
{}

void ModuleBottom::ConstructVolume()
{
  ConstructSubVolumes();
  PlaceSubVolumes();
}

void ModuleBottom::ConstructSubVolumes()
{
  MaterialManager* matMan = MaterialManager::Instance();
  QuantityStore*   qStore = QuantityStore::Instance();

  // Get variables
  std::vector<G4double> modLegDim      = qStore->kModuleLegDim;         
  std::vector<G4double> modLegFootDim  = qStore->kModuleLegFootDim;     
  std::vector<G4double> dummyFlangeDim = qStore->kBottomDummyFlangeDim; 
  std::vector<G4double> legPosition    = qStore->kModuleLegPosition;    
 
  // Legs
  G4Box* solModuleLegShin = new G4Box("solModuleLegShin", 
                                      (modLegDim[0]/2.), 
                                      (modLegDim[1]/2.), 
                                      (modLegDim[2]/2.));
  G4Box* solModuleLegFoot = new G4Box("solModuleLegFoot", 
                                      (modLegFootDim[0]/2.), 
                                      (modLegFootDim[1]/2.), 
                                      (modLegFootDim[2]/2.)); 
  G4ThreeVector transl1(0, solModuleLegShin->GetYHalfLength()-solModuleLegFoot->GetYHalfLength(), 0);
  G4UnionSolid* solModuleLegTemp = new G4UnionSolid("solModuleLegTemp", 
                                                     solModuleLegShin, 
                                                     solModuleLegFoot, 
                                                     0, 
                                                     transl1);
  G4UnionSolid* solModuleLeg = new G4UnionSolid("solModuleLeg", 
                                                 solModuleLegTemp, 
                                                 solModuleLegFoot, 
                                                 0, 
                                                 -1*transl1);                                                    
  fVolModuleLeg = new G4LogicalVolume(solModuleLeg, 
                                      matMan->FindMaterial("FR4"),
                                      "volModuleLeg");  

  // Place all four legs in container
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  G4Box* solActiveMod = (G4Box*)lvStore->GetVolume("volActiveModule")->GetSolid();
  G4Box* solModuleLegContainer = new G4Box("solModuleLegContainer",
                                            solActiveMod->GetXHalfLength(),
                                             (modLegDim[1]/2.),
                                            solActiveMod->GetZHalfLength());
  fVolModuleLegContainer = new G4LogicalVolume(solModuleLegContainer,
	                                             matMan->FindMaterial("LAr"),
	                                            "volModuleLegContainer");

  G4ThreeVector pos(legPosition[0], 0, legPosition[2]);
  new G4PVPlacement(0, pos, fVolModuleLeg, fVolModuleLeg->GetName(), fVolModuleLegContainer, false, 0);

  pos = {-1*legPosition[0], 0, legPosition[2]};
  new G4PVPlacement(0, pos, fVolModuleLeg, fVolModuleLeg->GetName(), fVolModuleLegContainer, false, 1);

  pos = {-1*legPosition[0], 0, -1*legPosition[2]};
  new G4PVPlacement(0, pos, fVolModuleLeg, fVolModuleLeg->GetName(), fVolModuleLegContainer, false, 2);

  pos = {legPosition[0], 0, -1*legPosition[2]};
  new G4PVPlacement(0, pos, fVolModuleLeg, fVolModuleLeg->GetName(), fVolModuleLegContainer, false, 3);                                      

  // Dummy flange
  G4Box* solBottomDummyFlange = new G4Box("solBottomDummyFlange", 
                                          solModuleLegContainer->GetXHalfLength(), 
                                          (dummyFlangeDim[1]/2.), 
                                          solModuleLegContainer->GetZHalfLength());
  fVolBottomDummyFlange = new G4LogicalVolume(solBottomDummyFlange, 
                                              matMan->FindMaterial("SSteel304"),
                                              "volBottomDummyFlange");

  // Bottom volume
  G4Box* solModuleBottom = new G4Box("solModuleBottom",
                                      solModuleLegContainer->GetXHalfLength(),
                                      solModuleLegContainer->GetYHalfLength()+solBottomDummyFlange->GetYHalfLength(),
                                      solModuleLegContainer->GetZHalfLength());
  fVolModuleBottom = new G4LogicalVolume(solModuleBottom,
                                         matMan->FindMaterial("LAr"),
                                         "volModuleBottom");
}

void ModuleBottom::PlaceSubVolumes()
{
  std::vector<G4LogicalVolume*>  geoms;
  std::vector<G4double>          geomsDim;
  std::vector<G4double>          steps;
  std::vector<G4ThreeVector>     positions;
  std::vector<G4RotationMatrix*> rotations;
  arcutil::Utilities util;

  geoms    = { fVolBottomDummyFlange, 
               fVolModuleLegContainer };
  geomsDim = { ((G4Box*)geoms[0]->GetSolid())->GetYHalfLength(),
               ((G4Box*)geoms[1]->GetSolid())->GetYHalfLength() };
  
  G4double yLen = geomsDim[0]+geomsDim[1];
  steps = util.Stack(geomsDim, yLen);

  positions.resize(steps.size());
  rotations.resize(steps.size(), 0);

  for (unsigned s = 0; s < steps.size(); s++) positions[s] = G4ThreeVector(0,steps[s],0); 

  
  util.Place(geoms, positions, rotations, fVolModuleBottom);
}
}
