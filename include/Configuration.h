// 
// File: Configuration.h
//
// Author: Hunter Sullivan
//
// Discription: Class to initalize configuration.
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
 
    void Initialize(const std::string& configPath);
    void SetVisualization(const bool& b) { fShowVis = b; };

    const bool&        ShowVis()         const { return fShowVis; };
    const std::string& VisMacroPath()    const { return fVisMacroPath; };
    const std::string& GDMLOutputPath()  const { return fGDMLOutputPath; };
    const double       ModuleG10WallThickness() const { return fModuleG10WallThickness; };
    const double       ModuleClearance()        const { return fModuleClearance; };
    const std::vector<double>& WorldDimensions()  const { return fWorldDimensions; };
    const std::vector<double>& ModuleDimensions() const { return fModuleDimensions; };
    const std::vector<double>& CryostatDimensions() const { return fCryostatDimensions; };
   
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
    std::string fVisMacroPath;
    std::string fGDMLOutputPath;
    bool        fShowVis;
    double      fModuleG10WallThickness;
    double      fModuleClearance;
    std::vector<double> fWorldDimensions;
    std::vector<double> fModuleDimensions;
    std::vector<double> fCryostatDimensions;
};
}

#endif
