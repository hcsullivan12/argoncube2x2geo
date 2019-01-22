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

    bool        ShowVis()         const { return fShowVis; };
    std::string VisMacroPath()    const { return fVisMacroPath; };
    std::string GDMLOutputPath()  const { return fGDMLOutputPath; };
    
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
};
}

#endif
