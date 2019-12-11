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

//Very dirty function, used to prepare combined plots superimposing XY corrections for different eras and comparing with the results of the other method (taking directly the MET value to compute the correction).

int prepare_combined_plots(int year = 2018, int partial=0){
  
  std::vector<TFile*> files;
  if(year==2018){
    if(partial==0 or partial==1 or partial==12 or partial==123) files.push_back(TFile::Open("plots_2018/outFile_combined_DataDoubleMu2018A.root"));
    if(partial==0 or partial==2 or partial==12 or partial==123) files.push_back(TFile::Open("plots_2018/outFile_combined_DataDoubleMu2018B.root"));
    if(partial==0 or partial==3 or partial==34 or partial==123) files.push_back(TFile::Open("plots_2018/outFile_combined_DataDoubleMu2018C.root"));
    if(partial==0 or partial==4 or partial==34 or partial==456 or partial==4567) files.push_back(TFile::Open("plots_2018/outFile_combined_DataDoubleMu2018D.root"));
    if(partial==0 or partial==5 or partial==56 or partial==456 or partial==4567) files.push_back(TFile::Open("plots_2018/outFile_combined_MC2018.root"));
  }
  else if(year==2017){
    if(partial==0 or partial==1 or partial==12 or partial==123) files.push_back(TFile::Open("plots_2017/outFile_combined_DataDoubleMu2017B.root"));
    if(partial==0 or partial==2 or partial==12 or partial==123) files.push_back(TFile::Open("plots_2017/outFile_combined_DataDoubleMu2017C.root"));
    if(partial==0 or partial==3 or partial==34 or partial==123) files.push_back(TFile::Open("plots_2017/outFile_combined_DataDoubleMu2017D.root"));
    if(partial==0 or partial==4 or partial==34 or partial==45  or partial==456 or partial==4567) files.push_back(TFile::Open("plots_2017/outFile_combined_DataDoubleMu2017E.root"));
    if(partial==0 or partial==5 or partial==56 or partial==45 or partial==456 or partial==4567) files.push_back(TFile::Open("plots_2017/outFile_combined_DataDoubleMu2017F.root"));
    if(partial==0 or partial==6 or partial==56 or partial==456 or partial==4567) files.push_back(TFile::Open("plots_2017/outFile_combined_MC2017.root"));
  }
  else if(year==2016){
    if(partial==0 or partial==1 or partial==12 or partial==123) files.push_back(TFile::Open("plots_2016/outFile_combined_DataDoubleMu2016B.root"));
    if(partial==0 or partial==2 or partial==12 or partial==123) files.push_back(TFile::Open("plots_2016/outFile_combined_DataDoubleMu2016C.root"));
    if(partial==0 or partial==3 or partial==34 or partial==123) files.push_back(TFile::Open("plots_2016/outFile_combined_DataDoubleMu2016D.root"));
    if(partial==0 or partial==4 or partial==34 or partial==456 or partial==4567) files.push_back(TFile::Open("plots_2016/outFile_combined_DataDoubleMu2016E.root"));
    if(partial==0 or partial==5 or partial==56 or partial==456 or partial==4567) files.push_back(TFile::Open("plots_2016/outFile_combined_DataDoubleMu2016F.root"));
    if(partial==0 or partial==6 or partial==56 or partial==456 or partial==4567) files.push_back(TFile::Open("plots_2016/outFile_combined_DataDoubleMu2016G.root"));
    if(partial==0 or partial==7 or partial==78 or partial==789 or partial==4567) files.push_back(TFile::Open("plots_2016/outFile_combined_DataDoubleMu2016H.root"));
    if(partial==0 or partial==8 or partial==78 or partial==789) files.push_back(TFile::Open("plots_2016/outFile_combined_MC2016.root"));
  }
  
  std::vector<TF1*> laurentFunctionsX;
  std::vector<TF1*> laurentFunctionsY;
  if(year==2018){
    if(partial==0 or partial==1 or partial==12 or partial==123) laurentFunctionsX.push_back(new TF1("laurentFunctions2018A_x","0.363*x-1.945",0,100));
    if(partial==0 or partial==2 or partial==12 or partial==123) laurentFunctionsX.push_back(new TF1("laurentFunctions2018B_x","0.492*x-2.936",0,100));
    if(partial==0 or partial==3 or partial==34 or partial==123) laurentFunctionsX.push_back(new TF1("laurentFunctions2018C_x","0.521*x-1.445",0,100));
    if(partial==0 or partial==4 or partial==34 or partial==456 or partial==4567) laurentFunctionsX.push_back(new TF1("laurentFunctions2018D_x","0.531*x-1.376",0,100));
    if(partial==0 or partial==5 or partial==56 or partial==456 or partial==4567) laurentFunctionsX.push_back(new TF1("laurentFunctions2018MC_x","0.297*x-0.142",0,100));
    if(partial==0 or partial==1 or partial==12 or partial==123) laurentFunctionsY.push_back(new TF1("laurentFunctions2018A_y","0.071*x-0.307",0,100));
    if(partial==0 or partial==2 or partial==12 or partial==123) laurentFunctionsY.push_back(new TF1("laurentFunctions2018B_y","0.179*x-0.787",0,100));
    if(partial==0 or partial==3 or partial==34 or partial==123) laurentFunctionsY.push_back(new TF1("laurentFunctions2018C_y","0.119*x-1.964",0,100));
    if(partial==0 or partial==4 or partial==34 or partial==456 or partial==4567) laurentFunctionsY.push_back(new TF1("laurentFunctions2018D_y","0.088*x-1.571",0,100));
    if(partial==0 or partial==5 or partial==56 or partial==456 or partial==4567) laurentFunctionsY.push_back(new TF1("laurentFunctions2018MC_y","0.116*x+0.013",0,100));
  }
  else if(year==2017){
    if(partial==0 or partial==1 or partial==12 or partial==123) laurentFunctionsX.push_back(new TF1("laurentFunctions2017B_x","-0.259*x+1.954",0,100));
    if(partial==0 or partial==2 or partial==12 or partial==123) laurentFunctionsX.push_back(new TF1("laurentFunctions2017C_x","-0.233*x+1.083",0,100));
    if(partial==0 or partial==3 or partial==34 or partial==123) laurentFunctionsX.push_back(new TF1("laurentFunctions2017D_x","-0.238*x+1.805",0,100));
    if(partial==0 or partial==4 or partial==34 or partial==45 or partial==456 or partial==4567) laurentFunctionsX.push_back(new TF1("laurentFunctions2017E_x","-0.212*x+1.851",0,100));
    if(partial==0 or partial==5 or partial==56 or partial==45 or partial==456 or partial==4567) laurentFunctionsX.push_back(new TF1("laurentFunctions2017F_x","-0.233*x+2.241",0,100));
    if(partial==0 or partial==6 or partial==56 or partial==456 or partial==4567) laurentFunctionsX.push_back(new TF1("laurentFunctions2017MC_x","-0.218*x+0.493",0,100));
    if(partial==0 or partial==1 or partial==12 or partial==123) laurentFunctionsY.push_back(new TF1("laurentFunctions2017B_y","0.354*x-2.467",0,100));
    if(partial==0 or partial==2 or partial==12 or partial==123) laurentFunctionsY.push_back(new TF1("laurentFunctions2017C_y","0.258*x-1.174",0,100));
    if(partial==0 or partial==3 or partial==34 or partial==123) laurentFunctionsY.push_back(new TF1("laurentFunctions2017D_y","0.236*x-1.444",0,100));
    if(partial==0 or partial==4 or partial==34 or partial==45 or partial==456 or partial==4567) laurentFunctionsY.push_back(new TF1("laurentFunctions2017E_y","0.158*x-0.478",0,100));
    if(partial==0 or partial==5 or partial==56 or partial==45 or partial==456 or partial==4567) laurentFunctionsY.push_back(new TF1("laurentFunctions2017F_y","0.213*x+0.685",0,100));
    if(partial==0 or partial==6 or partial==56 or partial==456 or partial==4567) laurentFunctionsY.push_back(new TF1("laurentFunctions2017MC_y","0.177*x-0.337",0,100));
  }
  else if(year==2016){
    if(partial==0 or partial==1 or partial==12 or partial==123) laurentFunctionsX.push_back(new TF1("laurentFunctions2016B_x","-0.048*x-0.108",0,100));
    if(partial==0 or partial==2 or partial==12 or partial==123) laurentFunctionsX.push_back(new TF1("laurentFunctions2016C_x","-0.092*x+0.393",0,100));
    if(partial==0 or partial==3 or partial==34 or partial==123) laurentFunctionsX.push_back(new TF1("laurentFunctions2016D_x","-0.058*x+0.567",0,100));
    if(partial==0 or partial==4 or partial==34 or partial==456 or partial==4567) laurentFunctionsX.push_back(new TF1("laurentFunctions2016E_x","-0.066*x+0.537",0,100));
    if(partial==0 or partial==5 or partial==56 or partial==456 or partial==4567) laurentFunctionsX.push_back(new TF1("laurentFunctions2016F_x","-0.031*x+0.399",0,100));
    if(partial==0 or partial==6 or partial==56 or partial==456 or partial==4567) laurentFunctionsX.push_back(new TF1("laurentFunctions2016G_x","0.041*x-0.290",0,100));
    if(partial==0 or partial==7 or partial==78 or partial==789 or partial==4567) laurentFunctionsX.push_back(new TF1("laurentFunctions2016H_x","0.033*x-0.210",0,100));
    if(partial==0 or partial==8 or partial==78 or partial==789) laurentFunctionsX.push_back(new TF1("laurentFunctions2016MC_x","-0.195*x-0.171",0,100));
    if(partial==0 or partial==1 or partial==12 or partial==123) laurentFunctionsY.push_back(new TF1("laurentFunctions2016B_y","0.125*x+0.356",0,100));
    if(partial==0 or partial==2 or partial==12 or partial==123) laurentFunctionsY.push_back(new TF1("laurentFunctions2016C_y","0.151*x+0.114",0,100));
    if(partial==0 or partial==3 or partial==34 or partial==123) laurentFunctionsY.push_back(new TF1("laurentFunctions2016D_y","0.148*x+0.403",0,100));
    if(partial==0 or partial==4 or partial==34 or partial==456 or partial==4567) laurentFunctionsY.push_back(new TF1("laurentFunctions2016E_y","0.189*x-0.495",0,100));
    if(partial==0 or partial==5 or partial==56 or partial==456 or partial==4567) laurentFunctionsY.push_back(new TF1("laurentFunctions2016F_y","0.161*x+0.960",0,100));
    if(partial==0 or partial==6 or partial==56 or partial==456 or partial==4567) laurentFunctionsY.push_back(new TF1("laurentFunctions2016G_y","0.096*x+0.666",0,100));
    if(partial==0 or partial==7 or partial==78 or partial==789 or partial==4567) laurentFunctionsY.push_back(new TF1("laurentFunctions2016H_y","0.142*x+0.817",0,100));
    if(partial==0 or partial==8 or partial==78 or partial==789) laurentFunctionsY.push_back(new TF1("laurentFunctions2016MC_y","-0.031*x+0.788",0,100));
  }

  std::vector<TString> legendNames;
  if(year==2018){
    if(partial==0 or partial==1 or partial==12 or partial==123) legendNames.push_back("2018A");
    if(partial==0 or partial==2 or partial==12 or partial==123) legendNames.push_back("2018B");
    if(partial==0 or partial==3 or partial==34 or partial==123) legendNames.push_back("2018C");
    if(partial==0 or partial==4 or partial==34 or partial==456 or partial==4567) legendNames.push_back("2018D");
    if(partial==0 or partial==5 or partial==56 or partial==456 or partial==4567) legendNames.push_back("MC2018 (DY)");
  }
  else if(year==2017){
    if(partial==0 or partial==1 or partial==12 or partial==123) legendNames.push_back("2017B");
    if(partial==0 or partial==2 or partial==12 or partial==123) legendNames.push_back("2017C");
    if(partial==0 or partial==3 or partial==34 or partial==123) legendNames.push_back("2017D");
    if(partial==0 or partial==4 or partial==34 or partial==45 or partial==456 or partial==4567) legendNames.push_back("2017E");
    if(partial==0 or partial==5 or partial==56 or partial==45 or partial==456 or partial==4567) legendNames.push_back("2017F");
    if(partial==0 or partial==6 or partial==56 or partial==456 or partial==4567) legendNames.push_back("MC2017 (DY)");
  }
  else if(year==2016){
    if(partial==0 or partial==1 or partial==12 or partial==123) legendNames.push_back("2016B");
    if(partial==0 or partial==2 or partial==12 or partial==123) legendNames.push_back("2016C");
    if(partial==0 or partial==3 or partial==34 or partial==123) legendNames.push_back("2016D");
    if(partial==0 or partial==4 or partial==34 or partial==456 or partial==4567) legendNames.push_back("2016E");
    if(partial==0 or partial==5 or partial==56 or partial==456 or partial==4567) legendNames.push_back("2016F");
    if(partial==0 or partial==6 or partial==56 or partial==456 or partial==4567) legendNames.push_back("2016G");
    if(partial==0 or partial==7 or partial==78 or partial==789 or partial==4567) legendNames.push_back("2016H");
    if(partial==0 or partial==8 or partial==78 or partial==789) legendNames.push_back("MC2016 (DY)");
  }

  std::vector<int> colors;
  if(partial==0 or partial==1 or partial==12 or partial==123) colors.push_back(4);
  if(partial==0 or partial==2 or partial==12 or partial==123) colors.push_back(2);
  if(partial==0 or partial==3 or partial==34 or partial==123) colors.push_back(3);
  if(partial==0 or partial==4 or partial==45 or partial==34 or partial==456 or partial==4567) colors.push_back(6);
  if(partial==0 or partial==5 or partial==45 or partial==56 or partial==456 or partial==4567) colors.push_back(7);
  if(partial==0 or partial==6 or partial==56 or partial==456 or partial==4567) colors.push_back(8);
  if(partial==0 or partial==7 or partial==78 or partial==789 or partial==4567) colors.push_back(42);
  if(partial==0 or partial==8 or partial==78 or partial==789) colors.push_back(28);

  std::cout << "Files are open." << std::endl;

  int maxNvtx = 100;
  if(year==2016) maxNvtx = 50;
  if(year==2017) maxNvtx = 75;

  std::vector<TString> names, shortNames, xAxis, yAxis;
  std::vector<int> range;
  names.push_back("histo_combined_x");
  shortNames.push_back("combined_x");
  xAxis.push_back("ngoodVertices");
  yAxis.push_back("p_{x}(GeV)");
  range.push_back(maxNvtx);
  names.push_back("histo_combined_y");
  shortNames.push_back("combined_y");
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
      histos[iFiles]->SetAxisRange(-20.,20.,"Y");
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
      if(iHist == 0) laurentFunctionsX[i]->Draw("same");
      if(iHist == 1) laurentFunctionsY[i]->Draw("same");
    }
    theLegend.Draw();
    if(year==2018 and partial==0) myCan.SaveAs("plots_2018/plot_"+shortNames[iHist]+"_all2018.pdf");
    else if(year==2018 and partial==1) myCan.SaveAs("plots_2018/plot_"+shortNames[iHist]+"_2018A.pdf");
    else if(year==2018 and partial==2) myCan.SaveAs("plots_2018/plot_"+shortNames[iHist]+"_2018B.pdf");
    else if(year==2018 and partial==3) myCan.SaveAs("plots_2018/plot_"+shortNames[iHist]+"_2018C.pdf");
    else if(year==2018 and partial==4) myCan.SaveAs("plots_2018/plot_"+shortNames[iHist]+"_2018D.pdf");
    else if(year==2018 and partial==5) myCan.SaveAs("plots_2018/plot_"+shortNames[iHist]+"_2018MC.pdf");
    else if(year==2018 and partial==12) myCan.SaveAs("plots_2018/plot_"+shortNames[iHist]+"_2018AB.pdf");
    else if(year==2018 and partial==34) myCan.SaveAs("plots_2018/plot_"+shortNames[iHist]+"_2018CD.pdf");

    else if(year==2017 and partial==0) myCan.SaveAs("plots_2017/plot_"+shortNames[iHist]+"_all2017.pdf");
    else if(year==2017 and partial==1) myCan.SaveAs("plots_2017/plot_"+shortNames[iHist]+"_2017B.pdf");
    else if(year==2017 and partial==2) myCan.SaveAs("plots_2017/plot_"+shortNames[iHist]+"_2017C.pdf");
    else if(year==2017 and partial==3) myCan.SaveAs("plots_2017/plot_"+shortNames[iHist]+"_2017D.pdf");
    else if(year==2017 and partial==4) myCan.SaveAs("plots_2017/plot_"+shortNames[iHist]+"_2017E.pdf");
    else if(year==2017 and partial==5) myCan.SaveAs("plots_2017/plot_"+shortNames[iHist]+"_2017F.pdf");
    else if(year==2017 and partial==6) myCan.SaveAs("plots_2017/plot_"+shortNames[iHist]+"_2017MC.pdf");
    else if(year==2017 and partial==12) myCan.SaveAs("plots_2017/plot_"+shortNames[iHist]+"_2017BC.pdf");
    else if(year==2017 and partial==34) myCan.SaveAs("plots_2017/plot_"+shortNames[iHist]+"_2017DE.pdf");
    else if(year==2017 and partial==45) myCan.SaveAs("plots_2017/plot_"+shortNames[iHist]+"_2017EF.pdf");
    else if(year==2017 and partial==123) myCan.SaveAs("plots_2017/plot_"+shortNames[iHist]+"_2017BCD.pdf");

    else if(year==2016 and partial==0) myCan.SaveAs("plots_2016/plot_"+shortNames[iHist]+"_all2016.pdf");
    else if(year==2016 and partial==1) myCan.SaveAs("plots_2016/plot_"+shortNames[iHist]+"_2016B.pdf");
    else if(year==2016 and partial==2) myCan.SaveAs("plots_2016/plot_"+shortNames[iHist]+"_2016C.pdf");
    else if(year==2016 and partial==3) myCan.SaveAs("plots_2016/plot_"+shortNames[iHist]+"_2016D.pdf");
    else if(year==2016 and partial==4) myCan.SaveAs("plots_2016/plot_"+shortNames[iHist]+"_2016E.pdf");
    else if(year==2016 and partial==5) myCan.SaveAs("plots_2016/plot_"+shortNames[iHist]+"_2016F.pdf");
    else if(year==2016 and partial==6) myCan.SaveAs("plots_2016/plot_"+shortNames[iHist]+"_2016G.pdf");
    else if(year==2016 and partial==7) myCan.SaveAs("plots_2016/plot_"+shortNames[iHist]+"_2016H.pdf");
    else if(year==2016 and partial==8) myCan.SaveAs("plots_2016/plot_"+shortNames[iHist]+"_2016MC.pdf");
    else if(year==2016 and partial==12) myCan.SaveAs("plots_2016/plot_"+shortNames[iHist]+"_2016BC.pdf");
    else if(year==2016 and partial==34) myCan.SaveAs("plots_2016/plot_"+shortNames[iHist]+"_2016DE.pdf");
    else if(year==2016 and partial==56) myCan.SaveAs("plots_2016/plot_"+shortNames[iHist]+"_2016FG.pdf");
    else if(year==2016 and partial==123) myCan.SaveAs("plots_2016/plot_"+shortNames[iHist]+"_2016BCD.pdf");
    else if(year==2016 and partial==456) myCan.SaveAs("plots_2016/plot_"+shortNames[iHist]+"_2016EFG.pdf");
    else if(year==2016 and partial==4567) myCan.SaveAs("plots_2016/plot_"+shortNames[iHist]+"_2016EFGH.pdf");
  }

  return 0;
}
