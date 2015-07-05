#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <list>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include "TROOT.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TChain.h"
#include "TObject.h"
#include "TCanvas.h"
#include "TMath.h"
#include <math.h>
#include "TLegend.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TTree.h"
#include "TF1.h"
#include "TCutG.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TMultiGraph.h"
#include "TPaveText.h"
#include "THStack.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TString.h"

TGraphErrors *h1= new TGraphErrors;
TGraphErrors *h2= new TGraphErrors;
TGraphErrors *h3= new TGraphErrors;
TGraphErrors *h4 = new TGraphErrors;
TGraphErrors *h5 = new TGraphErrors;
TGraphErrors *h6 = new TGraphErrors;

  
void PlotWithRatio(TCanvas *c1, TGraphErrors *mcdymumu, TGraphErrors *mcdyee, TGraphErrors *datadymumu, TGraphErrors *datadyee, TGraphErrors *mcgjets, TGraphErrors *datagjets, TString EjeX) {


TH1::SetDefaultSumw2();


  c1->cd(); 
  c1->Print("~/www/plotdata.png");
    
  TMultiGraph *g = new TMultiGraph();

  datadymumu->SetMarkerStyle(22);
  datadymumu->SetMarkerColor(3);

  datadyee->SetMarkerStyle(23);
  datadyee->SetMarkerColor(2);
  
  datagjets->SetMarkerStyle(24);
  datagjets->SetMarkerStyle(4);
  

                 g->Add(datadymumu);
                 g->Add(datadyee);
                 g->Add(datagjets);

  //g->GetYaxis()->SetTitle("u_{||} (GeV)");
   
  //Since all the histograms should have the same binning, take the info from one of them
  double xmin=mcdymumu->GetX()[0];
  double xmax=mcdymumu->GetX()[mcdymumu->GetN()-1];
  double nbins=mcdymumu->GetN();
  
  TH1F * hmcdymumu= new TH1F("hmcdymumu","hmcdymumu", nbins,xmin,xmax);
  TH1F * hdatadymumu= new TH1F("hdatadymumu","hdatadymumu", nbins,xmin,xmax);
  TH1F * hmcdyee= new TH1F("hmcdyee","hmcdyee", nbins,xmin,xmax);
  TH1F * hdatadyee= new TH1F("hdatadyee","hdatadyee", nbins,xmin,xmax);
  TH1F * hmcgjets= new TH1F("hmcgjets","hmcgjets", nbins,xmin,xmax);
  TH1F * hdatagjets= new TH1F("hdatagjets","hdatagjets", nbins,xmin,xmax);


    for (int i = 0, n = datadymumu->GetN(); i < n; ++i) {
            
            //filling the histograms with the values in the tgraphs
            hdatadymumu->SetBinContent(i+1,datadymumu->GetY()[i]);
            hdatadyee->SetBinContent(i+1,datadyee->GetY()[i]);
            hdatagjets->SetBinContent(i+1,datagjets->GetY()[i]);
            hmcdymumu->SetBinContent(i+1,mcdymumu->GetY()[i]);
            hmcdyee->SetBinContent(i+1,mcdyee->GetY()[i]);
            hmcgjets->SetBinContent(i+1,mcgjets->GetY()[i]);
            //setting the errors
      	    hdatadymumu->SetBinError(i+1,datadymumu->GetErrorY(i));
            hdatadyee->SetBinError(i+1,datadyee->GetErrorY(i));
            hdatagjets->SetBinError(i+1,datagjets->GetErrorY(i));
            hmcdymumu->SetBinError(i+1,mcdymumu->GetErrorY(i));
            hmcdyee->SetBinError(i+1,mcdyee->GetErrorY(i));
            hmcgjets->SetBinError(i+1,mcgjets->GetErrorY(i));
            
    }

    TH1F *ratiodymumu=(TH1F*)hdatadymumu->Clone(); 
    TH1F *ratiodyee=(TH1F*)hdatadyee->Clone(); 
    TH1F *ratiogjets=(TH1F*)hdatagjets->Clone();
    
    ratiodymumu->Divide(hmcdymumu);
    ratiodyee->Divide(hmcdyee);
    ratiogjets->Divide(hmcgjets);
    
    TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
    pad1->SetBottomMargin(0);
    pad1->Draw();
    pad1->cd();
//    pad1->SetLogy();
    datadymumu->GetHistogram()->GetYaxis()->SetTitle(EjeX);
    
    
    g->Draw("AP");
    g->GetYaxis()->SetTitle("u_{||} [GeV]");
    g->GetYaxis()->SetLabelFont(63);
    g->GetYaxis()->SetLabelSize(16);
    g->GetYaxis()->SetTitleFont(63); //font in pixels
    g->GetYaxis()->SetTitleSize(20); //in 
    g->GetYaxis()->SetRangeUser(0,100);
    g->GetYaxis()->SetTitleOffset(1.5);
    
    TLegend* leg(0);
    leg = new TLegend(0.7,0.65,0.9,0.85);
    leg->SetFillColor(0);
    leg->AddEntry(datadymumu, "DY#rightarrow#mu#mu", "lp");
    leg->AddEntry(datadyee, "DY#rightarrowee", "lp");
    leg->AddEntry(datagjets,"#gamma+jets","lp");
    leg->Draw();
    c1->Modified();
                                                                   
         
    
    c1->cd();
    TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.3);
    pad2->SetTopMargin(0);
    pad2->Draw();
    pad2->cd();
    c1->Print("~/www/ratiobefore.png");
    TLine *line = new TLine(ratiodymumu->GetXaxis()->GetXmin(), 1.0, ratiodymumu->GetXaxis()->GetXmax(), 1.0);
    line->SetLineColor(205);
    line->SetLineWidth(2);
    line->SetLineStyle(2);

     ratiodymumu->GetYaxis()->SetTitle("Data/MC");
     ratiodymumu->GetXaxis()->SetTitle("qt [GeV]");
     ratiodymumu->SetTitle("");
     ratiodymumu->GetYaxis()->SetRangeUser(0.5, 1.5);
     
     ratiodymumu->SetMarkerStyle(22);
     ratiodymumu->SetMarkerColor(3);
     ratiodymumu->SetLineColor(3);
         
     ratiodyee->SetMarkerStyle(23);
     ratiodyee->SetMarkerColor(2);
     ratiodyee->SetLineColor(2);
                   
     ratiogjets->SetMarkerStyle(24);
     ratiogjets->SetMarkerStyle(4);
     ratiogjets->SetLineColor(4);      
     
     
     ratiodymumu->GetXaxis()->SetLabelFont(63); //font in pixels
     ratiodymumu->GetXaxis()->SetLabelSize(16); //in pixels
     ratiodymumu->GetYaxis()->SetLabelFont(63);
     ratiodymumu->GetYaxis()->SetLabelSize(16);
     ratiodymumu->GetXaxis()->SetTitleFont(63); //font in pixels
     ratiodymumu->GetXaxis()->SetTitleSize(20); //in pixels  
     ratiodymumu->GetYaxis()->SetTitleFont(63); //font in pixels
     ratiodymumu->GetYaxis()->SetTitleSize(20); //in pixels
     ratiodymumu->GetXaxis()->SetTitleOffset(1.5);
     ratiodymumu->GetYaxis()->SetTitleOffset(1.5);
     
     
     ratiodymumu->Draw("E2");
     line->Draw();

     ratiodyee->Draw("E SAMES P ");
     ratiogjets->Draw("E SAMES P ");
     
                                                                                                                                                                     
    
    pad2->Update();
    c1->Update();
    c1->cd();
    c1->Print("~/www/ratio.png");


}


void testRatio(TString histograma, TString EjeX) {

TCanvas *c1 = new TCanvas("c1","example",600,700);

TFile file1 ("DY_tgraphs.root"); // MC DY
TFile file2 ("pseudo_tgraphs.root");// data DY
//TFile file3 ("DY_tgraphs.root");
//TFile file4 ("pseudo_tgraphs.root");
TFile file3 ("Not_BKG_SubtractionGamma_tgraphs.root"); //gjets mc
TFile file4 ("BKG_SubtractionPseudo_tgraphs.root"); //data gjets

 h1 = (TGraphErrors*) file1.Get("MuMu_"+ histograma);
 h2 = (TGraphErrors*) file1.Get("EE_"+ histograma);
 h3 = (TGraphErrors*) file2.Get("MuMu_"+ histograma);
 h4 = (TGraphErrors*) file2.Get("EE_"+ histograma);
 h5 = (TGraphErrors*) file3.Get(histograma); // luego quita ee y mumu
 h6 = (TGraphErrors*) file4.Get(histograma); //luego quita ee y mumu
h1->Draw();
//h1->Rebin(2);
//h2->Rebin(2);
   gStyle->SetOptStat(0);
   gStyle->SetErrorX(0.5);
   PlotWithRatio(c1, h1, h2, h3,h4,h5,h6, EjeX);
}
