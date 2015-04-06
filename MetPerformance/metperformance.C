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
#include "TString.h"

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

using namespace RooFit;

double FWHM (double, double);
double FWHMError (double, double, double, double, double, double, double,
		  double);
void
metperformance (TString samplephys14, TString variablename, TString xvariable, TString tchannel, bool drawchi2)
{

  TString variablenamepng=variablename;
  variablenamepng.ReplaceAll("/","over");

  TH1::SetDefaultSumw2() ;

  TString folder = "DY";
  if (samplephys14.Contains ("TT"))
    folder = "TTbar";
  if (samplephys14.Contains ("GJet"))
    folder = "Gamma";


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

  int sizexarray = 0;

  if (xvariable == "nvtx")
    sizexarray = 6;
  if (xvariable == "sumEt")
    sizexarray = 6;
  if (xvariable == "qt")
    sizexarray = 10;




  Double_t tgraphx[sizexarray], tgraphy[sizexarray], etgraphy[sizexarray],
    etgraphx[sizexarray], tgraphxchi2[sizexarray], tgraphychi2[sizexarray];

  for (int index = 0; index < sizexarray; index++)
    {
      if (xvariable == "nvtx")
	limitup = (index + 1) * 5;
      if (xvariable == "sumEt")
	limitup = (index + 1) * 200;
      if (xvariable == "qt")
	limitup = (index + 1) * 12;
      strlimitup = Form ("%d", limitup);

      resolution.push_back (new TH1F (Form ("resx%d", index), " ", 50, -200, 200));
      
       
      TString dileptonch="";
      if (tchannel=="MuMu")  dileptonch="1";
      if (tchannel=="EE") dileptonch="0";

      TString condition="(weighttotal)*(channel=="+dileptonch +")*";
      if (tchannel == "Gamma") condition="";
      if (xvariable == "nvtx") condition += "(" + xvariable + "==" + strlimitup +")";
      else condition += "(" + xvariable + "<" + strlimitup + ")*(" + xvariable + ">" + strlimitdown + ")";

      treephys14->Draw (variablename + ">>" + TString (resolution[index]->GetName ()),
			condition.Data(),"sames");

      double m =  resolution[index]->GetMean ();
      double um = resolution[index]->GetMean () - resolution[index]->GetRMS ();
      double uM = resolution[index]->GetMean () + resolution[index]->GetRMS ();





      ////////



      RooRealVar x ("x", "x", -200, 200);
      RooDataHist Hist ("Hist", "Hist", x,
			(TH1 *) resolution[index]->Clone ());
      RooRealVar g_w ("g_w", "width Gaus", 10., 0., 100., "GeV");	//40
      RooRealVar gamma_Z0 ("gamma_Z0_U", "Z0 width", 2.3, 0, 100, "GeV");	//20
      RooRealVar v_m ("v_m", "v_m", m, um, uM, "GeV");
      RooVoigtian *voigt =
	new RooVoigtian ("voigt", "Voigtian", x, v_m, gamma_Z0, g_w);

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
      cout << "correlacion Vgs " << Vgs << " y correlación Vsg" << Vsg <<
	endl;
      double f = FWHM (sigma, gamma);
      double efwhm =	FWHMError (sigma, gamma, esigma, egamma, Vss, Vsg, Vgs, Vgg);

      //if (f/2.3 < 5) continue;
      RooPlot *xFrame = x.frame ();
      (Hist).plotOn (xFrame);
      TString titlexfit = "";
      if (variablename == "pfmetx")
	titlexfit = "MET_{x} (GeV)";
      if (variablename == "pfmety")
	titlexfit = "MET_{y} (GeV)";
      xFrame->SetXTitle (titlexfit);
      voigt->plotOn (xFrame);
      
      xFrame->Draw ();
      TString histoname = resolution[index]->GetName ();
      
              
      c1->Print ("~/www/METfits/" + folder + "/" + tchannel +"/" + histoname + "_" +	variablenamepng + "_vs_" + xvariable + ".png");

      //Print chi2/dof value

      Double_t chi2 = xFrame->chiSquare ();	//"voigt", "Hist", 3);
      //cout << "chi2 = " << chi2 << endl;


      
   
      tgraphx[index] = index;

      if (xvariable == "nvtx")
	tgraphx[index] = limitup;
      if (xvariable == "sumEt")
	tgraphx[index] = limitup * 0.001;	//For the x axis to be in TEV
      if (xvariable == "qt")
	tgraphx[index] = limitup;

      tgraphxchi2[index] = tgraphx[index];
      tgraphychi2[index] = chi2;

      if (chi2 != chi2 || chi2 >= 100)
    	tgraphychi2[index] = -0.2;
      tgraphy[index] = f / 2.3548;
      if (variablename == "uparaqt" || variablename == "upararawqt")
	    tgraphy[index] = -resolution[index]->GetMean (); 
      etgraphy[index] = efwhm / 2.3548;
      if (variablename == "uparaqt" || variablename == "upararawqt")
	    etgraphy[index] = resolution[index]->GetMeanError ();
      etgraphx[index] = 0;


      //Set limit down
      limitdown = limitup;
      strlimitdown = Form ("%d", limitdown);


    }




  TGraph *gr =  new TGraphErrors (sizexarray, tgraphx, tgraphy, etgraphx, etgraphy);
  gr->SetMarkerColor (4);
  gr->SetMarkerStyle (21);

  TGraph *grchi2 = new TGraph (sizexarray, tgraphxchi2, tgraphychi2);
  grchi2->SetMarkerColor (2);
  grchi2->SetMarkerStyle (34);
  
  
  if (xvariable == "sumEt")
    {
      gr->GetXaxis ()->SetTitle ("sumE_{T} (TeV)");
      grchi2->GetXaxis ()->SetTitle ("sumE_{T} (TeV)");
    }
  if (xvariable == "nvtx")
    {
      gr->GetXaxis ()->SetTitle ("Number of Vertices");
      grchi2->GetXaxis ()->SetTitle ("Number of Vertices");
    }
  if (xvariable == "qt")
    {
      gr->GetXaxis ()->SetTitle ("qt (GeV)");
      grchi2->GetXaxis ()->SetTitle ("qt (GeV)");
    }



  gr->GetYaxis ()->SetTitle (titley);
  if (variablename.Contains ("upara"))
  gr->GetYaxis ()->SetTitle ("#sigma(u_{||}) [GeV]");
  if (variablename.Contains("uperp"))
  gr->GetYaxis ()->SetTitle ("#sigma(u_{#perp}  ) [GeV]");
  

  if (drawchi2) {
  grchi2->GetYaxis ()->SetTitle ("#Chi^{2}");
  grchi2->Draw ("AP");
  c1->Print ("~/www/METResolution/" + folder + "/" + tchannel + "/" +  variablenamepng  + "_vs_" +	     xvariable + "_chi2.png");
  c1->Clear (); }

  TFile f (folder + "_tgraphs.root", "UPDATE");
  gr->Write (variablename + "_vs_" + xvariable);


  c1->Update ();

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

  
  

  gr->Draw ("AP");
  if (variablename!="uparaqt" && variablename!="upararawqt") gr->GetYaxis()->SetRangeUser(-40,40);
  else gr->GetYaxis()->SetRangeUser(0.5,1.1);
  c1->Update();
  
  
  if (variablename == "uparaqt" || variablename=="upararawqt")
    {
      TLine *lineR =  new TLine ( gr->GetHistogram ()->GetXaxis ()->GetXmin (), 1, gr->GetHistogram ()->GetXaxis ()->GetXmax (), 1);
      lineR->SetLineColor (kBlue + 1);
      lineR->SetLineWidth (2);
      lineR->SetLineStyle (2);
      lineR->Draw ();
    }

  else
    {
      
      TLine *lineR =	new TLine (gr->GetHistogram ()->GetXaxis ()->GetXmin (), 0,  gr->GetHistogram ()->GetXaxis ()->GetXmax (), 0);
      lineR->SetLineColor (kBlue + 1);
      lineR->SetLineWidth (2);
      lineR->SetLineStyle (2);
      lineR->Draw ();


    }

  

  c1->Print ("~/www/METResolution/" + folder + "/" + tchannel + "/" + variablenamepng  + "_vs_" +	     xvariable + ".png");



}



double
FWHM (double sigma, double gamma)
{

  double f_g = 2 * sigma * sqrt (2 * log (2));
  double f_l = 2 * gamma;

  return 0.5346 * 2 * gamma + sqrt (0.2166 * f_l * f_l + f_g * f_g);
}



double
FWHMError (double sigma, double gamma, double esigma, double egamma,
	   double Vss, double Vsg, double Vgs, double Vgg)
{


  double a = 0.5346;
  double b = 0.2166;
  double ef_g = 2 * esigma * sqrt (2 * log (2));
  double ef_l = 2 * egamma;

  double dg =
    2 * a + 4 * b * gamma / sqrt (4 * b * pow (gamma, 2) +
				  4 * pow (sigma, 2) * log (2));

  double ds =
    (sigma * log (4)) / sqrt (b * pow (gamma, 2) + pow (sigma, 2) * log (2));
  
  double p1 = ef_l * ef_l * Vgg * dg;
  double p2 = ef_g * ef_l * Vsg * dg * ds;	//identical (should be)
  double p3 = ef_g * ef_l * Vgs * dg * ds;
  double p4 = ef_g * ef_g * Vss * ds;

  return sqrt (abs (p1) + abs (p2) + abs (p3) + abs (p4));

}

