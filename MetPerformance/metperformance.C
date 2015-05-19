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
#include "TSystem.h"

//#include "METFunctions.hh"

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooFitResult.h"
//#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooPlot.h"
#include "RooVoigtian.h"


using namespace RooFit;
                           

double FWHM (double, double);
double FWHMError (double, double, double, double, double, double, double,
		  double);
double FWHMError_fixed (double, double, double, double, double, double, double,
		  double);



RooRealVar x ("x", "x", -800, 800);
RooRealVar g_w ("g_w", "width Gaus", 10., 0., 100., "GeV");	//40
RooRealVar gamma_Z0 ("gamma_Z0_U", "Z0 width", 2.3, 0, 100, "GeV");	//20
RooRealVar v_m ("v_m", "v_m",0,-10.,10.);

RooVoigtian *voigt;
RooFitResult *result;

double f;
double efwhm;

void constructModel(RooDataHist Hist,double m,double um,double uM) {

  f=0;
  efwhm=0;

  v_m.setVal(m);
  v_m.setRange(um,uM);

  voigt =new RooVoigtian ("voigt", "Voigtian", x, v_m, gamma_Z0, g_w);

  //      RooFitResult *result = voigt->fitTo ((Hist), RooFit::SumW2Error (kFALSE), RooFit::Save (kTRUE), RooFit::PrintLevel (-1));	// -1 verbose
  result = voigt->fitTo (Hist, RooFit::Minimizer("Minuit2","migrad"),RooFit::Strategy(2), RooFit::SumW2Error (kFALSE), RooFit::Save (kTRUE), RooFit::PrintLevel (-1));// -1 verbose
  //https://root.cern.ch/phpBB3/viewtopic.php?f=15&t=16764
  //status=0 ok
  if(result->status()!=0) voigt=0;

  //Get the FWHM
  double sigma = g_w.getVal ();
  double gamma = gamma_Z0.getVal ();
  double esigma = g_w.getError ();
  double egamma = gamma_Z0.getError ();

  double Vsg = result->correlation (g_w, gamma_Z0);
  double Vgs = result->correlation (gamma_Z0, g_w);
  double Vss = result->correlation (g_w, g_w);
  double Vgg = result->correlation (gamma_Z0, gamma_Z0);
  cout << "correlacion Vgs " << Vgs << " y correlacion Vsg" << Vsg << endl;
  double f = FWHM (sigma, gamma);
  double efwhm =	FWHMError (sigma, gamma, esigma, egamma, Vss, Vsg, Vgs, Vgg);

  return;

}


void
metperformance (TString samplephys14, TString variablename, TString xvariable, TString tchannel,		bool drawchi2)
{

  //gROOT->ProcessLine(".L tdrstyle.C");
  //setTDRStyle();
         gROOT->SetStyle("tdrStyle");
         gROOT->ForceStyle(true);
                                                  
  
  
  TString variablenamepng=variablename;
  variablenamepng.ReplaceAll("/","over");



  TH1::SetDefaultSumw2() ;

   

  TString folder = "DY";
  if (samplephys14.Contains ("TT"))
    folder = "TTbar";


  TString titley = "";
  if (variablename == "pfmetx")
    titley = "#sigma(MET_{x}) GeV";
  if (variablename == "pfmety")
    titley = "#sigma(MET_{y}) GeV";
  //  gSystem->Load ("libRooFit");
  //  gSystem->Load ("RooRealVar");

/*  gStyle->SetOptStat (0);
  gStyle->SetCanvasColor (0);
  gStyle->SetPadColor (0);
  gStyle->SetPalette (1);
  //gStyle->SetMarkerStyle (kFullCircle);
  // gStyle->SetMarkerSize (1);
  gStyle->SetTextFont (42);
  gStyle->SetMarkerColor (1);
  gStyle->SetOptTitle (0);

*/
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
    
    
    TH1F *histonvertex=new TH1F("histonvertex","",50,0,50);
    TH1F *histoqt=new TH1F("histoqt","",100,0,1200);
    TH1F *histosumEt=new TH1F("histosumEt","",100,0,4);
    TH1F *histouparaqt=new TH1F("histouparaqt","",50,-300,300);
    TH1F *histouperp=new TH1F("histouperp","",50,-300,300);
    
    


    TString dileptonch="";
    if (tchannel=="MuMu")  dileptonch="1";
    if (tchannel=="EE") dileptonch="0";
     

     // Plot inclusive distributions of the main variables
      treephys14->Draw ("nvtx >> histonvertex","(weighttotal)*(channel=="+ dileptonch+")", "sames");
       treephys14->Draw ("qt >> histoqt","(weighttotal)*(channel=="+ dileptonch+")", "sames");
        treephys14->Draw ("sumEt/1000 >> histosumEt","(weighttotal)*(channel=="+ dileptonch+")", "sames");
         treephys14->Draw ("upara+qt >> histouparaqt","(weighttotal)*(channel=="+ dileptonch+")", "sames");
          treephys14->Draw ("uperp >> histouperp","(weighttotal)*(channel=="+ dileptonch+")", "sames");
          
          
          histonvertex->Draw();
          histonvertex->GetXaxis ()->SetTitle ("Number of Vertices");
          c1->Print ("~/www/METResolution/" + folder + "/"+tchannel +"/nvtx_inclusive_.png");
          c1->SetLogy();
          histoqt->Draw();
          histoqt->GetXaxis ()->SetTitle ("qt [GeV]");
          c1->Print ("~/www/METResolution/" + folder + "/"+tchannel +"/qt_inclusive_.png");
          histosumEt->Draw();
          histosumEt->GetXaxis ()->SetTitle("sumE_{T} [TeV]");
          c1->Print ("~/www/METResolution/" + folder + "/"+tchannel +"/sumEt_inclusive_.png");
          histouparaqt->Draw();
          histouparaqt->GetXaxis ()->SetTitle ("u_{||}+qt [GeV]");
          c1->Print ("~/www/METResolution/" + folder + "/"+tchannel +"/uparaqt_inclusive_.png");
          histouperp->Draw();
          histouperp->GetXaxis ()->SetTitle ("u_{#perp}   [GeV]");
          c1->Print ("~/www/METResolution/" + folder + "/"+tchannel +"/uperp_inclusive_.png");
          c1->SetLogy(0);

      Double_t tgraphx[sizexarray], tgraphy[sizexarray], etgraphy[sizexarray], etgraphx[sizexarray], tgraphxchi2[sizexarray], tgraphychi2[sizexarray];

  for (int index = 0; index < sizexarray; index++)
    {
      if (xvariable == "nvtx")
	limitup = (index + 1) * 5;
      if (xvariable == "sumEt")
	limitup = (index + 1) * 200;
      if (xvariable == "qt")
	limitup = (index + 1) * 12;
      strlimitup = Form ("%d", limitup);

      resolution.push_back (new TH1F (Form ("resx%d", index), " ", 200, -800, 800));


      //      TString condition="(weighttotal)*(channel=="+ dileptonch +")*(" + xvariable + "<" + strlimitup + ")*(" + xvariable + ">=" + strlimitdown + ")";

      TString condition="(weighttotal)*(channel=="+dileptonch +")*";
      //      if (tchannel == "Gamma") condition="(weighttotal)*";
      if (xvariable == "nvtx") condition += "(" + xvariable + "==" + strlimitup +")";
      else condition += "(" + xvariable + "<" + strlimitup + ")*(" + xvariable + ">" + strlimitdown + ")";

      treephys14->Draw (variablename + ">>" + TString (resolution[index]->GetName ()),
			condition.Data(), "sames");


      double m  = resolution[index]->GetMean ();
      double um = resolution[index]->GetMean () - resolution[index]->GetRMS ();
      double uM = resolution[index]->GetMean () + resolution[index]->GetRMS ();


      ////////

      RooDataHist Hist ("Hist", "Hist", x,
			(TH1 *) resolution[index]->Clone ());

      // construct the voightian model
      // fit the Hist Dataset also
      // fill f and efwhm that are the parameter of the voightian
      constructModel(Hist,m,um,uM);

      //if (f/2.3 < 5) continue;
      RooPlot *xFrame = x.frame ();
      (Hist).plotOn (xFrame);
      TString titlexfit = "";
      if (variablename == "pfmetx")
	titlexfit = "MET_{x} [GeV]";
      if (variablename == "pfmety")
	titlexfit = "MET_{y} [GeV]";
      xFrame->SetXTitle (titlexfit);

      int color=kBlack;
      if (xvariable == "nvtx")
	color = kRed;
      if (xvariable == "sumEt")
	color = kGreen+1;
      if (xvariable == "qt")
	color = kBlue;

      if(voigt) voigt->plotOn(xFrame,RooFit::FillColor(kGray),VisualizeError(*result,1),RooFit::Components(*voigt)); // 1 sigma band in gray
      if(voigt) voigt->plotOn(xFrame,RooFit::LineColor(color));
      //      voigt->paramOn(xFrame, Format("NELU", AutoPrecision(2)), Layout(0.1, 0.45,0.99) ); // to add the legend of parameters

      c1->cd();
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
  //gr->SetMarkerColor (4);
  //gr->SetMarkerStyle (21);

  TGraph *grchi2 = new TGraph (sizexarray, tgraphxchi2, tgraphychi2);
  //grchi2->SetMarkerColor (2);
  //grchi2->SetMarkerStyle (34);
  
  
  if (xvariable == "sumEt")
    {
      gr->GetXaxis ()->SetTitle ("sumE_{T} [TeV]");
      grchi2->GetXaxis ()->SetTitle ("sumE_{T} [TeV]");
    }
  if (xvariable == "nvtx")
    {
      gr->GetXaxis ()->SetTitle ("Number of Vertices");
      grchi2->GetXaxis ()->SetTitle ("Number of Vertices");
    }
  if (xvariable == "qt")
    {
      gr->GetXaxis ()->SetTitle ("qt [GeV]");
      grchi2->GetXaxis ()->SetTitle ("qt [GeV]");
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


double
FWHMError_fixed (double sigma, double gamma, double esigma, double egamma,
	   double Vss, double Vsg, double Vgs, double Vgg)
{
  // Vss = correlation(sigma, sigma)
  // Vsg = correlation(sigma, gamma)
  // etc
  double a = 0.5346;
  double b = 0.2166;
  double c = 2 * sqrt( 2*log(2) );
  double f_g = c * sigma;
  double f_l = 2 * gamma;
  double sq = sqrt( b * pow(f_l, 2) + pow(f_g, 2) );
  
  // Partial derivatives of f_voigtian w.r.t sigma and gamma
  // f = a * f_l + sqrt( b * f_l^2 + f_g^2 )
  double dfds = c * ( f_g / sq ) ;
  double dfdg = 2 * ( a + b * f_l / sq ) ;
  
  // esigma * esigma * pow( Vss, 2 ) gives covariance(sigma, sigma) etc
  double p1 = dfds * dfds * esigma * esigma * pow( Vss, 2 );
  double p2 = dfds * dfdg * esigma * egamma * pow( Vsg, 2 );
  double p3 = dfdg * dfds * egamma * esigma * pow( Vgs, 2 );
  double p4 = dfdg * dfdg * egamma * egamma * pow( Vgg, 2 );

  return sqrt ( p1 + p2 + p3 + p4 );
}

