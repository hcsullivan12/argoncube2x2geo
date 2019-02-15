// File:        QuantityStore.cxx
// Author:      Hunter Sullivan
// Description: This store holds the parameters used in construction.
//              There are two sets of parameters here:
//                    1) Configuration parameters (intialized below)
//                    2) Dynamic parameters (initialized during construction)
//

#include "QuantityStore.h"

#include <iostream>
#include <iomanip>

#include "G4SystemOfUnits.hh"

#ifdef VERSION
#define version VERSION
#endif

namespace geo
{

QuantityStore* QuantityStore::instance = 0;

QuantityStore* QuantityStore::Instance()
{
  if (instance == 0)
  {
    static QuantityStore config;
    instance = &config;
  }
  return instance;
}

QuantityStore::QuantityStore()
{}

QuantityStore::~QuantityStore()
{}

void QuantityStore::Initialize()
{
  // World
  kWorldDim = {50*m, 50*m, 50*m};

  // Module
  // Many of the dimensions are derived from other volumes,
  // so zeros act as placeholders
  kModuleWallThickness     = 1.0*cm;
  kModuleTopWallThickness  = 1.0*cm;
  kModuleClearance         = 0.1*cm;
  kModuleMedFTDim          = {6*cm, 6.5*cm, 10*cm};
  kFieldShellThickness     = 0.005*cm;
  kCathodeThickness        = 0.5*cm;

  kPixelPlaneThickness     = 0.3*cm;
  kPixelSpacing            = 5*mm;
  kPixelRadius             = 1.5*mm; // spacing > 2*radius
  //kPixelActiveDim          = {0,0,0};

  kActiveLArDim            = {64.9*cm, 1215*mm, 65.19*cm}; // adjusted to get 67 x 67 foot
  kLightDetDim             = {10*cm, 50*cm, 0.4*cm};
  kTopLArDim               = {0.*cm, 182.5*mm, 0.*cm};
  kTopGArDim               = {0.*cm, 182.5*mm, 0.*cm};

  kModuleLegDim            = {6.*cm, 140*mm, 2*cm};
  kModuleLegFootDim        = {8.*cm, 2*cm, 4*cm};
  kBottomDummyFlangeDim    = {0.*cm, 20*mm, 0*cm};
  kModuleLegPosition       = {25*cm, 0*cm, 25*cm};

  // Cryostat
  kCryostatDim             = {(198/2.)*cm, 205*cm, 1.*cm, 5.*cm, (210/2.)*cm, 250*cm, 1.*cm};
  kCryostatLegDim          = {0.*cm, 5.*cm, 70*cm};
  kCryostatLegFootDim      = {0.*cm, 10*cm, 5.*cm};
  kCryostatLegOffset       = 50*cm;
  kCryostatFlangeDim       = {0,0,0};

  std::cout << std::setfill('-') << std::setw(60) << "-" << std::setfill(' ')  << std::endl;
  std::cout << "              ArgonCube2x2Geo " << version                << std::endl;
  std::cout << "      ArgonCube2x2 detector construction software       " << std::endl;
  std::cout                                                               << std::endl;
  std::cout << std::setfill('-') << std::setw(60) << "-" << std::setfill(' ')  << std::endl;

}
}

