#include "TString.h"
#include "TSystem.h"
#include "PlotWithRatio.C"

void runPlotWithRatio(){
//gSystem->CompileMacro("PlotWithRatio.C", "MuMu");


  testRatio("met_uPara_zlloverzll_pt_vs_zll_pt", "qt [GeV]", "MuMu");
  testRatio( "met_uPara_zll+zll_pt_vs_zll_pt", "qt [GeV]","MuMu") ;
  testRatio("met_uPerp_zll_vs_zll_pt","qt [GeV]", "MuMu");
  testRatio("met_uPara_zll+zll_pt_vs_met_sumEt", "sumET [GeV]", "MuMu");
  testRatio("met_uPerp_zll_vs_met_sumEt","sumET [GeV]", "MuMu");
  testRatio("met_uPara_zll+zll_pt_vs_nVert","nvertex","MuMu") ; 
  testRatio("met_uPerp_zll_vs_nVert","nvertex", "MuMu");

}

