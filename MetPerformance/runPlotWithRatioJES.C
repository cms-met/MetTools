#include "TString.h"
#include "TSystem.h"
#include "PlotWithRatioJES.C"

void runPlotWithRatioJES(TString channel){
//gSystem->CompileMacro("PlotWithRatio.C", channel);


  testRatio("met_uPara_zlloverzll_pt_vs_zll_pt", "qt [GeV]", channel);
  testRatio( "met_uPara_zll+zll_pt_vs_zll_pt", "qt [GeV]",channel) ;
  testRatio("met_uPerp_zll_vs_zll_pt","qt [GeV]", channel);
  testRatio("met_uPara_zll+zll_pt_vs_met_sumEt", "sumET [TeV]", channel);
  testRatio("met_uPerp_zll_vs_met_sumEt","sumET [TeV]", channel);
  testRatio("met_uPara_zll+zll_pt_vs_nVert","nvertex",channel) ; 
  testRatio("met_uPerp_zll_vs_nVert","nvertex", channel);

}

