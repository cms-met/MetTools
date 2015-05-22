#include "TString.h"
#include "TSystem.h"
#include "metperformance.C"

void runmetperformance(TString sample){

gSystem->CompileMacro("metperformance.C");

 if (sample.Contains ("GJet") || sample.Contains ("QCD")) {

   metperformance(sample,"upararaw+qt","qt", "Gamma" , false, false);
   metperformance(sample,"upara+qt","qt", "Gamma" , false, false);
   metperformance(sample,"uperp","qt", "Gamma" , false, false);
   metperformance(sample,"uperpraw","qt", "Gamma" , false, false);
   metperformance(sample,"upararaw+qt","sumEt", "Gamma" , false, false);
   metperformance(sample,"upara+qt","sumEt", "Gamma" , false, false);
   metperformance(sample,"uperp","sumEt", "Gamma" , false, false);
   metperformance(sample,"uperpraw","sumEt", "Gamma" , false, false);
   metperformance(sample,"upararaw+qt","nvtx", "Gamma" , false, false);
   metperformance(sample,"upara+qt","nvtx", "Gamma" , false, false);
   metperformance(sample,"uperp","nvtx", "Gamma" , false, false);
   metperformance(sample,"uperpraw","nvtx", "Gamma" , false, false);
   // Plot the scale plots
   metperformance(sample,"upara/qt","qt", "Gamma" , false, false);
   metperformance(sample,"upararaw/qt","qt", "Gamma" , false, false);
   metperformance(sample,"upara/qt","nvtx", "Gamma" , false, false);
   metperformance(sample,"upararaw/qt","nvtx", "Gamma" , false, false);
   metperformance(sample,"upara/qt","sumEt", "Gamma" , false, false);
   metperformance(sample,"upararaw/qt","sumEt", "Gamma" , false, false);

 } 
 else {


   metperformance(sample,"upararaw+qt","qt", "EE" , false, false);
   metperformance(sample,"upara+qt","qt", "EE" , false, false);
   metperformance(sample,"uperp","qt", "EE" , false, false);
   metperformance(sample,"uperpraw","qt", "EE" , false, false);
   metperformance(sample,"upararaw+qt","sumEt", "EE" , false, false);
   metperformance(sample,"upara+qt","sumEt", "EE" , false, false);
   metperformance(sample,"uperp","sumEt", "EE" , false, false);
   metperformance(sample,"uperpraw","sumEt", "EE" , false, false);
   metperformance(sample,"upararaw+qt","nvtx", "EE" , false, false);
   metperformance(sample,"upara+qt","nvtx", "EE" , false, false);
   metperformance(sample,"uperp","nvtx", "EE" , false, false);
   metperformance(sample,"uperpraw","nvtx", "EE" , false, false);
   // Plot the scale plots
   metperformance(sample,"upara/qt","qt", "EE" , false, false);
   metperformance(sample,"upararaw/qt","qt", "EE" , false, false);


   metperformance(sample,"upararaw+qt","qt", "MuMu" , false, false);
   metperformance(sample,"upara+qt","qt", "MuMu" , false, false);
   metperformance(sample,"uperp","qt", "MuMu" , false, false);
   metperformance(sample,"uperpraw","qt", "MuMu" , false, false);
   metperformance(sample,"upararaw+qt","sumEt", "MuMu" , false, false);
   metperformance(sample,"upara+qt","sumEt", "MuMu" , false, false);
   metperformance(sample,"uperp","sumEt", "MuMu" , false, false);
   metperformance(sample,"uperpraw","sumEt", "MuMu" , false, false);
   metperformance(sample,"upararaw+qt","nvtx", "MuMu" , false, false);
   metperformance(sample,"upara+qt","nvtx", "MuMu" , false, false);
   metperformance(sample,"uperp","nvtx", "MuMu" , false, false);
   metperformance(sample,"uperpraw","nvtx", "MuMu" , false, false);
   // Plot the scale plots
   metperformance(sample,"upara/qt","qt", "MuMu" , false, false);
   metperformance(sample,"upararaw/qt","qt", "MuMu" , false, false);

 }

}

