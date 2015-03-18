void runcomparetgraphs(TString sample){

gSystem->CompileMacro("comparetgraphs.C");


comparetgraphs(sample,"uperpraw_vs_nvtx","uperp_vs_nvtx");

comparetgraphs(sample,"upararaw_vs_nvtx","upara_vs_nvtx");

comparetgraphs(sample,"uperpraw_vs_sumEt","uperp_vs_sumEt");

comparetgraphs(sample,"upararaw_vs_sumEt","upara_vs_sumEt");

comparetgraphs(sample,"uperpraw_vs_qt","uperp_vs_qt");

comparetgraphs(sample,"upararaw_vs_qt","upara_vs_qt");

comparetgraphs(sample,"upararawqt_vs_qt","uparaqt_vs_qt");

}
