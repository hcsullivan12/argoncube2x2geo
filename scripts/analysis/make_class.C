void make_class() {
    TFile *f = new TFile("/home/hunter/projects/Majorana/output/simulateOutputXaxis.root");
    TTree *t = (TTree*)f->Get("anatree");
    t->MakeClass("xAxisAna");
}
