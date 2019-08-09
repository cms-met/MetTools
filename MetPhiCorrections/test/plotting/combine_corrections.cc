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

int combine_corrections(){

  //TFile* file = TFile::Open("histo_merged_DataDoubleMu2018A_v2.root");
  std::vector<TFile*> files;
  files.push_back(TFile::Open("histo_merged_DataDoubleMu2018A_v3.root"));
  files.push_back(TFile::Open("histo_merged_DataDoubleMu2018B_v3.root"));
  files.push_back(TFile::Open("histo_merged_DataDoubleMu2018C_v3.root"));
  files.push_back(TFile::Open("histo_merged_DataDoubleMu2018D_v3.root"));

  std::vector<TFile*> outFiles;
  outFiles.push_back(new TFile("outFile_combined_DataDoubleMu2018A_v3.root", "RECREATE"));
  outFiles.push_back(new TFile("outFile_combined_DataDoubleMu2018B_v3.root", "RECREATE"));
  outFiles.push_back(new TFile("outFile_combined_DataDoubleMu2018C_v3.root", "RECREATE"));
  outFiles.push_back(new TFile("outFile_combined_DataDoubleMu2018D_v3.root", "RECREATE"));


  for(unsigned int iFiles = 0 ; iFiles < files.size() ; iFiles++){
    files[iFiles]->cd();

    std::vector<TH1D*> histos_x;
    std::vector<TH1D*> histos_y;

    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_hEtaPlus_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_hEtaMinus_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_hHFPlus_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_hHFMinus_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_h0Barrel_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_h0EndcapPlus_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_h0EndcapMinus_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_gammaBarrel_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_gammaEndcapPlus_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_gammaEndcapMinus_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_egammaHFPlus_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_egammaHFMinus_Px"));


    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_hEtaPlus_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_hEtaMinus_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_hHFPlus_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_hHFMinus_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_h0Barrel_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_h0EndcapPlus_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_h0EndcapMinus_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_gammaBarrel_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_gammaEndcapPlus_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_gammaEndcapMinus_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_egammaHFPlus_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_egammaHFMinus_Py"));


    TH1D* histo_combined_x = new TH1D("histo_combined_x",";ngoodVertices;p_{x}(GeV)",100,0,100);
    TH1D* histo_combined_y = new TH1D("histo_combined_y",";ngoodVertices;p_{y}(GeV)",100,0,100);
    std::vector<double> binValues_x (101,0);
    std::vector<double> squaredBinErrors_x (101,0);
    std::vector<double> binValues_y (101,0);
    std::vector<double> squaredBinErrors_y (101,0);

    for (unsigned int iHist = 0 ; iHist < histos_x.size() ; iHist++){
      for (int iBin = 0 ; iBin <= histos_x[iHist]->GetNbinsX() ; iBin++){
        binValues_x[iBin] += histos_x[iHist]->GetBinContent(iBin);
        binValues_y[iBin] += histos_y[iHist]->GetBinContent(iBin);
        squaredBinErrors_x[iBin] += histos_x[iHist]->GetBinError(iBin)*histos_x[iHist]->GetBinError(iBin);
        squaredBinErrors_y[iBin] += histos_y[iHist]->GetBinError(iBin)*histos_y[iHist]->GetBinError(iBin);
        //std::cout << "File " << iFiles << ", histo " << histos_x[iHist]->GetName() << ", bin " << iBin << ": value is " << histos_x[iHist]->GetBinContent(iBin) << " and uncertainty is " << histos_x[iHist]->GetBinError(iBin) << ". Current sum is " << binValues_x[iBin] << " and current squared error is " << squaredBinErrors_x[iBin] << "." << std::endl;
      }
    }
    for (int iBin = 0 ; iBin <= histo_combined_x->GetNbinsX() ; iBin++){
      histo_combined_x->SetBinContent(iBin, binValues_x[iBin]);
      histo_combined_x->SetBinError(iBin, sqrt(squaredBinErrors_x[iBin]));
      histo_combined_y->SetBinContent(iBin, binValues_y[iBin]);
      histo_combined_y->SetBinError(iBin, sqrt(squaredBinErrors_y[iBin]));
    }

    //histo_combined_x->Draw();
    //histo_combined_y->Draw();
    outFiles[iFiles]->cd();
    histo_combined_x->Write();
    histo_combined_y->Write();
  }

  return 0;
}
