// 
// File: Configuration.h
//
// Author: Hunter Sullivan
//

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <array>
#include <vector>
#include <iostream>

#include "G4Types.hh"
#include "G4SystemOfUnits.hh"

namespace geo
{

class Configuration
{
  public:
    static Configuration* Instance();
    ~Configuration();
 
    void Initialize(const std::string& gdmlPath);

    const std::string& GDMLOutputPath()                 const { return fGDMLOutputPath; };
    const G4double       ModuleWallThickness()          const { return fModuleWallThickness; };
    const G4double       ModuleTopWallThickness()       const { return fModuleTopWallThickness; };
    const G4double       FieldShellThickness()          const { return fFieldShellThickness; };
    const G4double       CathodeThickness()             const { return fCathodeThickness; };
    const G4double       PixelPlaneThickness()          const { return fPixelPlaneThickness; };
    const G4double       PixelSpacing()                 const { return fPixelSpacing; };
    const G4double       PixelRadius()                  const { return fPixelRadius; };
    const G4double       ModuleClearance()              const { return fModuleClearance; };
    const G4double       CryostatLegOffset()            const { return fCryostatLegOffset; };
    const std::vector<G4double>& WorldDim()             const { return fWorldDim; };
    const std::vector<G4double>& ModuleDim()            const { return fModuleDim; };
    const std::vector<G4double>& CryostatDim()          const { return fCryostatDim; };
    const std::vector<G4double>& CryostatLegDim()       const { return fCryostatLegDim; };
    const std::vector<G4double>& CryostatLegFootDim()   const { return fCryostatLegFootDim; };
    const std::vector<G4double>& CryostatFlangeDim()    const { return fCryostatFlangeDim; };
    const std::vector<G4double>& ActiveLArDim()         const { return fActiveLArDim; };
    const std::vector<G4double>& LightDetDim()          const { return fLightDetDim; };
    const std::vector<G4double>& ModuleLegDim()         const { return fModuleLegDim; };
    const std::vector<G4double>& ModuleLegFootDim()     const { return fModuleLegFootDim; };
    const std::vector<G4double>& BottomDummyFlangeDim() const { return fBottomDummyFlangeDim; };
    const std::vector<G4double>& ModuleLegPosition()    const { return fModuleLegPosition; };
    const std::vector<G4double>& ModuleMedFTDim()       const { return fModuleMedFTDim; };
    const std::vector<G4double>& TopLArDim()            const { return fTopLArDim; };
    const std::vector<G4double>& TopGArDim()            const { return fTopGArDim; };

  private:
    Configuration();
    static Configuration* instance;

    std::string   fGDMLOutputPath;
    G4double      fModuleWallThickness;
    G4double      fModuleClearance;
    G4double      fFieldShellThickness;
    G4double      fCathodeThickness;
    G4double      fPixelPlaneThickness;
    G4double      fPixelSpacing;
    G4double      fPixelRadius;
    G4double      fModuleTopWallThickness;
    G4double      fCryostatLegOffset;
    std::vector<G4double> fWorldDim;
    std::vector<G4double> fModuleDim;
    std::vector<G4double> fCryostatDim;
    std::vector<G4double> fCryostatLegDim;
    std::vector<G4double> fCryostatLegFootDim;
    std::vector<G4double> fCryostatFlangeDim;
    std::vector<G4double> fActiveLArDim;
    std::vector<G4double> fLightDetDim;
    std::vector<G4double> fModuleLegDim;
    std::vector<G4double> fModuleLegFootDim;
    std::vector<G4double> fModuleMedFTDim;
    std::vector<G4double> fBottomDummyFlangeDim;
    std::vector<G4double> fModuleLegPosition;
    std::vector<G4double> fTopLArDim;
    std::vector<G4double> fTopGArDim;
};
}

#endif
