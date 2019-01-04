#define ana_cxx
#include "ana.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

TH

std::vector<float> avgs, stdev;
unsigned nEvents = 0;

void ana::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;

  for (Long64_t jentry=0; jentry<nentries;jentry++) 
  {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    
    nEvents++;
   /* std::cout << "Event  = " << event << "\n"
              << "nMPPCs = " << nMPPCs << "\n"
              << "nPrimaries = " << nPrimaries << "\n"
              << "SourcePos = " << sourcePosRTZ[0] << " " << sourcePosRTZ[1] << "\n"
              << std::endl; */

    for (int m = 0; m < nMPPCs; m++)
    {
      if (avgs.size() < nMPPCs)
      {
        avgs.push_back(mppcToLY[m]);
        continue; 
      }
      avgs[m] = avgs[m] + mppcToLY[m];
    }    
  }
  for (int m = 0; m < avgs.size(); m++)
  {
    avgs[m] = avgs[m]/nEvents;
    std::cout << avgs[m] << std::endl;
  }

  for (Long64_t jentry=0; jentry<nentries;jentry++) 
  {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    for (int m = 0; m < nMPPCs; m++)
    {
      if (stdev.size() < nMPPCs)
      {
        float delta = mppcToLY[m] - avgs[m];
        stdev.push_back(delta*delta);
        continue; 
      } 
      float delta = mppcToLY[m] - avgs[m];
      stdev[m] = stdev[m] + delta*delta;
    }  
  }
  for (int m = 0; m < avgs.size(); m++)
  {
    stdev[m] = std::sqrt(stdev[m]/(nEvents-1));
    std::cout << stdev[m] << std::endl;
  }
}
