#include <TH1.h>
#include <TH2.h>
#include <TString.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TFile.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include <TLatex.h>
#include <iostream>
#include <vector>

//Prepares plots for the X and Y MET corrections as a function of different variables and of different particle types. Superimposes them for different eras within the same year.

int prepare_plots(int year=2018){
  
  //Update file names and paths if needed.
  std::vector<TFile*> files;
  if(year==2018){
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2018A.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2018B.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2018C.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2018D.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_MC2018.root"));
  }
  else if (year==2017){
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2017B.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2017C.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2017D.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2017E.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2017F.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_MC2017.root"));
  }
  else if (year==2016){
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2016B.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2016C.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2016D.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2016E.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2016F.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2016G.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2016H.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_MC2016.root"));
  }

  std::vector<TString> legendNames;
  if(year==2018){
    legendNames.push_back("2018A");
    legendNames.push_back("2018B");
    legendNames.push_back("2018C");
    legendNames.push_back("2018D");
    legendNames.push_back("MC2018 (DY)");
  }
  else if (year==2017){
    legendNames.push_back("2017B");
    legendNames.push_back("2017C");
    legendNames.push_back("2017D");
    legendNames.push_back("2017E");
    legendNames.push_back("2017F");
    legendNames.push_back("MC2017 (DY)");
  }
  else if (year==2016){
    legendNames.push_back("2016B");
    legendNames.push_back("2016C");
    legendNames.push_back("2016D");
    legendNames.push_back("2016E");
    legendNames.push_back("2016F");
    legendNames.push_back("2016G");
    legendNames.push_back("2016H");
    legendNames.push_back("MC2016 (DY)");
  }

  std::vector<int> colors{4,2,3,6,7,8,42,28};

  std::cout << "Files are open." << std::endl;

  int maxNvtx = 50;

  std::vector<TString> names, shortNames, xAxis, yAxis;
  std::vector<int> range;
  names.push_back("metPhiCorrInfoWriter_multiplicity_hEtaPlus_Px");
  shortNames.push_back("mult_hEtaPlus_Px");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(1000);
  names.push_back("metPhiCorrInfoWriter_multiplicity_hEtaPlus_Py");
  shortNames.push_back("mult_hEtaPlus_Py");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(1000);
  names.push_back("metPhiCorrInfoWriter_multiplicity_hEtaMinus_Px");
  shortNames.push_back("mult_hEtaMinus_Px");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(1000);
  names.push_back("metPhiCorrInfoWriter_multiplicity_hEtaMinus_Py");
  shortNames.push_back("mult_hEtaMinus_Py");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(1000);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_hEtaPlus_Px");
  shortNames.push_back("vert_hEtaPlus_Px");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_hEtaPlus_Py");
  shortNames.push_back("vert_hEtaPlus_Py");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_hEtaMinus_Px");
  shortNames.push_back("vert_hEtaMinus_Px");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_hEtaMinus_Py");
  shortNames.push_back("vert_hEtaMinus_Py");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("metPhiCorrInfoWriter_sumPt_hEtaPlus_Px");
  shortNames.push_back("pt_hEtaPlus_Px");
  xAxis.push_back("sumPt (GeV)");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(1000);
  names.push_back("metPhiCorrInfoWriter_sumPt_hEtaPlus_Py");
  shortNames.push_back("pt_hEtaPlus_Py");
  xAxis.push_back("sumPt (GeV)");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(1000);
  names.push_back("metPhiCorrInfoWriter_sumPt_hEtaMinus_Px");
  shortNames.push_back("pt_hEtaMinus_Px");
  xAxis.push_back("sumPt (GeV)");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(1000);
  names.push_back("metPhiCorrInfoWriter_sumPt_hEtaMinus_Py");
  shortNames.push_back("pt_hEtaMinus_Py");
  xAxis.push_back("sumPt (GeV)");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(1000);
  names.push_back("metPhiCorrInfoWriter_multiplicity_h0Barrel_Px");
  shortNames.push_back("mult_h0Barrel_Px");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(50);
  names.push_back("metPhiCorrInfoWriter_multiplicity_h0Barrel_Py");
  shortNames.push_back("mult_h0Barrel_Py");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(50);
  names.push_back("metPhiCorrInfoWriter_multiplicity_h0EndcapPlus_Px");
  shortNames.push_back("mult_h0EndcapPlus_Px");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(50);
  names.push_back("metPhiCorrInfoWriter_multiplicity_h0EndcapPlus_Py");
  shortNames.push_back("mult_h0EndcapPlus_Py");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(50);
  names.push_back("metPhiCorrInfoWriter_multiplicity_h0EndcapMinus_Px");
  shortNames.push_back("mult_h0EndcapMinus_Px");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(50);
  names.push_back("metPhiCorrInfoWriter_multiplicity_h0EndcapMinus_Py");
  shortNames.push_back("mult_h0EndcapMinus_Py");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(50);
  names.push_back("metPhiCorrInfoWriter_multiplicity_gammaBarrel_Px");
  shortNames.push_back("mult_gammaBarrel_Px");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(500);
  names.push_back("metPhiCorrInfoWriter_multiplicity_gammaBarrel_Py");
  shortNames.push_back("mult_gammaBarrel_Py");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(500);
  names.push_back("metPhiCorrInfoWriter_multiplicity_gammaEndcapPlus_Px");
  shortNames.push_back("mult_gammaEndcapPlus_Px");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(100);
  names.push_back("metPhiCorrInfoWriter_multiplicity_gammaEndcapPlus_Py");
  shortNames.push_back("mult_gammaEndcapPlus_Py");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(100);
  names.push_back("metPhiCorrInfoWriter_multiplicity_gammaEndcapMinus_Px");
  shortNames.push_back("mult_gammaEndcapMinus_Px");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(100);
  names.push_back("metPhiCorrInfoWriter_multiplicity_gammaEndcapMinus_Py");
  shortNames.push_back("mult_gammaEndcapMinus_Py");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(100);
  names.push_back("metPhiCorrInfoWriter_multiplicity_egammaHFPlus_Px");
  shortNames.push_back("mult_egammaHFPlus_Px");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(300);
  names.push_back("metPhiCorrInfoWriter_multiplicity_egammaHFPlus_Py");
  shortNames.push_back("mult_egammaHFPlus_Py");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(300);
  names.push_back("metPhiCorrInfoWriter_multiplicity_egammaHFMinus_Px");
  shortNames.push_back("mult_egammaHFMinus_Px");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(300);
  names.push_back("metPhiCorrInfoWriter_multiplicity_egammaHFMinus_Py");
  shortNames.push_back("mult_egammaHFMinus_Py");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(300);
  names.push_back("metPhiCorrInfoWriter_multiplicity_hHFMinus_Px");
  shortNames.push_back("mult_hHFMinus_Px");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(300);
  names.push_back("metPhiCorrInfoWriter_multiplicity_hHFMinus_Py");
  shortNames.push_back("mult_hHFMinus_Py");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(300);
  names.push_back("metPhiCorrInfoWriter_multiplicity_hHFPlus_Px");
  shortNames.push_back("mult_hHFPlus_Px");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(300);
  names.push_back("metPhiCorrInfoWriter_multiplicity_hHFPlus_Py");
  shortNames.push_back("mult_hHFPlus_Py");
  xAxis.push_back("multiplicity");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(300);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_hHFMinus_Px");
  shortNames.push_back("vert_hHFMinus_Px");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_hHFMinus_Py");
  shortNames.push_back("vert_hHFMinus_Py");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_hHFPlus_Px");
  shortNames.push_back("vert_hHFPlus_Px");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_hHFPlus_Py");
  shortNames.push_back("vert_hHFPlus_Py");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_h0Barrel_Px");
  shortNames.push_back("vert_h0Barrel_Px");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_h0Barrel_Py");
  shortNames.push_back("vert_h0Barrel_Py");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_h0EndcapPlus_Px");
  shortNames.push_back("vert_h0EndcapPlus_Px");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_h0EndcapPlus_Py");
  shortNames.push_back("vert_h0EndcapPlus_Py");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_h0EndcapMinus_Px");
  shortNames.push_back("vert_h0EndcapMinus_Px");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_h0EndcapMinus_Py");
  shortNames.push_back("vert_h0EndcapMinus_Py");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(100);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_gammaBarrel_Px");
  shortNames.push_back("vert_gammaBarrel_Px");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_gammaBarrel_Py");
  shortNames.push_back("vert_gammaBarrel_Py");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(100);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_gammaEndcapPlus_Px");
  shortNames.push_back("vert_gammaEndcapPlus_Px");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_gammaEndcapPlus_Py");
  shortNames.push_back("vert_gammaEndcapPlus_Py");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(100);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_gammaEndcapMinus_Px");
  shortNames.push_back("vert_gammaEndcapMinus_Px");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_gammaEndcapMinus_Py");
  shortNames.push_back("vert_gammaEndcapMinus_Py");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(100);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_egammaHFPlus_Px");
  shortNames.push_back("vert_egammaHFPlus_Px");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_egammaHFPlus_Py");
  shortNames.push_back("vert_egammaHFPlus_Py");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_egammaHFMinus_Px");
  shortNames.push_back("vert_egammaHFMinus_Px");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("metPhiCorrInfoWriter_ngoodVertices_egammaHFMinus_Py");
  shortNames.push_back("vert_egammaHFMinus_Py");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(maxNvtx);

  for(unsigned iHist = 0; iHist < names.size(); iHist++){
    std::vector<TH1D*> histos;
    TLegend theLegend(0.7,0.7,0.9,0.9);
    double minValue = 0;
    double maxValue = 0;
    for(unsigned iFiles = 0 ; iFiles < files.size() ; iFiles++){
      files[iFiles]->cd();
      histos.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/"+names[iHist]));
      histos[iFiles]->SetLineColor(colors[iFiles]);
      histos[iFiles]->SetTitle(shortNames[iHist]);
      histos[iFiles]->GetXaxis()->SetTitle(xAxis[iHist]);
      histos[iFiles]->GetXaxis()->SetLabelSize(0.05);
      histos[iFiles]->GetXaxis()->SetTitleSize(0.05);
      histos[iFiles]->GetXaxis()->SetTitleOffset(0.98);
      histos[iFiles]->GetYaxis()->SetTitle(yAxis[iHist]);
      histos[iFiles]->GetYaxis()->SetLabelSize(0.05);
      histos[iFiles]->GetYaxis()->SetTitleSize(0.05);
      histos[iFiles]->GetYaxis()->SetTitleOffset(0.8);
      histos[iFiles]->SetAxisRange(0.,range[iHist],"X");
      if(minValue > histos[iFiles]->GetMinimum()) minValue = histos[iFiles]->GetMinimum();
      if(maxValue < histos[iFiles]->GetMaximum()) maxValue = histos[iFiles]->GetMaximum();

      theLegend.AddEntry(histos[iFiles],legendNames[iFiles],"lpfe");
    }
    TCanvas myCan("can","Output canvas",700,500);
    myCan.cd();
    gStyle->SetOptStat(0);
    for(unsigned i = 0 ; i < files.size() ; i++){
      if(minValue > 0) histos[i]->SetMinimum(minValue*0.9);
      else histos[i]->SetMinimum(minValue*1.1);
      if(maxValue > 0) histos[i]->SetMaximum(maxValue*1.1);
      else histos[i]->SetMaximum(maxValue*0.9);
      histos[i]->Draw("e0same");
    }
    theLegend.Draw();
    if(year==2018) myCan.SaveAs("plots_2018/plot_"+shortNames[iHist]+".pdf");
    else if(year==2017) myCan.SaveAs("plots_2017/plot_"+shortNames[iHist]+".pdf");
    else if(year==2016) myCan.SaveAs("plots_2016/plot_"+shortNames[iHist]+".pdf");
  }

  return 0;
}
