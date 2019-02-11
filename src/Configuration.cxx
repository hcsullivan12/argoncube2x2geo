// 
// File: Configuration.cxx
//
// Author: Hunter Sullivan
//

#include "Configuration.h"
#include <iomanip>
#include <math.h>

#ifdef VERSION
#define version VERSION
#endif

namespace geo
{

Configuration* Configuration::instance = 0;

Configuration* Configuration::Instance()
{
  if (instance == 0)
  {
    static Configuration config;
    instance = &config;
  }
  return instance;
}

Configuration::Configuration()
{}

Configuration::~Configuration()
{}

void Configuration::Initialize(const std::string& gdmlPath)
{
  fGDMLOutputPath = gdmlPath;

  // World
  fWorldDim = {10*m, 10*m, 10*m};

  // Module
  // Many of the dimensions are derived from other volumes,
  // so zeros act as placeholders
  fModuleWallThickness     = 1.0*cm;
  fModuleTopWallThickness  = 1.0*cm;
  fModuleClearance         = 0.1*cm;
  fModuleMedFTDim          = {6*cm, 6.5*cm, 10*cm};
  fFieldShellThickness     = 0.005*cm;
  fCathodeThickness        = 0.5*cm;
  fPixelPlaneThickness     = 0.3*cm;

  fActiveLArDim            = {64.9*cm, 1215*mm, 65.19*cm}; // adjusted to get 67 x 67 foot
  fLightDetDim             = {10*cm, 50*cm, 0.4*cm};
  fTopLArDim               = {0.*cm, 182.5*mm, 0.*cm};
  fTopGArDim               = {0.*cm, 182.5*mm, 0.*cm};

  fModuleLegDim            = {6.*cm, 140*mm, 2*cm};
  fModuleLegFootDim        = {8.*cm, 2*cm, 4*cm};
  fBottomDummyFlangeDim    = {0.*cm, 20*mm, 0*cm};
  fModuleLegPosition       = {25*cm, 0*cm, 25*cm};

  // Cryostat
  fCryostatDim             = {(198/2.)*cm, 205*cm, 1.*cm, 5.*cm, (210/2.)*cm, 250*cm, 1.*cm};
  fCryostatLegDim          = {0.*cm, 5.*cm, 70*cm};
  fCryostatLegFootDim      = {0.*cm, 10*cm, 5.*cm};
  fCryostatLegOffset       = 50*cm;
  fCryostatFlangeDim       = {0,0,0};

  std::cout << std::setfill('-') << std::setw(60) << "-" << std::setfill(' ')  << std::endl;
  std::cout << "              ArgonCube2x2Geo " << version                << std::endl;
  std::cout << "      ArgonCube2x2 detector construction software       " << std::endl;
  std::cout                                                               << std::endl;
  std::cout << std::setfill('-') << std::setw(60) << "-" << std::setfill(' ')  << std::endl;

}
}

