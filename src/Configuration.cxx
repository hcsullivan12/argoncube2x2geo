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

void Configuration::Initialize(const std::string& configPath)
{
  fConfigPath = configPath;
  // Read json file
  FILE *configFile = fopen(fConfigPath.c_str(), "r");
  char readBuffer[65536];
  rapidjson::FileReadStream jsonStream(configFile, readBuffer, sizeof(readBuffer));

  fJSONDoc.ParseStream(jsonStream);
  fclose(configFile);

  if (fJSONDoc.HasParseError() || !fJSONDoc.IsObject())
  {
    std::cerr << "Error. Failed to parse config file " << fConfigPath << "!" << std::endl;
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
  #ifdef G4_GDML 
  fGDMLOutputPath = GetJSONMember("gdmlOutputPath", rapidjson::kStringType).GetString();
  #endif
  fModuleWallThickness = GetJSONMember("moduleWallThickness", rapidjson::kNumberType).GetDouble();
  fModuleClearance = GetJSONMember("moduleClearance", rapidjson::kNumberType).GetDouble();
  fFieldShellThickness = GetJSONMember("fieldShellThickness", rapidjson::kNumberType).GetDouble();
  fCathodeThickness = GetJSONMember("cathodeThickness", rapidjson::kNumberType).GetDouble();
  fPixelPlaneThickness = GetJSONMember("pixelPlaneThickness", rapidjson::kNumberType).GetDouble();
  
  fWorldDimensions  = std::vector<double>(3, 0);
  auto jsonArrayItr = GetJSONMember("worldDimensions", rapidjson::kArrayType, 3, rapidjson::kNumberType).Begin();
  for (auto& d : fWorldDimensions) {d = jsonArrayItr->GetDouble(); jsonArrayItr++;}
 
  fCryostatDimensions  = std::vector<double>(3, 0);
  jsonArrayItr = GetJSONMember("cryostatDimensions", rapidjson::kArrayType, 3, rapidjson::kNumberType).Begin();
  for (auto& d : fCryostatDimensions) {d = jsonArrayItr->GetDouble(); jsonArrayItr++;}
 
  fModuleDimensions  = std::vector<double>(3, 0);
  jsonArrayItr = GetJSONMember("moduleDimensions", rapidjson::kArrayType, 3, rapidjson::kNumberType).Begin();
  for (auto& d : fModuleDimensions) {d = jsonArrayItr->GetDouble(); jsonArrayItr++;}

  fActiveLArDim = std::vector<double>(3, 0);
  jsonArrayItr = GetJSONMember("activeLArDim", rapidjson::kArrayType, 3, rapidjson::kNumberType).Begin();
  for (auto& d : fActiveLArDim) {d = jsonArrayItr->GetDouble(); jsonArrayItr++;}

  fLightDetDim = std::vector<double>(3, 0);
  jsonArrayItr = GetJSONMember("lightDetDim", rapidjson::kArrayType, 3, rapidjson::kNumberType).Begin();
  for (auto& d : fLightDetDim) {d = jsonArrayItr->GetDouble(); jsonArrayItr++;}

  fModuleLegDim = std::vector<double>(3, 0);
  jsonArrayItr = GetJSONMember("moduleLegDim", rapidjson::kArrayType, 3, rapidjson::kNumberType).Begin();
  for (auto& d : fModuleLegDim) {d = jsonArrayItr->GetDouble(); jsonArrayItr++;}

  fModuleLegFootDim = std::vector<double>(3, 0);
  jsonArrayItr = GetJSONMember("moduleLegFootDim", rapidjson::kArrayType, 3, rapidjson::kNumberType).Begin();
  for (auto& d : fModuleLegFootDim) {d = jsonArrayItr->GetDouble(); jsonArrayItr++;}

  fBottomDummyFlangeDim = std::vector<double>(3, 0);
  jsonArrayItr = GetJSONMember("bottomDummyFlangeDim", rapidjson::kArrayType, 3, rapidjson::kNumberType).Begin();
  for (auto& d : fBottomDummyFlangeDim) {d = jsonArrayItr->GetDouble(); jsonArrayItr++;}

  fLegPosition = std::vector<double>(3, 0);
  jsonArrayItr = GetJSONMember("legPosition", rapidjson::kArrayType, 3, rapidjson::kNumberType).Begin();
  for (auto& d : fLegPosition) {d = jsonArrayItr->GetDouble(); jsonArrayItr++;}
}

const rapidjson::Value& Configuration::GetJSONMember(const std::string&     memberName,
                                                     rapidjson::Type        memberType,
                                                     const unsigned&        arraySize,
                                                     const rapidjson::Type& arrayType)
{
   // Check to see if the document has memberName
   if (!fJSONDoc.HasMember(memberName.c_str())) 
   {
     std::cerr << "ERROR: \"" << memberName << "\" in config file not found!" << std::endl;
     exit(1);
   }

   // Get the value specified for memberName
   rapidjson::Value& member = fJSONDoc[memberName.c_str()];

   // Make sure the types match
   if ( ((memberType       == rapidjson::kTrueType) || (memberType       == rapidjson::kFalseType)) &&
       !((member.GetType() == rapidjson::kTrueType) || (member.GetType() == rapidjson::kFalseType)) ) 
   {
     std::cerr << "ERROR: \"" << memberName << "\" in config file has wrong type!"<< std::endl;
     std::cerr << "Expected " << fJSONTypes.at(rapidjson::kTrueType)
               << " or " << fJSONTypes.at(rapidjson::kFalseType)
               << ", got " << fJSONTypes.at(member.GetType()) << "." << std::endl;
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
     std::cerr << "Expected " << fJSONTypes.at(memberType) << ", got " << fJSONTypes.at(member.GetType())
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
         std::cerr << "Expected " << fJSONTypes.at(arrayType) << ", got "
                   << fJSONTypes.at(value.GetType()) << "." << std::endl;
         exit(1);
       }
     }
   }
   return member;
}

void Configuration::CheckConfiguration()
{
  // Make sure the configuration makes sense
}

void Configuration::PrintConfiguration()
{
  // Hello there!
  std::cout << std::setfill('-') << std::setw(60) << "-" << std::setfill(' ')  << std::endl;
  std::cout << "              geo " << version                                 << std::endl;
  std::cout << "  ArgonCube2x2 detector construction software       "          << std::endl;
  std::cout << "     Author: Hunter Sullivan (UT Arlington)         "          << std::endl;
  std::cout                                                                    << std::endl;
  std::cout << "geo Configuration:\n";
  #ifdef G4_GDML
  std::cout << "GDMLOutputPath     " << fGDMLOutputPath     << std::endl;
  #endif
  std::cout << std::setfill('-') << std::setw(60) << "-" << std::setfill(' ')  << std::endl;
}  
}

