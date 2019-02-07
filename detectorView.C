double moduleHeight = 0;
double moduleFootX  = 0;
double moduleFootZ  = 0;
double cryoHeight   = 0;

void checkOverlaps( TGeoManager *geo );

void detectorView(TString filename,Bool_t checkoverlaps=kTRUE)
{  
  Int_t PriKolor[] = {  2,  3,  4,  5,  6,  7,  8, 9, 28, 30, 38, 40, 41, 42, 46 };
  Int_t PriIndex = 0;
  std::map<TString,Int_t> color;
  color["Steel"] = kWhite;
  color["Copper"] = kYellow;
  color["Aluminum"] = kRed;
  color["FR4"] = kGreen;
  color["LAr"] = kBlue;
  color["PVT"] = kViolet;
  color["Kapton"] = kBlack;
  color["Rock"] = kOrange+3;
  color["SSteel304"] = kGray+1;

  TGeoManager *geo = new TGeoManager("geo","test");
  geo->Import(filename);
  geo->SetVisLevel(20);
  TGeoVolume *volume = NULL;
  TObjArray *volumes = geo->GetListOfVolumes();
  Int_t nvolumes = volumes->GetEntries();

  if (checkoverlaps==kTRUE) checkOverlaps(geo);

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
    volume->SetLineColor(color[volume->GetMaterial()->GetName()]);
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

