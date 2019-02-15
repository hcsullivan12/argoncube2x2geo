// 
// File: QuantityStore.h
//
// Author: Hunter Sullivan
//

#ifndef QUANTITYSTORE_H
#define QUANTITYSTORE_H

#include <string>
#include <vector>

#include "G4Types.hh"

namespace geo
{

class QuantityStore
{
  public:
    static QuantityStore* Instance();
    ~QuantityStore();
 
    void Initialize();

  private:
    QuantityStore();
    static QuantityStore* instance;

  public:
    G4double      kModuleWallThickness;
    G4double      kModuleClearance;
    G4double      kFieldShellThickness;
    G4double      kCathodeThickness;
    G4double      kPixelPlaneThickness;
    G4double      kPixelSpacing;
    G4double      kPixelRadius;
    G4double      kModuleTopWallThickness;
    G4double      kCryostatLegOffset;
    std::vector<G4double> kWorldDim;
    std::vector<G4double> kModuleDim;
    std::vector<G4double> kCryostatDim;
    std::vector<G4double> kCryostatLegDim;
    std::vector<G4double> kCryostatLegFootDim;
    std::vector<G4double> kCryostatFlangeDim;
    std::vector<G4double> kActiveLArDim;
    std::vector<G4double> kLightDetDim;
    std::vector<G4double> kModuleLegDim;
    std::vector<G4double> kModuleLegFootDim;
    std::vector<G4double> kModuleMedFTDim;
    std::vector<G4double> kBottomDummyFlangeDim;
    std::vector<G4double> kModuleLegPosition;
    std::vector<G4double> kTopLArDim;
    std::vector<G4double> kTopGArDim;

    // Variable stored later
    G4double kCryoOuterWallTubDepth;
    G4double kCryoDepth; 
    G4double kCryoOuterWallR;
    G4double kCryoLegHeight;   
    G4double kCryoLegShinR;
    G4double kCryoFTHeight;     

    G4double kModuleMiddleFrameYHalfL;
    G4double kTopModuleYHalfL;    
};
}

#endif
