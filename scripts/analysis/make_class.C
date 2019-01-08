void make_class() {
    TFile *f = new TFile("/home/hunter/projects/Majorana/output/simulateOutput1000.root");
    TTree *t = (TTree*)f->Get("anatree");
    t->MakeClass("centerAna1000");
}
