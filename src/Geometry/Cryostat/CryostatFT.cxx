// 
// File: CryostatFT.cxx
//
// Author: Hunter Sullivan
//

#include "Geometry/Cryostat/CryostatFT.h"
#include "Geometry/Auxiliary/Feedthrough.h"
#include "Configuration.h"
#include "MaterialManager.h"
#include "Utilities.h"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4LogicalVolumeStore.hh"

namespace geo
{

CryostatFT::CryostatFT()
{}

CryostatFT::~CryostatFT()
{}

void CryostatFT::ConstructVolume()
{
  // NEEDS WORK!
  ConstructSubVolumes();
  PlaceSubVolumes();
}

void CryostatFT::ConstructSubVolumes()
{
  // Get material manager and config
  MaterialManager* matMan       = MaterialManager::Instance();
  Configuration* config         = Configuration::Instance();
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();

  // Get the parameters
  std::vector<G4double> moduleMedFTDim = config->ModuleMedFTDim();        
  G4double innerR = moduleMedFTDim[0];
  G4double outerR = moduleMedFTDim[1];
  G4double height = moduleMedFTDim[2];
  G4double moduleTopWallThickness      = config->ModuleTopWallThickness(); 
  G4Box* volModuleWall = (G4Box*)lvStore->GetVolume("volModuleWall")->GetSolid();

  // Top wall 
  G4Box* solModuleTopWall = new G4Box("solModuleTopWall",
                                       volModuleWall->GetXHalfLength(),
                                       volModuleWall->GetZHalfLength(),
                                       moduleTopWallThickness/2. );
  fVolModuleTopWall = new G4LogicalVolume(solModuleTopWall,
                                           matMan->FindMaterial("Steel"),
                                           "volModuleTopWall");

  // Container for feedthroughs
  G4Box* solModuleFTContainer = new G4Box("solModuleFTContainer",
                                           solModuleTopWall->GetXHalfLength(),
                                           solModuleTopWall->GetYHalfLength(),
                                           height/2.);
  fVolModuleFTContainer = new G4LogicalVolume(solModuleFTContainer,
                                              matMan->FindMaterial("Air"),
                                              "volModuleFTContainer");
  
  // Medium sized feethrough
  Feedthrough ft;
  fVolModuleMedFT = ft.ConstructVolume("ModuleFT", innerR, outerR, 2*solModuleFTContainer->GetZHalfLength(), "Steel");

  // Small feedthrough
  fVolModuleSmallFT = ft.ConstructVolume("ModuleSmallFT", 3*cm, 3.5*cm, 2*solModuleFTContainer->GetZHalfLength(), "Steel");

  // Container for wall and FTs
  G4Box* solModuleFlange = new G4Box("solModuleFlange", 
                                      solModuleFTContainer->GetXHalfLength(),
                                      solModuleFTContainer->GetYHalfLength(),
                                      solModuleFTContainer->GetZHalfLength()+solModuleTopWall->GetZHalfLength());
  fVolModuleFlange = new G4LogicalVolume(solModuleFlange,
                                        matMan->FindMaterial("Air"),
                                        "volModuleFlange"); 

  // Container for all module walls and fts
  G4Box* solModContainer = (G4Box*)lvStore->GetVolume("volModuleContainer")->GetSolid();
  G4Box* solTopContainer = new G4Box("solTopContainer",
                                      solModContainer->GetXHalfLength(),
                                      solModContainer->GetZHalfLength(),
                                      solModuleFlange->GetZHalfLength());
  fVolTopContainer = new G4LogicalVolume(solTopContainer,
                                         matMan->FindMaterial("Air"),
                                         "volTopContainer");

  fHeight = 2*solTopContainer->GetZHalfLength();
}

void CryostatFT::PlaceSubVolumes()
{
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  Configuration* config = Configuration::Instance();
  arcutil::Utilities util;
  G4Box* volModuleWall = (G4Box*)lvStore->GetVolume("volModuleWall")->GetSolid();

  // Place FTs in FT container
  // loosely positioned here
  G4double xBound = volModuleWall->GetXHalfLength();
  G4double yBound = volModuleWall->GetZHalfLength();
  G4double x1 = xBound - 12*cm;
  G4double x2 = x1 - 6*cm;
  G4double z2 = x1;
  G4RotationMatrix* xRot2 = new G4RotationMatrix;
  xRot2->rotateX(pi/2);
  new G4PVPlacement(0, G4ThreeVector(x1,0,0),   fVolModuleMedFT, fVolModuleMedFT->GetName()+"_pos1", fVolModuleFTContainer, false, 0);
  new G4PVPlacement(0, G4ThreeVector(-x1,0,0),  fVolModuleMedFT, fVolModuleMedFT->GetName()+"_pos2", fVolModuleFTContainer, false, 1);
  new G4PVPlacement(0, G4ThreeVector(x2,z2,0),  fVolModuleMedFT, fVolModuleMedFT->GetName()+"_pos3", fVolModuleFTContainer, false, 2);
  new G4PVPlacement(0, G4ThreeVector(-x2,z2,0), fVolModuleMedFT, fVolModuleMedFT->GetName()+"_pos4", fVolModuleFTContainer, false, 3);
  new G4PVPlacement(0, G4ThreeVector(0,-x1,0),  fVolModuleMedFT, fVolModuleMedFT->GetName()+"_pos4", fVolModuleFTContainer, false, 4);

  new G4PVPlacement(0, G4ThreeVector(0,z2,0), fVolModuleSmallFT, fVolModuleSmallFT->GetName()+"_pos1", fVolModuleFTContainer, false, 0);

  // Place top wall and FT container in Module flange
  std::vector<G4LogicalVolume*> geoms = {fVolModuleTopWall,
                                         fVolModuleFTContainer};
  std::vector<G4double> geomsDim = {((G4Box*)geoms[0]->GetSolid())->GetZHalfLength(),
                                    ((G4Box*)geoms[1]->GetSolid())->GetZHalfLength() };


  G4double zLen = ((G4Box*)fVolModuleFlange->GetSolid())->GetZHalfLength();
  std::vector<G4double> steps = util.Stack(geomsDim, zLen);
  std::vector<G4ThreeVector> positions;
  positions.resize(steps.size());
  for (unsigned s = 0; s < steps.size(); s++) positions[s] = G4ThreeVector(0,0,steps[s]); 
  util.Place(geoms, positions, fVolModuleFlange);
  
  // Place module flanges in larger container
  G4double modInsideGap = config->ModuleClearance();
  G4Box* solModContainer = (G4Box*)lvStore->GetVolume("volModuleContainer")->GetSolid();
  G4Box* solMod          = (G4Box*)lvStore->GetVolume("volModule")->GetSolid();
  G4double boundZ = -1*solModContainer->GetZHalfLength();
  G4double boundX = -1*solModContainer->GetXHalfLength();

  std::vector<G4double> stepsX = {boundX + solMod->GetXHalfLength(),
                                  boundX + 3*solMod->GetXHalfLength() + modInsideGap};
  std::vector<G4double> stepsZ = {boundZ + solMod->GetZHalfLength(),
                                  boundZ + 3*solMod->GetZHalfLength() + modInsideGap};

  zLen = ((G4Tubs*)fVolTopContainer->GetSolid())->GetZHalfLength();
  positions = {G4ThreeVector(stepsX[0], stepsZ[0], zLen),
               G4ThreeVector(stepsX[0], stepsZ[1], zLen),
               G4ThreeVector(stepsX[1], stepsZ[0], zLen),
               G4ThreeVector(stepsX[1], stepsZ[1], zLen)};

  G4RotationMatrix* xRot3 = new G4RotationMatrix;
  xRot3->rotateX(3*pi/2.);
  new G4PVPlacement(0, positions[0], fVolModuleFlange, fVolModuleFlange->GetName()+"_pos1", fVolTopContainer, false, 1);
  new G4PVPlacement(0, positions[1], fVolModuleFlange, fVolModuleFlange->GetName()+"_pos2", fVolTopContainer, false, 2);
  new G4PVPlacement(0, positions[2], fVolModuleFlange, fVolModuleFlange->GetName()+"_pos3", fVolTopContainer, false, 3);
  new G4PVPlacement(0, positions[3], fVolModuleFlange, fVolModuleFlange->GetName()+"_pos4", fVolTopContainer, false, 4);
}


}
