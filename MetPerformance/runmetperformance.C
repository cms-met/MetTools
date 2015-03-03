void runmetperformance(TString sample){

gSystem->CompileMacro("metperformance.C");


metperformance(sample,"upararaw","qt",false);
metperformance(sample,"upara","qt",false);
metperformance(sample,"uperp","qt",false);
metperformance(sample,"uperpraw","qt",false);
metperformance(sample,"upararaw","sumEt",false);
metperformance(sample,"upara","sumEt",false);
metperformance(sample,"uperp","sumEt",false);
metperformance(sample,"uperpraw","sumEt",false);
metperformance(sample,"upararaw","nvtx",false);
metperformance(sample,"upara","nvtx",false);
metperformance(sample,"uperp","nvtx",false);
metperformance(sample,"uperpraw","nvtx",false);

// Plot the scale plots
metperformance(sample,"uparaqt","qt",false);
metperformance(sample,"upararawqt","qt",false);
}
