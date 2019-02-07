double moduleHeight = 0;
double moduleFootX  = 0;
double moduleFootZ  = 0;
double cryoHeight   = 0;

void detectorView(TString filename,Bool_t checkoverlaps=kFALSE)
{
  
  Int_t PriKolor[] = {  2,  3,  4,  5,  6,  7,  8, 9, 28, 30, 38, 40, 41, 42, 46 };
  Int_t PriIndex = 0;
  std::map<TString,Int_t> Kolor;
  Kolor["Steel"] = kWhite;
  Kolor["Copper"] = kYellow;
  Kolor["Aluminum"] = kRed;
  Kolor["FR4"] = kGreen;
  Kolor["LAr"] = kBlue;
  Kolor["PVT"] = kViolet;
  Kolor["Kapton"] = kBlack;
  Kolor["Rock"] = kOrange+3;
  Kolor["SSteel304"] = kGray+1;

  TGeoManager *geo2 = new TGeoManager("geo2","test");
  geo2->Import(filename);
  geo2->SetVisLevel(20);
  TGeoVolume *volume = NULL;
  TObjArray *volumes = geo2->GetListOfVolumes();
  Int_t nvolumes = volumes->GetEntries();

  for ( int i = 0; i < nvolumes; i++ )
  {
    volume = (TGeoVolume*)volumes->At(i);
    volume->SetVisContainers(kTRUE);

    if (TString(volume->GetMaterial()->GetName()).Contains("Air")) volume->SetInvisible();
    if (TString(volume->GetMaterial()->GetName()).Contains("LAr")) volume->SetTransparency(80);
    //if (TString(volume->GetMaterial()->GetName()).Contains("SSteel")) volume->SetTransparency(10);

//    if (TString(volume->GetMaterial()->GetName()).Contains("Steel")) volume->SetTransparency(10);
    //if (TString(volume->GetMaterial()->GetName()).Contains("Rock")) volume->SetInvisible();

    auto tubs = volume->GetShape();
    cout << volume->GetName() << endl;


     //if (TString(tubs->GetName()) == "solCryoInnerBath") tubs->Draw("ogl");
   //if (TString(volume->GetName()) == "volCryoLeg") volume->Draw("ogl");


    if (TString(volume->GetName()).Contains("ArgonCubeCryostat")) 
    {  
      TGeoTube* tubs = (TGeoTube*)volume->GetShape();
      cryoHeight = 2*tubs->GetDX();
    }
    if (   TString(volume->GetName()).Contains("volModuleWall")) 
    {  
      TGeoBBox* tubs = (TGeoBBox*)volume->GetShape();
      moduleFootX  = 2*tubs->GetDX();
      moduleFootZ  = 2*tubs->GetDZ();
    }
    if (TString(volume->GetName()) == TString("volModule"))
    {  
      TGeoBBox* tubs = (TGeoBBox*)volume->GetShape();
      moduleHeight = 2*tubs->GetDY();
    }
 
    Int_t daughters = volume->GetNdaughters();
    //cout << endl;
		//cout << volume->GetName() << volume->GetMaterial()->GetName() << daughters << endl;
    volume->SetLineColor(Kolor[volume->GetMaterial()->GetName()]);
  }

  cout << endl;
  cout << "Module height     = " << moduleHeight << " cm" << endl;
  cout << "Module foot size  = " << moduleFootX <<  " x " << moduleFootZ << " cm2" << endl;
  cout << endl;

  geo2->GetTopVolume()->Draw("ogl");

  TGLViewer * v = (TGLViewer *)gPad->GetViewer3D();
  Double_t refPos[3] = {0,0,0};
  v->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kTRUE, refPos);
  v->DrawGuides();
}
