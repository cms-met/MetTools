void runmetperformance(TString sample){

gSystem->CompileMacro("metperformance.C");


metperformance(sample,"upararaw+qt","qt", "EE" , false);
metperformance(sample,"upara+qt","qt", "EE" , false);
metperformance(sample,"uperp","qt", "EE" , false);
metperformance(sample,"uperpraw","qt", "EE" , false);
metperformance(sample,"upararaw+qt","sumEt", "EE" , false);
metperformance(sample,"upara+qt","sumEt", "EE" , false);
metperformance(sample,"uperp","sumEt", "EE" , false);
metperformance(sample,"uperpraw","sumEt", "EE" , false);
metperformance(sample,"upararaw+qt","nvtx", "EE" , false);
metperformance(sample,"upara+qt","nvtx", "EE" , false);
metperformance(sample,"uperp","nvtx", "EE" , false);
metperformance(sample,"uperpraw","nvtx", "EE" , false);
// Plot the scale plots
metperformance(sample,"upara/qt","qt", "EE" , false);
metperformance(sample,"upararaw/qt","qt", "EE" , false);


metperformance(sample,"upararaw+qt","qt", "MuMu" , false);
metperformance(sample,"upara+qt","qt", "MuMu" , false);
metperformance(sample,"uperp","qt", "MuMu" , false);
metperformance(sample,"uperpraw","qt", "MuMu" , false);
metperformance(sample,"upararaw+qt","sumEt", "MuMu" , false);
metperformance(sample,"upara+qt","sumEt", "MuMu" , false);
metperformance(sample,"uperp","sumEt", "MuMu" , false);
metperformance(sample,"uperpraw","sumEt", "MuMu" , false);
metperformance(sample,"upararaw+qt","nvtx", "MuMu" , false);
metperformance(sample,"upara+qt","nvtx", "MuMu" , false);
metperformance(sample,"uperp","nvtx", "MuMu" , false);
metperformance(sample,"uperpraw","nvtx", "MuMu" , false);
// Plot the scale plots
metperformance(sample,"upara/qt","qt", "MuMu" , false);
metperformance(sample,"upararaw/qt","qt", "MuMu" , false);

}
