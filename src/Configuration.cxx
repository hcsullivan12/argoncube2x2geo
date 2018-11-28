// 
// File: Configuration.cxx
//
// Author: Hunter Sullivan
//
// Discription: Class to initalize configuration.
//

#include "Configuration.h"
#include <iomanip>
#include <math.h>

// Preprocessing variables
#ifdef VERSION
#define majoranaVersion VERSION
#endif

namespace majorana
{
Configuration::Configuration()
{}

Configuration::~Configuration()
{}

void Configuration::Initialize(const std::string& configPath)
{
  m_configPath = configPath;
  // Read json file
  FILE *configFile = fopen(m_configPath.c_str(), "r");
  char readBuffer[65536];
  rapidjson::FileReadStream jsonStream(configFile, readBuffer, sizeof(readBuffer));

  m_jsonDoc.ParseStream(jsonStream);
  fclose(configFile);

  if (m_jsonDoc.HasParseError() || !m_jsonDoc.IsObject())
  {
    std::cerr << "Error. Failed to parse config file " << m_configPath << "!" << std::endl;
    std::exit(1);
  }

  // Save the configuration
  ReadJSONFile();
  // Safety checks
  CheckConfiguration();
  // Output to terminal
  PrintConfiguration();
}

void Configuration::ReadJSONFile()
{
  m_simulateOutputPath = GetJSONMember("simulateOutputPath", rapidjson::kStringType).GetString();
  m_recoAnaTreePath    = GetJSONMember("recoAnaTreePath", rapidjson::kStringType).GetString();
  m_steeringFilePath   = GetJSONMember("steeringFilePath", rapidjson::kStringType).GetString();
  m_visMacroPath       = GetJSONMember("visMacroPath", rapidjson::kStringType).GetString();
  m_sourceMode         = GetJSONMember("sourceMode", rapidjson::kNumberType).GetUint();
  m_nEvents            = GetJSONMember("nEvents", rapidjson::kNumberType).GetUint();
  m_nMPPCs             = GetJSONMember("nMPPCs", rapidjson::kNumberType).GetUint();
  m_mppcArea           = GetJSONMember("mppcArea", rapidjson::kNumberType).GetDouble();
  m_diskRadius         = GetJSONMember("diskRadius", rapidjson::kNumberType).GetDouble();
  m_diskThickness      = GetJSONMember("diskThickness", rapidjson::kNumberType).GetDouble();
  m_sourcePosSigma     = GetJSONMember("sourcePosSigma", rapidjson::kNumberType).GetDouble(); 
  m_sourcePeakE        = GetJSONMember("sourcePeakE", rapidjson::kNumberType).GetDouble(); 
  m_sourcePeakESigma   = GetJSONMember("sourcePeakESigma", rapidjson::kNumberType).GetDouble(); 
  m_reconstruct        = GetJSONMember("reconstruct", rapidjson::kFalseType).GetBool();  
}

const rapidjson::Value& Configuration::GetJSONMember(const std::string&     memberName,
                                                     rapidjson::Type        memberType,
                                                     const unsigned&        arraySize,
                                                     const rapidjson::Type& arrayType)
{
   // Check to see if the document has memberName
   if (!m_jsonDoc.HasMember(memberName.c_str())) 
   {
     std::cerr << "ERROR: \"" << memberName << "\" in config file not found!" << std::endl;
     exit(1);
   }

   // Get the value specified for memberName
   rapidjson::Value& member = m_jsonDoc[memberName.c_str()];

   // Make sure the types match
   if ( ((memberType       == rapidjson::kTrueType) || (memberType       == rapidjson::kFalseType)) &&
       !((member.GetType() == rapidjson::kTrueType) || (member.GetType() == rapidjson::kFalseType)) ) 
   {
     std::cerr << "ERROR: \"" << memberName << "\" in config file has wrong type!"<< std::endl;
     std::cerr << "Expected " << m_jsonTypes.at(rapidjson::kTrueType)
               << " or " << m_jsonTypes.at(rapidjson::kFalseType)
               << ", got " << m_jsonTypes.at(member.GetType()) << "." << std::endl;
     exit(1);
   }
   // Handle boolean
   if ( (memberType == rapidjson::kTrueType) || (memberType == rapidjson::kFalseType) )
   {
     memberType = member.GetType();
   }

   if (member.GetType() != memberType) 
   {
     std::cerr << "ERROR: \"" << memberName << "\" in run config file has wrong type!"<< std::endl;
     std::cerr << "Expected " << m_jsonTypes.at(memberType) << ", got " << m_jsonTypes.at(member.GetType())
               << "." << std::endl;
     exit(1);
   }

   if (member.GetType() == rapidjson::kArrayType) 
   {
     if (member.Size() != arraySize) 
     {
       std::cerr << "ERROR: Size mismatch for array \"" << memberName << "\" in config file!" << std::endl;
       std::cerr << "Expected " << arraySize << ", got " << member.Size() << "." << std::endl;
       exit(1);
     }
     for (const auto& value : member.GetArray()) 
     {
       if (value.GetType() != arrayType) 
       {
         std::cerr << "ERROR: Type mismatch in array \"" << memberName << "\" in config file!" << std::endl;
         std::cerr << "Expected " << m_jsonTypes.at(arrayType) << ", got "
                   << m_jsonTypes.at(value.GetType()) << "." << std::endl;
         exit(1);
       }
     }
   }
   return member;
}

void Configuration::CheckConfiguration()
{
  // Make sure the configuration makes sense
  if (m_nMPPCs     <= 0) { std::cerr << "ERROR. Number of MPPCs < 0." << std::endl; exit(1); }
  if (m_mppcArea   <= 0) { std::cerr << "ERROR. MPPC areas < 0."      << std::endl; exit(1); }
  if (m_diskRadius <= 0) { std::cerr << "ERROR. Disk radius < 0." << std::endl; exit(1); }
  if (m_diskThickness <= 0) { std::cerr << "ERROR. Disk thickness < 0." << std::endl; exit(1); }
  if (m_nEvents < 0)     { std::cerr << "ERROR. Number of events < 0." << std::endl; exit(1); }
  if (m_sourceMode > 1)  { std::cerr << "ERROR. Input mode usage: 0 - random positions, 1 - input text file." 
                                     << std::endl; exit(1); }
}

void Configuration::PrintConfiguration()
{
  // Hello there!
  std::cout << std::setfill('-') << std::setw(60) << "-" << std::setfill(' ')  << std::endl;
  std::cout << "              Majorana " << majoranaVersion                    << std::endl;
  std::cout << "       Simulation software for SiPM Wheel           "          << std::endl;
  std::cout << "     Author: Hunter Sullivan (UT Arlington)         "          << std::endl;
  std::cout                                                                    << std::endl;
  std::cout << "Majorana Configuration:\n";
  std::cout << "SimulateOutputPath " << m_simulateOutputPath << std::endl
            << "Mode               " << m_sourceMode         << std::endl;
  if (m_sourceMode == 1) { std::cout << "SteeringFilePath    "  << m_steeringFilePath << std::endl; } 
  else             { std::cout << "nEvents            "  << m_nEvents   << std::endl; }
  std::cout << "NumerOfMPPCs       " << m_nMPPCs             << std::endl
            << "SipmArea           " << m_mppcArea           << " cm2" << std::endl
            << "DiskRadius         " << m_diskRadius         << " cm"  << std::endl
            << "DiskThickness      " << m_diskThickness      << " cm"  << std::endl
            << "SourcePosSigma     " << m_sourcePosSigma     << " cm"  << std::endl
            << "SourcePeakE        " << m_sourcePeakE        << " eV"  << std::endl
            << "SourcePeakESigma   " << m_sourcePeakESigma   << " eV"  << std::endl
            << "Reconstruct        "; 
  if (m_reconstruct) { std::cout << "true\n"; std::cout << "RecoAnaTreePath    " << m_recoAnaTreePath << std::endl; } 
  else                 std::cout << "false\n";
  std::cout << std::setfill('-') << std::setw(60) << "-" << std::setfill(' ')  << std::endl;

}  
}

