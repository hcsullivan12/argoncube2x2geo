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
  fModuleWallThickness     = 1.0*cm;
  fModuleTopWallThickness  = 1.0*cm;
  fModuleClearance         = 0.1*cm;
  fModuleMedFTDim          = {6*cm, 6.5*cm, 10*cm};
  fFieldShellThickness     = 0.005*cm;
  fCathodeThickness        = 0.5*cm;
  fPixelPlaneThickness     = 0.3*cm;
  fActiveLArDim            = {65*cm, 120*cm, 60*cm};
  fLightDetDim             = {10*cm, 50*cm, 0.4*cm};
  fModuleLegDim            = {6.*cm, 8*cm, 2*cm};
  fModuleLegFootDim        = {8.*cm, 2*cm, 4*cm};
  fBottomDummyFlangeDim    = {0.*cm, 2*cm, 0*cm};
  fModuleLegPosition       = {25*cm, 0*cm, 25*cm};
  fTopLArDim               = {0.*cm, 20*cm, 0.*cm};
  fTopGArDim               = {0.*cm, 20*cm, 0.*cm};

  // Cryostat
  fCryostatDim             = {105*cm, 200*cm, 1.*cm, 5.*cm, 110*cm, 230*cm, 1.*cm};
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

