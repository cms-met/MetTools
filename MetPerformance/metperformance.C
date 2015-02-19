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

//#include "METFunctions.hh"

#include "RooRealVar.h"
#include <RooDataSet.h>
#include <RooDataHist.h>
#include <RooFitResult.h>
//#include <RooGaussian.h>
#include <RooAddPdf.h>
#include <RooPlot.h>
#include <TStyle.h>
#include <RooVoigtian.h>
#include <TSystem.h>

double FWHM (double, double);
double FWHMError (double,double,double,double,double,double);
void
metperformance (TString samplephys14, TString variablename)
{

  TString folder = "DY";
  if (samplephys14.Contains ("TT"))
    folder = "TTbar";


  TString titley = "";
  if (variablename == "pfmetx")
    titley = "#sigma(MET_{x}) GeV";
  if (variablename == "pfmety")
    titley = "#sigma(MET_{y}) GeV";
//gSystem->Load("libRooFit") ;
  gSystem->Load ("libRooFit");
  gSystem->Load ("RooRealVar");

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
  


  TTree *treephys14 = (TTree *) filephys14.Get ("Events");
  


  std::vector < TH1F * >resolution;
  resolution.clear ();
  



  int limitdown (0), limitup (0);
  TString strlimitup = "0";
  TString strlimitdown = "0";


//treephys14->Draw (variablename + ">>histoprueba", "(channel==1)*(sumEt> 400)*(sumEt< 500) ");

  Double_t tgraphx[25], tgraphy[25], etgraphy[25], etgraphx[25];

  for (int index = 0; index <= 24; index++)
    {

//if (index !=11) continue;

//cout << " y vuelve a empezar " << endl;
      limitup = (index + 1) * 100;
      strlimitup = Form ("%d", limitup);

      resolution.push_back (new TH1F (Form ("resx%d", index), " ", 50, -200, 200));
      



      treephys14->Draw (variablename + ">>" +			TString (resolution[index]->GetName ()),			"(channel==1)*(sumEt<"			+ strlimitup + ")*(channel==1)*(sumEt>" +			strlimitdown + ")", "sames");
      

      limitdown = limitup;
      strlimitdown = Form ("%d", limitdown);



      double m = resolution[index]->GetMean ();
      double um =	resolution[index]->GetMean () - resolution[index]->GetRMS ();
      double uM =	resolution[index]->GetMean () + resolution[index]->GetRMS ();
      double meU = 0;		//-1* ( bVar[iqt+1] + bVar[iqt] )/2.;
      double mU = -50;		//qt -100;
      double MU = 50;		//qt +100;



      ////////



      RooRealVar x ("x", "x", -200, 200);
      RooDataHist Hist ("Hist", "Hist", x,
			(TH1 *) resolution[index]->Clone ());
      RooRealVar g_w ("g_w", "width Gaus", 10., 0., 100., "GeV");	//40
      RooRealVar gamma_Z0 ("gamma_Z0_U", "Z0 width", 2.3, 0, 100, "GeV");	//20
      RooRealVar v_m ("v_m", "v_m", m, um, uM, "GeV");
      RooVoigtian *voigt =
	new RooVoigtian ("voigt", "Voightian", x, v_m, gamma_Z0, g_w);

      RooFitResult *result = voigt->fitTo ((Hist), RooFit::SumW2Error (kFALSE), RooFit::Save (kTRUE), RooFit::PrintLevel (-1));	// -1 verbose

      //Get the FWHM
      double sigma = g_w.getVal ();
      double gamma = gamma_Z0.getVal ();
      double esigma = g_w.getError ();
      double egamma = gamma_Z0.getError ();

      double Vsg = result->correlation (g_w, gamma_Z0);
      double Vgs = result->correlation (gamma_Z0, g_w);
      double Vss = result->correlation (g_w, g_w);
      double Vgg = result->correlation (gamma_Z0, gamma_Z0);

      double f = FWHM (sigma, gamma);
      double efwhm = FWHMError( esigma, egamma, Vss, Vsg, Vgs, Vgg);
  
      cout << " f value " << f << endl;
      if (f/2.3 < 5) continue;
      RooPlot *xFrame = x.frame ();
      (Hist).plotOn (xFrame);
      TString titlexfit = "";
      if (variablename == "pfmetx")
	titlexfit = "MET_{x} (GeV)";
      if (variablename == "pfmety")
	titlexfit = "MET_{y} (GeV)";
      xFrame->SetXTitle (titlexfit);
      voigt->plotOn (xFrame);
      //sum.plotOn(xFrame, CMS.RooFit::Components(expo), CMS.RooFit::LineStyle(kDashed)) ;
      xFrame->Draw ();
      TString histoname = resolution[index]->GetName ();
      c1->Print ( histoname + "_" + folder +"_"+ variablename + ".png");



      cout << " index " << index << " -- sigma " << sigma << endl;
      cout << "index " << index << " -- gamma " << gamma << endl;

      tgraphx[index] = index * 0.1;
      tgraphy[index] = f / 2.3548;
      etgraphy[index] = efwhm/ 2.3548;
      etgraphx[index] = 0;
      c1->Print ("~/www/prueba.png");

    }




  TGraph *gr = new TGraphErrors (25, tgraphx, tgraphy,etgraphx,etgraphy);
  gr->SetMarkerColor(4);
  gr->SetMarkerStyle(21);

  gr->GetXaxis ()->SetTitle ("sumE_{T} (TeV)");
  gr->GetYaxis ()->SetTitle (titley);
  gr->Draw ("AP");

  c1->Update ();
  TLegendEntry *legge;
  TLegend *leg;

  leg = new TLegend (0.60, 0.6, 0.91, 0.81);
  leg->SetFillStyle (0);
  leg->SetBorderSize (0);
  leg->SetTextSize (0.04);
  leg->SetTextFont (42);
  
  

  leg->SetFillColor (0);





  // leg->Draw ();
  TLatex l1;
  l1.SetTextAlign (12);
  l1.SetTextSize (0.04);
  l1.SetNDC ();
  l1.DrawLatex (0.155, 0.98, "CMS Preliminary, #sqrt{s} = 13 TeV");


  // c1->Modified ();
  //c1->Update ();


  c1->Print ( variablename +"_"+folder+ ".png");



}



double FWHM (double sigma, double gamma)
{

  double f_g = 2 * sigma * sqrt (2 * log (2));
  double f_l = 2 * gamma;

  return 0.5346 * 2 * gamma + sqrt (0.2166 * f_l * f_l + f_g * f_g);
}



double FWHMError( double esigma, double egamma, double Vss, double Vsg, double Vgs, double Vgg){
  double ef_g =  2*esigma*sqrt(2*log(2));
    double ef_l = 2* egamma;
      
        double p1 = ef_l*ef_l*Vgg;
          double p2 = ef_g*ef_l*Vsg; 
            double p3 = ef_g*ef_l*Vgs;
              double p4 = ef_g*ef_g*Vss;
                
                  return sqrt(p1 + p2 + p3 + p4);
                  }
