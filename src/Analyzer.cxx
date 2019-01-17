// 
// File: Analyzer.cxx
//
// Author: Hunter Sullivan
//
// Discription: 
//

#include "Analyzer.h"
#include "TFile.h"
#include "OpDetPhotonTable.h"
#include "G4Helper.h"

namespace majorana 
{

Analyzer::Analyzer(const std::string& simOutputPath)
 : m_anaTree(NULL),
   m_simulateOutputPath(simOutputPath)
{
  // Reset variables
  ResetVars();
  
  m_anaTree = new TTree("anatree", "analysis tree");
  m_anaTree->Branch("event",      &m_event, "event/I");
  m_anaTree->Branch("nMPPCs",     &m_nMPPCs, "nMPPCs/I");
  m_anaTree->Branch("diskRadius", &m_diskRadius, "diskRadius/D");
  m_anaTree->Branch("nPrimaries", &m_nPrimaries, "nPrimaries/I");
  m_anaTree->Branch("sourcePosXYZ", m_sourcePosXYZ, "sourcePosXYZ[3]/D");
  m_anaTree->Branch("sourcePosRTZ", m_sourcePosRTZ, "sourcePosRTZ[3]/D");
  m_anaTree->Branch("mppcToLY", m_mppcToLY, "mppcToLY[nMPPCs]/D");
  m_anaTree->Branch("mppcToSourceR", m_mppcToSourceR, "mppcToSourceR[nMPPCs]/D");
  m_anaTree->Branch("mppcToSourceT", m_mppcToSourceT, "mppcToSourceT[nMPPCs]/D");
}

Analyzer::~Analyzer()
{
  TFile f(m_simulateOutputPath.c_str(), "UPDATE");
  m_anaTree->Write();
  f.Close();

  if (m_anaTree) delete m_anaTree;
}

void Analyzer::Fill(const unsigned& e)
{
  ResetVars();
  // Get the necessary information
  OpDetPhotonTable* photonTable = OpDetPhotonTable::Instance();
  if (!photonTable) return;
  G4Helper* g4Helper = G4Helper::Instance();
  if (!g4Helper) return;

  // Basically we want to look at the light yield as a function of position 
  m_event        = e;
  m_nMPPCs       = g4Helper->GetDetectorConstruction()->WheelGeometry()->NMPPCs();
  m_diskRadius   = g4Helper->GetDetectorConstruction()->WheelGeometry()->Radius();
  m_nPrimaries   = g4Helper->GetActionInitialization()->GetGeneratorAction()->GetNPrimaries();

  auto xyzVec = g4Helper->GetActionInitialization()->GetGeneratorAction()->GetSourcePositionXYZ();
  auto rtzVec = g4Helper->GetActionInitialization()->GetGeneratorAction()->GetSourcePositionRTZ();
  m_sourcePosXYZ[0] = xyzVec[0]; 
  m_sourcePosXYZ[1] = xyzVec[1]; 
  m_sourcePosXYZ[2] = xyzVec[2];
  m_sourcePosRTZ[0] = rtzVec[0]; 
  m_sourcePosRTZ[1] = rtzVec[1]; 
  m_sourcePosRTZ[2] = rtzVec[2];
  
  auto photonsDetected = photonTable->GetPhotonsDetected();
  photonTable->Print();
  for (unsigned m = 1; m <= m_nMPPCs; m++)
  {
    int photons(0);
    if (photonsDetected.find(m) != photonsDetected.end())
    {
      photons = photonsDetected.find(m)->second.size();
    }
    m_mppcToLY[m-1] = photons;

    // Calculate distances to each mppc
    float r        = m_sourcePosRTZ[0];
    float thetaDeg = m_sourcePosRTZ[1];
    float betaDeg  = (m - 1)*360/m_nMPPCs;
    float diffRad = (betaDeg - thetaDeg)*pi/180;

    float R = std::sqrt(r*r + m_diskRadius*m_diskRadius - 2*r*m_diskRadius*std::cos(diffRad));
    float alphaDeg = std::abs(TMath::ASin(r*std::sin(diffRad)/R)*180/pi);
    
    m_mppcToSourceR[m-1] = R;
    m_mppcToSourceT[m-1] = alphaDeg;
  }
  
  m_anaTree->Fill();
}

void Analyzer::ResetVars()
{
  m_event  = -99999;
  m_nMPPCs = -99999;
  m_diskRadius = -99999;
  m_nPrimaries = -99999;
  m_sourcePosXYZ[0] = -99999; m_sourcePosXYZ[1] = -99999; m_sourcePosXYZ[2] = -99999;
  m_sourcePosRTZ[0] = -99999; m_sourcePosRTZ[1] = -99999; m_sourcePosRTZ[2] = -99999;
  for (unsigned k = 0; k < kMaxMPPCs; k++)
  {
    m_mppcToLY[k]      = -99999;
    m_mppcToSourceR[k] = -99999;
    m_mppcToSourceT[k] = -99999;
  }
}

}
