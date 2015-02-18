#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
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



void
scriptcompare (TString samplephys14, TString samplecsa14,
	       TString variablename)
{



  gStyle->SetOptStat (0);
  gStyle->SetCanvasColor (0);
  gStyle->SetPadColor (0);
  gStyle->SetPalette (1);
  //gStyle->SetMarkerStyle (kFullCircle);
  // gStyle->SetMarkerSize (1);
  gStyle->SetTextFont (42);
  gStyle->SetMarkerColor (1);
  gStyle->SetOptTitle (0);

  TCanvas *c1 = new TCanvas ("c1", "c1", 800, 800);

  c1->SetTickx ();
  c1->SetTicky ();
  c1->SetFillColor (kWhite);
  c1->SetFillStyle (0);
  c1->SetRightMargin (0.05);
  c1->SetTopMargin (0.08);

  


  TFile filephys14 (samplephys14);
  TFile filecsa14 (samplecsa14);


  TTree *treephys14 = (TTree *) filephys14.Get ("Events");
  TTree *treecsa14 = (TTree *) filecsa14.Get ("Events");

  TH1D *histocsa14;
  TH1D *histophys14;

  if (variablename == "pfmet")
    {

      histocsa14 = new TH1D ("histocsa14", "", 50, 0, 200);
      histophys14 = new TH1D ("histophys14", "", 50, 0, 200);

    }


  if (variablename == "qt")
    {

      histocsa14 = new TH1D ("histocsa14", "", 50, 0, 200);
      histophys14 = new TH1D ("histophys14", "", 50, 0, 200);

    }


  if (variablename == "nvtx")
    {

      histocsa14 = new TH1D ("histocsa14", "", 50, 0, 50);
      histophys14 = new TH1D ("histophys14", "", 50, 0, 50);

    }



  if (variablename == "invmass")
    {

      histocsa14 = new TH1D ("histocsa14", "", 50, 0, 200);
      histophys14 = new TH1D ("histophys14", "", 50, 0, 200);

    }



  if (variablename == "upara")
    {

      histocsa14 = new TH1D ("histocsa14", "", 50, -200, 200);
      histophys14 = new TH1D ("histophys14", "", 50, -200, 200);

    }

  if (variablename == "uperp")
    {

      histocsa14 = new TH1D ("histocsa14", "", 50, -200, 200);
      histophys14 = new TH1D ("histophys14", "", 50, -200, 200);


    }




  treecsa14->Draw (variablename + ">>histocsa14", "(channel==1)");
  treephys14->Draw (variablename + ">>histophys14", "(channel==1)");


  histocsa14->SetLineColor (2);
  histocsa14->SetLineWidth (2);
  histophys14->SetLineWidth (2);



  cout << "llega   " << endl;
  TLegendEntry *legge;
  TLegend *leg;

  leg = new TLegend (0.60, 0.6, 0.91, 0.81);
  leg->SetFillStyle (0);
  leg->SetBorderSize (0);
  leg->SetTextSize (0.04);
  leg->SetTextFont (42);
  legge = leg->AddEntry (histophys14, "PHYS14", "l");
  legge = leg->AddEntry (histocsa14, "CSA14", "l");


  leg->SetFillColor (0);



 TAxis *histocsa14_xaxis = histocsa14->GetXaxis ();
   TAxis *histophys14_xaxis = histophys14->GetXaxis ();
   
     TAxis *histocsa14_yaxis = histocsa14->GetYaxis ();
       TAxis *histophys14_yaxis = histophys14->GetYaxis ();
       
       
         histophys14_xaxis->SetTitle (variablename + " [GeV]");
           histocsa14_xaxis->SetTitle (variablename + " [GeV]");
           
             if (variablename == "nvtx")
                 {
                 
                       histophys14_xaxis->SetTitle (variablename);
                             histocsa14_xaxis->SetTitle (variablename);
                             
                             
                                 }
                                   //histophys14_yaxis->SetTitle ("Normalized Events");
                                   //histocsa14_yaxis->SetTitle ("Normalized Events");
                                     

  histophys14_yaxis->SetTitleOffset (1.5);
  histocsa14_yaxis->SetTitleOffset (1.7);
  histophys14->DrawNormalized ();
  histocsa14->DrawNormalized ("sames");

  

  leg->Draw ();
  TLatex l1;
  l1.SetTextAlign (12);
  l1.SetTextSize (0.04);
  l1.SetNDC ();
  l1.DrawLatex (0.155, 0.98, "CMS Preliminary, #sqrt{s} = 13 TeV");


  c1->Modified ();
  c1->Update ();


  

  c1->Print (variablename + ".png");


}
