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


//TH1::SetDefaultSumw2();

TGraphErrors *h1= new TGraphErrors;
TGraphErrors *h2= new TGraphErrors;
TGraphErrors *h1up= new TGraphErrors;
TGraphErrors *h1down = new TGraphErrors;


  
void PlotWithRatio(TCanvas *c1, TGraphErrors *mcdy,  TGraphErrors *datady, TGraphErrors *mcdyup, TGraphErrors *mcdydown,  TString EjeX, TString histogramaname, TString channel) {


TH1::SetDefaultSumw2();


  c1->cd(); 
  //c1->Print("~/www/plotdata.png");
    
  TMultiGraph *g = new TMultiGraph();

  datady->SetMarkerStyle(22);
  datady->SetMarkerColor(4);
  
  mcdy->SetMarkerColor(2);
  

                 g->Add(datady);
                 g->Add(mcdy);

  //g->GetYaxis()->SetTitle("u_{||} (GeV)");
   
  //Since all the histograms should have the same binning, take the info from one of them
  double xmin=mcdy->GetX()[0];
  double xmax=mcdy->GetX()[mcdy->GetN()-1];
  double nbins=mcdy->GetN();
  
  TH1F * hmcdy= new TH1F("hmcdy","hmcdy", nbins,xmin,xmax);
  TH1F * hdatady= new TH1F("hdatady","hdatady", nbins,xmin,xmax);
  

  TH1F * hmcdyup= new TH1F("hmcdyup","hmcdyup", nbins,xmin,xmax);
  TH1F * hmcdydown= new TH1F("hmcdydown","hmcdydown", nbins,xmin,xmax);

    for (int i = 0, n = datady->GetN(); i < n; ++i) {
            
            //filling the histograms with the values in the tgraphs
            hdatady->SetBinContent(i+1,datady->GetY()[i]);
            hmcdy->SetBinContent(i+1,mcdy->GetY()[i]);
            hmcdydown->SetBinContent(i+1,mcdydown->GetY()[i]);
            hmcdyup->SetBinContent(i+1,mcdyup->GetY()[i]);
             
           
            //setting the errors
      	    hdatady->SetBinError(i+1,datady->GetErrorY(i));
            hmcdy->SetBinError(i+1,mcdy->GetErrorY(i));
          
            
          
            
    }

    TH1F *ratiody=(TH1F*)hdatady->Clone(); 
   
    ratiody->Divide(hmcdy);
   
    
    TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
    pad1->SetBottomMargin(0);
    
    pad1->Draw();
    pad1->cd();
//    pad1->SetLogy();
    datady->GetHistogram()->GetYaxis()->SetTitle(EjeX);
    
    
    TString ylabel="";
if (histogramaname=="met_uPara_zlloverzll_pt_vs_zll_pt") ylabel="-<u_{||}>/qt";
if (histogramaname=="met_uPara_zll+zll_pt_vs_zll_pt") ylabel="u_{||}+qt [GeV]";
if (histogramaname=="met_uPerp_zll_vs_zll_pt") ylabel="u_{#perp}  [GeV]";
if (histogramaname=="met_uPara_zll+zll_pt_vs_met_sumEt") ylabel="u_{||}+qt [GeV]";
if (histogramaname=="met_uPerp_zll_vs_met_sumEt")ylabel="u_{#perp}  [GeV]";
if (histogramaname=="met_uPara_zll+zll_pt_vs_nVert")ylabel="u_{||}+qt [GeV]";
if (histogramaname=="met_uPerp_zll_vs_nVert")ylabel="u_{#perp}  [GeV]";

    
    g->Draw("AP");
    g->GetYaxis()->SetTitle(ylabel);
    g->GetYaxis()->SetLabelFont(63);
    g->GetYaxis()->SetLabelSize(16);
    g->GetYaxis()->SetTitleFont(63); //font in pixels
    g->GetYaxis()->SetTitleSize(20); //in 
    g->GetYaxis()->SetRangeUser(0,2);
    if (!histogramaname.Contains("over")) g->GetYaxis()->SetRangeUser(0,40);
    g->GetYaxis()->SetTitleOffset(1.5);
    
    TLegend* leg(0);
    leg = new TLegend(0.7,0.65,0.9,0.85);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    if (channel=="MuMu")leg->AddEntry(mcdy, "DY#rightarrow#mu#mu", "lp");
    if (channel=="EE")leg->AddEntry(mcdy, "DY#rightarrowee", "lp");
    leg->AddEntry(datady, "Data", "lp");
    leg->Draw();
    c1->Modified();
                                                                   
         
    
    c1->cd();
    TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.2);
    pad2->Draw();
    pad2->cd();
    

     ratiody->GetYaxis()->SetTitle("Data/MC");
     ratiody->GetXaxis()->SetTitle(EjeX);
     ratiody->SetTitle("");
     ratiody->GetYaxis()->SetRangeUser(0.5, 1.5);
     
     ratiody->SetMarkerStyle(22);
     ratiody->SetMarkerColor(4);
     ratiody->SetLineColor(4);
     ratiody->GetXaxis()->SetLabelFont(63); //font in pixels
     ratiody->GetXaxis()->SetLabelSize(16); //in pixels
     ratiody->GetYaxis()->SetLabelFont(63);
     ratiody->GetYaxis()->SetLabelSize(16);
     ratiody->GetXaxis()->SetTitleFont(63); //font in pixels
     ratiody->GetXaxis()->SetTitleSize(20); //in pixels  
     ratiody->GetYaxis()->SetTitleFont(63); //font in pixels
     ratiody->GetYaxis()->SetTitleSize(20); //in pixels
     ratiody->GetXaxis()->SetTitleOffset(2.7);
     ratiody->GetYaxis()->SetTitleOffset(1.5);
     
     ratiody->SetFillColor(54);
    
     ratiody->SetFillStyle(3345);
  
     
     //==========  JES ============================================
     
     
     
     
     //============================================================
     
     TGraphAsymmErrors err= JESerror(hmcdy,hmcdydown,hmcdyup,hdatady);
     TGraphAsymmErrors staterr= staterror(hmcdy,hmcdydown,hmcdyup,hdatady);

     
    TAxis *err_xaxis = err.GetXaxis ();
	  TAxis *err_yaxis = err.GetYaxis ();

	  err_yaxis->SetRangeUser (0, 2);
	  err.SetTitle (0);
	
    err.SetFillColor (kRed);
    err.SetFillStyle (3003);
    
    TAxis *staterr_xaxis = staterr.GetXaxis ();
	  TAxis *staterr_yaxis = staterr.GetYaxis ();

	  staterr_yaxis->SetRangeUser (0, 3);
	  staterr.SetTitle (0);
	
    staterr.SetFillColor (kGreen);
    staterr.SetFillStyle (3003);
    
    
    ratiody->Draw("ep"); //"e2"
    err.Draw ("2 same");
    staterr.Draw("2 same");
     
    ratiody->SetMarkerSize (0.8);
    ratiody->SetMarkerStyle (20);
    TLine *lineR =  new TLine ( ratiody->GetXaxis ()->GetXmin (), 1, ratiody->GetXaxis ()->GetXmax (), 1);

     lineR->SetLineColor (kBlue + 1); lineR->SetLineWidth (2); lineR->SetLineStyle (2); lineR->Draw ();
         
         TLegend* legratio(0);
         legratio = new TLegend(0.70,0.75,0.90,0.95);
         legratio->SetFillColor(0);
         legratio->SetBorderSize(0);
         legratio->AddEntry(&staterr, "Stat","f");
         legratio->AddEntry(&err, "JES+Stat","f");
         legratio->Draw();
                                                     

     
                                                                                                                                                                     
    

    pad1->Modified ();
    pad2->Modified ();
    pad1->Update ();
    pad2->Update ();
                
    c1->Update();
    c1->cd();
    c1->Print("~/www/"+histogramaname+".png");


}


void testRatio(TString histograma, TString EjeX, TString channel) {


TCanvas *c1 = new TCanvas("c1","example",600,700);

TFile file1 ("Not_BKG_SubtractionDYtgraphs_DY"+channel+".root"); // MC DY
TFile file2 ("Not_BKG_SubtractionDatatgraphs_Data"+channel+".root");// data DY


TFile file1up ("Not_BKG_SubtractionDY_up_tgraphs_jes_DY"+channel+".root"); // MC DY up
TFile file1down ("Not_BKG_SubtractionDY_down_tgraphs_jes_DY"+channel+".root"); // MC DY down



 h1 = (TGraphErrors*) file1.Get(channel+"_"+ histograma); //mc mumu 
 h2 = (TGraphErrors*) file2.Get(channel+"_"+ histograma); //mc ee 
 
 TString histogramaup=histograma;
 TString histogramadown=histograma;
 if (histograma.Contains("Perp")) histogramaup.ReplaceAll("uPerp_zll","uPerp_zll_up");
 if (histograma.Contains("Para")) histogramaup.ReplaceAll("uPara_zll","uPara_zll_up");
 
 if (histograma.Contains("Perp")) histogramadown.ReplaceAll("uPerp_zll","uPerp_zll_down");
 if (histograma.Contains("Para")) histogramadown.ReplaceAll("uPara_zll","uPara_zll_down");


 h1up = (TGraphErrors*)  file1up.Get(channel+"_"+ histogramaup); //mc mumu 
 h1down = (TGraphErrors*) file1down.Get(channel+"_"+ histogramadown); //mc mumu


  
h1->Draw();


   gStyle->SetOptStat(0);
   gStyle->SetErrorX(0.5);
   PlotWithRatio(c1, h1, h2, h1up, h1down, EjeX, histograma, channel);
}



TGraphAsymmErrors JESerror(TH1F *Background, TH1F *Backgrounddown, TH1F *Backgroundup, TH1F * hdata){

   

    TH1F *den1 = (TH1F *) Background->Clone ("bkgden1");

	  TH1F *den2 = (TH1F *) Background->Clone ("bkgden2");
	  const Int_t nvar=12;// (const Int_t)Background->GetNbinsX ();
	  Double_t x[nvar];
	  Double_t y[nvar];
	  Double_t exl[nvar];
	  Double_t eyl[nvar];
	  Double_t exh[nvar];
	  Double_t eyh[nvar];
	  Double_t x1[nvar];
	  Double_t y1[nvar];
	  Double_t exl1[nvar];
	  Double_t eyl1[nvar];
	  Double_t exh1[nvar];
	  Double_t eyh1[nvar];



         TH1F *ratiop = (TH1F *) Background->Clone ("backgroundratiop");
         TH1F *ratiom = (TH1F *) Background->Clone ("backgroundratiom");

	  double ymin = 0.1, ymax = 2.;


	  for (int km = 0; km <= Background->GetNbinsX (); km++)
	    {

	      double conte1 =
		sqrt (Background->GetBinError (km) *
		      Background->GetBinError (km) +
		      (Background->GetBinContent (km) -
		       Backgroundup->GetBinContent (km)) *
		      (Background->GetBinContent (km) -
		       Backgroundup->GetBinContent (km)));
	      double conte2 =
		sqrt (Background->GetBinError (km) *
		      Background->GetBinError (km) +
		      (Background->GetBinContent (km) -
		       Backgrounddown->GetBinContent (km)) *
		      (Background->GetBinContent (km) -
		       Backgrounddown->GetBinContent (km)));


	      den1->SetBinContent (km,
				   Background->GetBinContent (km) + conte1);
	      den2->SetBinContent (km,
				   Background->GetBinContent (km) - conte2);


	      //      ymax = Background->GetBinContent(km) + conte1;
	      x1[km] = Background->GetBinCenter (km);
	      y1[km] = Background->GetBinContent (km);
	      exl1[km] = Background->GetBinWidth (km) / 2;
	      exh1[km] = Background->GetBinWidth (km) / 2;
	      eyl1[km] = conte2;
	      eyh1[km] = conte1;
	    }




	  ratiop->Divide (den1);
	  ratiom->Divide (den2);
	  
	  TH1F *ratio = (TH1F *) hdata->Clone ("ratiodata");
	  ratio->Divide (Background);

	  for (int km = 0; km <= ratio->GetNbinsX (); km++)
	    {
	      if (ratio->GetBinContent (km) > ymax)
     		ymax = ratio->GetBinContent (km) + ratio->GetBinError (km);
	      x[km] = ratio->GetBinCenter (km);
	      y[km] = 1;	
	      exl[km] = ratio->GetBinWidth (km) / 2;
	      exh[km] = ratio->GetBinWidth (km) / 2;


	      if (ratiop->GetBinContent (km) != 0)
		eyh[km] = 1. / ratiop->GetBinContent (km) - 1;
	      else
		eyh[km] = 0;

	      if (ratiom->GetBinContent (km) != 0)
		eyl[km] = 1 - 1. / ratiom->GetBinContent (km);
	      else
		eyl[km] = 0;



	    }



TGraphAsymmErrors *err = new TGraphAsymmErrors (nvar, x, y, exl, exh, eyl, eyh);


 return *err;


}

TGraphAsymmErrors staterror(TH1F *Background, TH1F *Backgrounddown, TH1F *Backgroundup, TH1F * hdata){

   

    TH1F *den1 = (TH1F *) Background->Clone ("bkgden1");

	  TH1F *den2 = (TH1F *) Background->Clone ("bkgden2");
	  const Int_t nvar=12;// (const Int_t)Background->GetNbinsX ();
	  Double_t x[nvar];
	  Double_t y[nvar];
	  Double_t exl[nvar];
	  Double_t eyl[nvar];
	  Double_t exh[nvar];
	  Double_t eyh[nvar];
	  Double_t x1[nvar];
	  Double_t y1[nvar];
	  Double_t exl1[nvar];
	  Double_t eyl1[nvar];
	  Double_t exh1[nvar];
	  Double_t eyh1[nvar];



         TH1F *ratiop = (TH1F *) Background->Clone ("backgroundratiop");
         TH1F *ratiom = (TH1F *) Background->Clone ("backgroundratiom");

	  double ymin = 0.1, ymax = 2.;


	  for (int km = 0; km <= Background->GetNbinsX (); km++)
	    {

	      double conte1=Background->GetBinError (km) ;
		      
	      double conte2 =Background->GetBinError (km);
		      


	      den1->SetBinContent (km,
				   Background->GetBinContent (km) + conte1);
	      den2->SetBinContent (km,
				   Background->GetBinContent (km) - conte2);


	      //      ymax = Background->GetBinContent(km) + conte1;
	      x1[km] = Background->GetBinCenter (km);
	      y1[km] = Background->GetBinContent (km);
	      exl1[km] = Background->GetBinWidth (km) / 2;
	      exh1[km] = Background->GetBinWidth (km) / 2;
	      eyl1[km] = conte2;
	      eyh1[km] = conte1;
	    }




	  ratiop->Divide (den1);
	  ratiom->Divide (den2);
	  
	  TH1F *ratio = (TH1F *) hdata->Clone ("ratiodata");
	  ratio->Divide (Background);

	  for (int km = 0; km <= ratio->GetNbinsX (); km++)
	    {
	      if (ratio->GetBinContent (km) > ymax)
     		ymax = ratio->GetBinContent (km) + ratio->GetBinError (km);
	      x[km] = ratio->GetBinCenter (km);
	      y[km] = 1;	
	      exl[km] = ratio->GetBinWidth (km) / 2;
	      exh[km] = ratio->GetBinWidth (km) / 2;


	      if (ratiop->GetBinContent (km) != 0)
		eyh[km] = 1. / ratiop->GetBinContent (km) - 1;
	      else
		eyh[km] = 0;

	      if (ratiom->GetBinContent (km) != 0)
		eyl[km] = 1 - 1. / ratiom->GetBinContent (km);
	      else
		eyl[km] = 0;



	    }



TGraphAsymmErrors *err = new TGraphAsymmErrors (nvar, x, y, exl, exh, eyl, eyh);


 return *err;


}


