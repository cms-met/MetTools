#include "TString.h"
#include "TSystem.h"
#include "metperformance.C"

//gSystem->CompileMacro("metperformance.C");
void runmetperformance(TString sample, bool background){




 if (sample.Contains ("GJet") || sample.Contains ("QCD")) {

   metperformance(sample,"upararaw+qt","qt", "Gamma" , false, background);
   metperformance(sample,"upara+qt","qt", "Gamma" , false, background);
   metperformance(sample,"uperp","qt", "Gamma" , false, background);
   metperformance(sample,"uperpraw","qt", "Gamma" , false, background);
   metperformance(sample,"upararaw+qt","sumEt", "Gamma" , false, background);
   metperformance(sample,"upara+qt","sumEt", "Gamma" , false, background);
   metperformance(sample,"uperp","sumEt", "Gamma" , false, background);
   metperformance(sample,"uperpraw","sumEt", "Gamma" , false, background);
   metperformance(sample,"upararaw+qt","nvtx", "Gamma" , false, background);
   metperformance(sample,"upara+qt","nvtx", "Gamma" , false, background);
   metperformance(sample,"uperp","nvtx", "Gamma" , false, background);
   metperformance(sample,"uperpraw","nvtx", "Gamma" , false, background);
   // Plot the scale plots
   metperformance(sample,"upara/qt","qt", "Gamma" , false, background);
   metperformance(sample,"upararaw/qt","qt", "Gamma" , false, background);
   metperformance(sample,"upara/qt","nvtx", "Gamma" , false, background);
   metperformance(sample,"upararaw/qt","nvtx", "Gamma" , false, background);
   metperformance(sample,"upara/qt","sumEt", "Gamma" , false, background);
   metperformance(sample,"upararaw/qt","sumEt", "Gamma" , false, background);

 } 
 else {


   metperformance(sample,"upararaw+qt","qt", "EE" , false, background);
   metperformance(sample,"upara+qt","qt", "EE" , false, background);
   metperformance(sample,"uperp","qt", "EE" , false, background);
   metperformance(sample,"uperpraw","qt", "EE" , false, background);
   metperformance(sample,"upararaw+qt","sumEt", "EE" , false, background);
   metperformance(sample,"upara+qt","sumEt", "EE" , false, background);
   metperformance(sample,"uperp","sumEt", "EE" , false, background);
   metperformance(sample,"uperpraw","sumEt", "EE" , false, background);
   metperformance(sample,"upararaw+qt","nvtx", "EE" , false, background);
   metperformance(sample,"upara+qt","nvtx", "EE" , false, background);
   metperformance(sample,"uperp","nvtx", "EE" , false, background);
   metperformance(sample,"uperpraw","nvtx", "EE" , false, background);
   // Plot the scale plots
   metperformance(sample,"upara/qt","qt", "EE" , false, background);
   metperformance(sample,"upararaw/qt","qt", "EE" , false, background);


   metperformance(sample,"upararaw+qt","qt", "MuMu" , false, background);
   metperformance(sample,"upara+qt","qt", "MuMu" , false, background);
   metperformance(sample,"uperp","qt", "MuMu" , false, background);
   metperformance(sample,"uperpraw","qt", "MuMu" , false, background);
   metperformance(sample,"upararaw+qt","sumEt", "MuMu" , false, background);
   metperformance(sample,"upara+qt","sumEt", "MuMu" , false, background);
   metperformance(sample,"uperp","sumEt", "MuMu" , false, background);
   metperformance(sample,"uperpraw","sumEt", "MuMu" , false, background);
   metperformance(sample,"upararaw+qt","nvtx", "MuMu" , false, background);
   metperformance(sample,"upara+qt","nvtx", "MuMu" , false, background);
   metperformance(sample,"uperp","nvtx", "MuMu" , false, background);
   metperformance(sample,"uperpraw","nvtx", "MuMu" , false, background);
   // Plot the scale plots
   metperformance(sample,"upara/qt","qt", "MuMu" , false, background);
   metperformance(sample,"upararaw/qt","qt", "MuMu" , false, background);

 }

}

