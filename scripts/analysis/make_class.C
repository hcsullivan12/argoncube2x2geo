void make_class() {
    TFile *f = new TFile("/home/hunter/projects/Majorana/production/simulateOutputVoxelization1cm_665.root");
    TTree *t = (TTree*)f->Get("anatree");
    t->MakeClass("voxelizationAna");
}
