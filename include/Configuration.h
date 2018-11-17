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

namespace majorana
{
class Configuration
{
  public:
    Configuration();
    ~Configuration();

    void Initialize();
    void SetConfigPath(const std::string& path)                    { m_configPath = path; };
    void SetInputPath(const std::string& path)                     { m_inputPath  = path; };
    void SetMode(const std::string& mode, const unsigned& nEvents) { m_mode = mode; m_nEvents = nEvents; };

  private:
    void ReadJSONFile();
    const rapidjson::Value& GetJSONMember(const std::string&     memberName,
                                          rapidjson::Type        memberType,
                                          const unsigned&        arraySize = 0,
                                          const rapidjson::Type& arrayType = rapidjson::kNullType);
    void CheckConfiguration();
    void PrintConfiguration();

    rapidjson::Document              m_jsonDoc;
    const std::array<std::string, 7> m_jsonTypes = {{"Null", "False", "True", "Object", "Array", "String", "Number"}};

    std::string                      m_configPath;
    std::string                      m_simulateOutputPath;
    std::string                      m_recoAnaTreePath;
    std::string                      m_inputPath;
    std::string                      m_mode;
    unsigned                         m_nEvents;
    unsigned                         m_nSiPMs;
    float                            m_sipmArea;
    float                            m_diskRadius;
    float                            m_diskThickness;
    float                            m_sourceSigma;
    bool                             m_reconstruct;
   
};
}

#endif
