#define centerAna1000NEW_cxx
#include "centerAna1000NEW.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

std::vector<TH1S*> histos;
TH1S* finalHisto;

void centerAna1000NEW::Loop()
{
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) 
  {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    std::cout << nPrimaries << std::endl;
    // Initialize histos
    if (jentry == 0) 
    {
      for (unsigned m = 1; m <= nMPPCs; m++)
      {
        std::string name = "mppc" + std::to_string(m);
        histos.push_back(new TH1S(name.c_str(), name.c_str(), 100, 0, 100));
      }
    }

    for (unsigned m = 1; m <= nMPPCs; m++)
    {
      histos[m-1]->Fill(mppcToLY[m-1]);
    }
  }

  /*unsigned m = 0;
  for (auto histo : histos)
  {
    std::string name = "c" + std::to_string(m);
    TCanvas *c1 = new TCanvas(name.c_str(), name.c_str(), 800, 800);
    histo->Draw();
    m++;
  }*/

  finalHisto = new TH1S("finalHisto", "MPPCs LY", 8, 0, 8);

  for (unsigned m = 0; m < nMPPCs; m++)
  {
    // Fit the histos and fill
    TF1 fit("fit", "gaus", 0, 100);
    histos[m]->Fit(&fit, "Q");

    finalHisto->SetBinContent(m+1, fit.GetParameter(1));
    finalHisto->SetBinError(m+1, fit.GetParameter(2));
    std::cout << fit.GetParameter(2) << std::endl;
  }

  finalHisto->SetMinimum(0);
  finalHisto->SetMaximum(60);
  //finalHisto->SetMarkerStyle(8);
  finalHisto->Draw("hist");
}
