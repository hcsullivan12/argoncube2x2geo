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
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/rapidjson.h"

namespace geo
{

class Configuration
{
  public:
    static Configuration* Instance();
    ~Configuration();
 
    void Initialize(const std::string& configPath, const std::string& gdmlPath);

    const std::string& GDMLOutputPath()  const { return fGDMLOutputPath; };
    const double       ModuleWallThickness() const { return fModuleWallThickness; };
    const double       ModuleTopWallThickness() const { return fModuleTopWallThickness; };
    const double       FieldShellThickness() const { return fFieldShellThickness; };
    const double       CathodeThickness() const { return fCathodeThickness; };
    const double       PixelPlaneThickness() const { return fPixelPlaneThickness; };
    const double       ModuleClearance()        const { return fModuleClearance; };
    const double       CryostatLegOffset()        const { return fCryostatLegOffset; };
    const std::vector<double>& WorldDim()  const { return fWorldDim; };
    const std::vector<double>& ModuleDim() const { return fModuleDim; };
    const std::vector<double>& CryostatDim() const { return fCryostatDim; };
    const std::vector<double>& CryostatLegDim() const { return fCryostatLegDim; };
    const std::vector<double>& CryostatLegFootDim() const { return fCryostatLegFootDim; };
    const std::vector<double>& CryostatFlangeDim() const { return fCryostatFlangeDim; };
    const std::vector<double>& ActiveLArDim() const { return fActiveLArDim; };
    const std::vector<double>& LightDetDim()  const { return fLightDetDim; };
    const std::vector<double>& ModuleLegDim()  const { return fModuleLegDim; };
    const std::vector<double>& ModuleLegFootDim()  const { return fModuleLegFootDim; };
    const std::vector<double>& BottomDummyFlangeDim()  const { return fBottomDummyFlangeDim; };
    const std::vector<double>& ModuleLegPosition()  const { return fModuleLegPosition; };
    const std::vector<double>& ModuleMedFTDim()  const { return fModuleMedFTDim; };
    const std::vector<double>& TopLArDim()  const { return fTopLArDim; };
    const std::vector<double>& TopGArDim()  const { return fTopGArDim; };

  private:
    Configuration();
    static Configuration* instance;

    void ReadJSONFile();
    const rapidjson::Value& GetJSONMember(const std::string&     memberName,
                                          rapidjson::Type        memberType,
                                          const unsigned&        arraySize = 0,
                                          const rapidjson::Type& arrayType = rapidjson::kNullType);
    void CheckConfiguration();
    void PrintConfiguration();

    rapidjson::Document              fJSONDoc;
    const std::array<std::string, 7> fJSONTypes = {{"Null", 
                                                    "False", 
                                                    "True", 
                                                    "Object", 
                                                    "Array", 
                                                    "String", 
                                                    "Number"}};
    std::string fConfigPath;
    std::string fGDMLOutputPath;
    double      fModuleWallThickness;
    double      fModuleClearance;
    double      fFieldShellThickness;
    double      fCathodeThickness;
    double      fPixelPlaneThickness;
    double      fModuleTopWallThickness;
    double      fCryostatLegOffset;
    std::vector<double> fWorldDim;
    std::vector<double> fModuleDim;
    std::vector<double> fCryostatDim;
    std::vector<double> fCryostatLegDim;
    std::vector<double> fCryostatLegFootDim;
    std::vector<double> fCryostatFlangeDim;
    std::vector<double> fActiveLArDim;
    std::vector<double> fLightDetDim;
    std::vector<double> fModuleLegDim;
    std::vector<double> fModuleLegFootDim;
    std::vector<double> fModuleMedFTDim;
    std::vector<double> fBottomDummyFlangeDim;
    std::vector<double> fModuleLegPosition;
    std::vector<double> fTopLArDim;
    std::vector<double> fTopGArDim;
};
}

#endif
