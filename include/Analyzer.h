// 
// File: Analyzer.h
//
// Author: Hunter Sullivan
//
// Discription: 
//

#ifndef ANALYZER_HH
#define ANALYZER_HH

#include "TTree.h"

const int kMaxMPPCs = 100;

namespace majorana {

class Analyzer
{
  public:
    Analyzer(const std::string& simOutputPath);
    ~Analyzer();

    void Fill(const unsigned& e);
  
  private: 
    void ResetVars();

    TTree* m_anaTree;
    int    m_event;
    int    m_nMPPCs;
    double m_diskRadius;
    int    m_nPrimaries;
    double m_sourcePosXYZ[3];
    double m_sourcePosRTZ[3];
    double m_mppcToLY[kMaxMPPCs];
    double m_mppcToSourceR[kMaxMPPCs];
    double m_mppcToSourceT[kMaxMPPCs];
    std::string m_simulateOutputPath;
};
}
#endif
