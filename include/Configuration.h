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

    enum ModeType
    {
      kRANDOMMODE = 0,
      kINPUTMODE  = 1
    };

    void Initialize(const std::string& configPath);

    unsigned NMPPCs()        const { return m_nMPPCs; };
    float    MPPCArea()      const { return m_mppcArea; };
    float    DiskRadius()    const { return m_diskRadius; };
    float    DiskThickness() const { return m_diskThickness; };
    
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
    unsigned                         m_mode;
    unsigned                         m_nEvents;
    unsigned                         m_nMPPCs;
    float                            m_mppcArea;
    float                            m_diskRadius;
    float                            m_diskThickness;
    float                            m_sourceSigma;
    bool                             m_reconstruct;
   
};
}

#endif
