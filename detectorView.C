double moduleHeight = 0;
double moduleFootX  = 0;
double moduleFootZ  = 0;
double cryoHeight   = 0;

void checkOverlaps( TGeoManager *geo );

void detectorView(TString filename,Bool_t checkoverlaps=kFALSE)
{  
  std::map<TString,Int_t> color;
  color["Steel"]     = kWhite;
  color["Copper"]    = kYellow;
  color["Aluminum"]  = kRed;
  color["FR4"]       = kGreen;
  color["LAr"]       = kBlue;
  color["PVT"]       = kViolet;
  color["Kapton"]    = kBlack;
  color["Rock"]      = kOrange+3;
  color["SSteel304"] = kGray+1;
  color["Steel"]     = kGray;

  TGeoManager *geo = new TGeoManager("geo","test");
  geo->Import(filename);
  geo->SetVisLevel(20);
  TGeoVolume *volume = NULL;
  TObjArray *volumes = geo->GetListOfVolumes();
  Int_t nvolumes = volumes->GetEntries();

  if (checkoverlaps==kTRUE) checkOverlaps(geo);

  cout << endl;
  for ( int i = 0; i < nvolumes; i++ )
  {
    volume = (TGeoVolume*)volumes->At(i);
    volume->SetVisContainers(kTRUE);

    if (TString(volume->GetMaterial()->GetName()).Contains("Air")) volume->SetInvisible();
    if (TString(volume->GetMaterial()->GetName()).Contains("LAr")) volume->SetTransparency(80);

    if (TString(volume->GetName()).Contains("volModule")) 
    {  
      TGeoBBox* tubs = (TGeoBBox*)volume->GetShape();
      moduleFootX  = 2*tubs->GetDX();
      moduleFootZ  = 2*tubs->GetDZ();
    }
    if (TString(volume->GetName()) == TString("volModule"))
    {  
      TGeoBBox* tubs = (TGeoBBox*)volume->GetShape();
      moduleHeight = moduleHeight + 2*tubs->GetDY();
    }
    if (TString(volume->GetName()) == TString("volModuleTopWall"))
    {
      // This is a boolean!
      moduleHeight = moduleHeight+1.0;
      volume->SetLineColor(kRed+2);
      continue;
    }
    if (TString(volume->GetName()) == TString("volModuleBottom"))
    {  
      TGeoBBox* tubs = (TGeoBBox*)volume->GetShape();
      cout << "Bottom height = " << 2*tubs->GetDY() << endl;
    }


   auto s = volume->GetShape();
   //cout << s->GetName() << endl;
   //if (TString(volume->GetName()) == TString("volTPC")) s->Draw("ogl");
 
    //cout << volume->GetName() << endl;
    volume->SetLineColor(color[volume->GetMaterial()->GetName()]);

    //if (TString(volume->GetName()) == TString("volTPC")) volume->Draw("ogl");
  }

  cout << endl;
  cout << "Module height     = " << moduleHeight << " cm" << endl;
  cout << "Module foot size  = " << moduleFootX <<  " x " << moduleFootZ << " cm2" << endl;
  cout << endl;

  geo->GetTopVolume()->Draw("ogl");

  TGLViewer * v = (TGLViewer *)gPad->GetViewer3D();
  Double_t refPos[3] = {0,0,0};
  v->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kTRUE, refPos);
  v->DrawGuides();

}

void checkOverlaps( TGeoManager *geo )
{
  geo->CheckOverlaps(0.01,"s10000000");
  geo->PrintOverlaps();
}

