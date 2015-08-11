#include "TString.h"
#include "TSystem.h"
#include "metperformance.C"

//gSystem->CompileMacro("metperformance.C");
void runmetperformance(TString sample, bool background){




 if (sample.Contains ("GJet") || sample.Contains ("QCD")) {

   metperformance(sample,"met_uPara_zll+zll_pt","zll_pt", "Gamma" , false, background);
   metperformance(sample,"met_uPerp_zll","zll_pt", "Gamma" , false, background);
   metperformance(sample,"met_uPara_zll+zll_pt","met_sumEt", "Gamma" , false, background);
   metperformance(sample,"met_uPerp_zll","met_sumEt", "Gamma" , false, background);
   metperformance(sample,"met_uPara_zll+zll_pt","nVert", "Gamma" , false, background);
   metperformance(sample,"met_uPerp_zll","nVert", "Gamma" , false, background);
   // Plot the scale plots
   metperformance(sample,"met_uPara_zll/zll_pt","zll_pt", "Gamma" , false, background);
   metperformance(sample,"met_uPara_zll/zll_pt","nVert", "Gamma" , false, background);
   metperformance(sample,"met_uPara_zll/zll_pt","met_sumEt", "Gamma" , false, background);

 } 
 else {


/*   metperformance(sample,"met_uPara_zllraw+zll_pt","zll_pt", "EE" , false, background);
   metperformance(sample,"met_uPara_zll+zll_pt","zll_pt", "EE" , false, background);
   metperformance(sample,"met_uPerp_zll","zll_pt", "EE" , false, background);
   metperformance(sample,"met_uPerp_zllraw","zll_pt", "EE" , false, background);
   metperformance(sample,"met_uPara_zllraw+zll_pt","met_sumEt", "EE" , false, background);
   metperformance(sample,"met_uPara_zll+zll_pt","met_sumEt", "EE" , false, background);
   metperformance(sample,"met_uPerp_zll","met_sumEt", "EE" , false, background);
   metperformance(sample,"met_uPerp_zllraw","met_sumEt", "EE" , false, background);
   metperformance(sample,"met_uPara_zllraw+zll_pt","nVert", "EE" , false, background);
   metperformance(sample,"met_uPara_zll+zll_pt","nVert", "EE" , false, background);
   metperformance(sample,"met_uPerp_zll","nVert", "EE" , false, background);
   metperformance(sample,"met_uPerp_zllraw","nVert", "EE" , false, background);
   // Plot the scale plots
   metperformance(sample,"met_uPara_zll/zll_pt","zll_pt", "EE" , false, background);
   metperformance(sample,"met_uPara_zllraw/zll_pt","zll_pt", "EE" , false, background);
*/
   metperformance(sample,"met_uPara_zll/zll_pt","zll_pt", "MuMu" , false, background);
   metperformance(sample,"met_uPara_zll+zll_pt","zll_pt", "MuMu" , false, background);
   metperformance(sample,"met_uPara_zll/zll_pt","zll_pt", "MuMu" , false, background);
   metperformance(sample,"met_uPerp_zll","zll_pt", "MuMu" , false, background);
   metperformance(sample,"met_uPara_zll+zll_pt","met_sumEt", "MuMu" , false, background);
   metperformance(sample,"met_uPerp_zll","met_sumEt", "MuMu" , false, background);
   metperformance(sample,"met_uPara_zll+zll_pt","nVert", "MuMu" , false, background);
   metperformance(sample,"met_uPerp_zll","nVert", "MuMu" , false, background);
   // Plot the scale plots

 }

}

