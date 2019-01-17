void make_class() {
    TFile *f = new TFile("/home/hunter/projects/Majorana/production/1000000_probabilities.root");
    TTree *t = (TTree*)f->Get("anatree");
    t->MakeClass("probabilitiesAna");
}
