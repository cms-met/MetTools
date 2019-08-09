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
#include <TF1.h>

int prepare_combined_plots(){
  
  std::vector<TFile*> files;
  files.push_back(TFile::Open("outFile_combined_DataDoubleMu2018A_v3.root"));
  files.push_back(TFile::Open("outFile_combined_DataDoubleMu2018B_v3.root"));
  files.push_back(TFile::Open("outFile_combined_DataDoubleMu2018C_v3.root"));
  files.push_back(TFile::Open("outFile_combined_DataDoubleMu2018D_v3.root"));
  
  std::vector<TF1*> laurentFunctionsX;
  std::vector<TF1*> laurentFunctionsY;
  laurentFunctionsX.push_back(new TF1("laurentFunctionsA_x","0.363*x-1.945",0,100));
  laurentFunctionsX.push_back(new TF1("laurentFunctionsB_x","0.492*x-2.936",0,100));
  laurentFunctionsX.push_back(new TF1("laurentFunctionsC_x","0.521*x-1.445",0,100));
  laurentFunctionsX.push_back(new TF1("laurentFunctionsD_x","0.531*x-1.376",0,100));
  laurentFunctionsY.push_back(new TF1("laurentFunctionsA_y","0.071*x-0.307",0,100));
  laurentFunctionsY.push_back(new TF1("laurentFunctionsB_y","0.179*x-0.787",0,100));
  laurentFunctionsY.push_back(new TF1("laurentFunctionsC_y","0.119*x-1.964",0,100));
  laurentFunctionsY.push_back(new TF1("laurentFunctionsD_y","0.088*x-1.571",0,100));

  std::vector<TString> legendNames;
  legendNames.push_back("2018A");
  legendNames.push_back("2018B");
  legendNames.push_back("2018C");
  legendNames.push_back("2018D");

  std::vector<int> colors{4,2,3,6,7,8};
  //std::vector<int> colors{3,6,7,8};

  std::cout << "Files are open." << std::endl;

  std::vector<TString> names, shortNames, xAxis, yAxis;
  std::vector<int> range;
  names.push_back("histo_combined_x");
  shortNames.push_back("combined_x");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(100);
  names.push_back("histo_combined_y");
  shortNames.push_back("combined_y");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{y}(GeV)");
  range.push_back(100);
  for(unsigned iHist = 0; iHist < names.size(); iHist++){
    std::vector<TH1D*> histos;
    TLegend theLegend(0.7,0.7,0.9,0.9);
    double minValue = 0;
    double maxValue = 0;
    for(unsigned iFiles = 0 ; iFiles < files.size() ; iFiles++){
      files[iFiles]->cd();
      histos.push_back((TH1D*)gDirectory->Get(names[iHist]));
      histos[iFiles]->SetLineColor(colors[iFiles]);
      laurentFunctionsX[iFiles]->SetLineColor(colors[iFiles]);
      laurentFunctionsY[iFiles]->SetLineColor(colors[iFiles]);
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
    TCanvas myCan("can","Nicolas made this awesome canvas",700,500);
    myCan.cd();
    gStyle->SetOptStat(0);
    for(unsigned i = 0 ; i < files.size() ; i++){
      if(minValue > 0) histos[i]->SetMinimum(minValue*0.9);
      else histos[i]->SetMinimum(minValue*1.1);
      if(maxValue > 0) histos[i]->SetMaximum(maxValue*1.1);
      else histos[i]->SetMaximum(maxValue*0.9);
      histos[i]->Draw("e0same");
      if(iHist == 0) laurentFunctionsX[i]->Draw("same");
      if(iHist == 1) laurentFunctionsY[i]->Draw("same");
    }
    theLegend.Draw();
    myCan.SaveAs("plots_v3/plot_"+shortNames[iHist]+"_all2018.pdf");
  }

  return 0;
}
