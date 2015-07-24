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
#include "TSystem.h"



#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooDataHist.h>
#include <RooFitResult.h>
#include <RooAddPdf.h>
#include <RooPlot.h>
#include <RooVoigtian.h>
#include <RooHistPdf.h>
#include <RooFormulaVar.h>


using namespace RooFit;
                           

double FWHM (double, double);
double FWHMError (double, double, double, double, double, double, double,
		  double);
double FWHMError_fixed (double, double, double, double, double, double, double,
		  double);
TString NtoString(Float_t type) ;



RooRealVar x ("x", "x", -400, 400); // changed the axis range, we only need 800 for zll_pt going to 500 GeV.
RooRealVar g_w ("g_w", "width Gaus", 10., 0., 100., "GeV");	//40
RooRealVar gamma_Z0 ("gamma_Z0_U", "Z0 width", 2.3, 0, 100, "GeV");	//20
RooRealVar v_m ("v_m", "v_m",0,-10.,10.);

RooVoigtian *voigt;
RooFitResult *result;
RooAddPdf *model;

double f;
double efwhm;
//TCanvas *c1 = new TCanvas ("c1", "c1", 800, 800);

void constructModel(RooDataHist Hist,RooDataHist *bkg_hist, double m,double um,double uM, bool BKGSubtract) {

      
  f=0;
  efwhm=0;


  v_m.setVal(m);
  v_m.setRange(um,uM);

  voigt =new RooVoigtian ("voigt", "Voigtian", x, v_m, gamma_Z0, g_w);
  
  
  if(BKGSubtract) {
    RooHistPdf *bkg_pdf = new RooHistPdf("bkg_pdf","bkg_pdf",RooArgSet(x),*bkg_hist);
    RooRealVar *lAbkgFrac =new RooRealVar("AbkgFrac","AbkgFrac",0.5,0.,1.);
    RooFormulaVar * sigbkgFrac= new RooFormulaVar("bkgfrac","@0",RooArgSet(*lAbkgFrac));
    model = new RooAddPdf("modelSB","modelSB",*voigt,*bkg_pdf,*sigbkgFrac);
    result = model->fitTo (Hist, RooFit::Minimizer("Minuit2","migrad"),RooFit::Strategy(2), RooFit::SumW2Error (kFALSE), RooFit::Save (kTRUE), RooFit::PrintLevel (-1));	// -1 verbose
                         
  } else {
      result = voigt->fitTo (Hist, RooFit::Minimizer("Minuit2","migrad"),RooFit::Strategy(2), RooFit::SumW2Error (kFALSE), RooFit::Save (kTRUE), RooFit::PrintLevel (-1));	// -1 verbose //
  }

  //if(result->status()!=0) voigt=0;



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
  f = FWHM (sigma, gamma);
  efwhm = FWHMError_fixed (sigma, gamma, esigma, egamma, Vss, Vsg, Vgs, Vgg);

  return;

}


void metperformance (TString samplephys14, TString variablename, TString xvariable, TString tchannel, bool drawchi2, bool WantBKGSubtract)
{
                                                   
  
  TString variablenamepng=variablename;
  variablenamepng.ReplaceAll("/","over");

  TString DestFolder; // Different folder for background subtracted or not subtracted files
  if(WantBKGSubtract){
    DestFolder="BKG_Subtraction";
  }
  else {
    DestFolder="Not_BKG_Subtraction";
  }

  TH1::SetDefaultSumw2() ;

   
cout << "sample phys " << samplephys14 << endl;
  TString folder = "DY";
  if (samplephys14.Contains ("TT"))
    folder = "TTbar";
  if (samplephys14.Contains ("GJet"))
    folder = "Gamma";
  if(samplephys14.Contains ("QCD"))
    folder = "QCD";
  if(samplephys14.Contains("Data"))
   folder ="Data"; 


cout << " folder   " << folder << "  -   " << "Destfolder " << DestFolder << endl;
  
  
  TString titley = "";
  if (variablename == "pfmetx")
    titley = "#sigma(MET_{x}) GeV";
  if (variablename == "pfmety")
    titley = "#sigma(MET_{y}) GeV";




  TCanvas *c1 = new TCanvas ("c1", "c1", 800, 800);

  c1->SetTickx ();
  c1->SetTicky ();
  c1->SetFillColor (kWhite);
  c1->SetFillStyle (0);
  c1->SetRightMargin (0.05);
  c1->SetTopMargin (0.08);




  TFile filephys14 (samplephys14);



  TTree *treephys14 = (TTree *) filephys14.Get ("METtree");




  std::vector < TH1F * >resolution;
  resolution.clear ();




  int limitdown (0), limitup (0);
  TString strlimitup = "0";
  TString strlimitdown = "0";

  int tempsizexarray = 0;

  if (xvariable == "nVert")
    tempsizexarray = 6;
  if (xvariable == "met_sumEt")
    tempsizexarray = 6;
  if (xvariable == "zll_pt")
    tempsizexarray = 10;


  const int sizexarray=tempsizexarray;
  TH1F *histonvertex=new TH1F("histonvertex","histonvertex",50,0,50); // added later
  TH1F *histozll_pt=new TH1F("histozll_pt","histozll_pt",100,0,1200); // added later
  TH1F *histomet_sumEt=new TH1F("histomet_sumEt","histomet_sumEt",100,0,4); // added later
  TH1F *histomet_uPara_zllzll_pt=new TH1F("histomet_uPara_zllzll_pt","histomet_uPara_zllzll_pt",50,-300,300); // added later
  TH1F *histomet_uPerp_zll=new TH1F("histomet_uPerp_zll","histomet_uPerp_zll",50,-300,300); // added later
  TH1F *met_uPara_zllresponse1=new TH1F("met_uPara_zllresponse1","met_uPara_zll",50,-300,300); // added later
  TH1F *zll_ptresponse1=new TH1F("zll_ptresponse1","zll_pt",100,0,100); // added later
  
  TString dileptonch="";
  if (tchannel=="MuMu")  dileptonch="1";
  if (tchannel=="EE" || tchannel=="Gamma") dileptonch="0";
  
  
  // Plot inclusive distributions of the main variables
  TString condition="(xsec)";//"(weighttotal)*(channel=="+dileptonch +")";
  if (samplephys14.Contains("Data")) condition="";
  //cout << condition.Data() << endl;
  treephys14->Draw ("nVert >> histonvertex", condition.Data());
  treephys14->Draw ("zll_pt >> histozll_pt", condition.Data());
  treephys14->Draw ("met_sumEt/1000 >> histomet_sumEt", condition.Data());
  treephys14->Draw ("met_uPara_zll+zll_pt >> histomet_uPara_zllzll_pt", condition.Data());
  treephys14->Draw ("met_uPerp_zll >> histomet_uPerp_zll",condition.Data());//condition.Data());//, condition.Data());
          
          
  histonvertex->Draw();
  histonvertex->GetXaxis ()->SetTitle ("Number of Vertices");
  c1->Print ("~/www/"+DestFolder+"/METResolution/" + folder + "/"+tchannel +"/nVert_inclusive_.png");
  c1->SetLogy();
  histozll_pt->Draw();
  histozll_pt->GetXaxis ()->SetTitle ("zll_pt [GeV]");
  c1->Print ("~/www/"+DestFolder+"/METResolution/"+ folder + "/"+tchannel +"/zll_pt_inclusive_.png");
  histomet_sumEt->Draw();
  histomet_sumEt->GetXaxis ()->SetTitle("sumE_{T} [TeV]");
  c1->Print ("~/www/"+DestFolder+"/METResolution/" + folder + "/"+tchannel +"/met_sumEt_inclusive_.png");
  histomet_uPara_zllzll_pt->Draw();
  histomet_uPara_zllzll_pt->GetXaxis ()->SetTitle ("u_{||}+zll_pt [GeV]");
  c1->Print ("~/www/"+DestFolder+"/METResolution/"+ folder + "/"+tchannel +"/met_uPara_zllzll_pt_inclusive_.png");
  histomet_uPerp_zll->Draw();
  histomet_uPerp_zll->GetXaxis ()->SetTitle ("u_{#perp}   [GeV]");
  c1->Print ("~/www/"+DestFolder+"/METResolution/" + folder + "/"+tchannel +"/met_uPerp_zll_inclusive_.png");
  c1->SetLogy(0);



  Double_t tgraphx[sizexarray], tgraphy[sizexarray], etgraphy[sizexarray],
    etgraphx[sizexarray], tgraphxchi2[sizexarray], tgraphychi2[sizexarray];
    
    



  for (int index = 0; index < sizexarray; index++)
    {
      if (xvariable == "nVert")
	limitup = (index + 1) * 5;
      if (xvariable == "met_sumEt")
	limitup = (index + 1) * 200;
      if (xvariable == "zll_pt")
	limitup = (index + 1) * 12;
      strlimitup = Form ("%d", limitup);
cout << variablename << endl;
cout << limitup << endl;
      if(variablenamepng.Contains("over"))
        resolution.push_back (new TH1F (Form ("resx%d", index), " ", 200, -200, 200)); // changing the x axis for met_uPara_zlloverzll_pt, so that models are more visible
      else
        resolution.push_back (new TH1F (Form ("resx%d", index), " ", 200, -400, 400));
      


     TString totalnevents="1";
     TString lumim="0.0073";
     TH1F * h0 = (TH1F*)filephys14.Get("Count");
     totalnevents=NtoString(h0->Integral());
         
      TString conditionbkg="";
      
      condition="(xsec)*(lumi)*";//"(weighttotal)*(channel=="+dileptonch +")*";

      if (xvariable == "nVert") condition = "(" + xvariable + "==" + strlimitup +")";
      else condition = "(" + xvariable + "<" + strlimitup + ")*(" + xvariable + ">" + strlimitdown + ")";
      
      conditionbkg=condition;
      if(!samplephys14.Contains("Data")) condition=condition+"*((xsec)*(lumi))/"+totalnevents;   
      
      cout << "condition data"  << condition.Data() << endl;
      treephys14->Draw (variablename + ">>" + TString (resolution[index]->GetName ()),			condition.Data(), "sames");

      //c1->Print("~/www/"+TString (resolution[index]->GetName ())+".png");     
      double m =  resolution[index]->GetMean ();
      double um = resolution[index]->GetMean () - resolution[index]->GetRMS ();
      double uM = resolution[index]->GetMean () + resolution[index]->GetRMS ();



      ////////
      
      RooDataHist Hist ("Hist", "Hist", x,			(TH1 *) resolution[index]->Clone ());

      RooDataHist *bkg_histogram=0;

      if(WantBKGSubtract) {
	TFile *file_ ;
	
	if (tchannel=="Gamma") file_=TFile::Open("QCD_BKG_Train.root");
	else file_=TFile::Open("TTJets13TeV.root");
	
	TString totalnbkg="1";
	TH1F * h1 = (TH1F*)file_->Get("Count");
		totalnbkg=NtoString(h1->Integral());
conditionbkg=conditionbkg+"*((xsec)*(lumi))/("+totalnbkg+")"; 
	TTree *treephys14bkg = (TTree *) file_->Get ("METtree");
	int bkgbin(0);
	if(variablenamepng.Contains("over"))bkgbin = 5;
	else bkgbin = 400;
	TH1F *h_ = new TH1F("h_"," ", 200, -bkgbin, bkgbin);
	cout << "condition bkg " << conditionbkg << endl;
	treephys14bkg->Draw (variablename + ">>" + TString (h_->GetName ()),	conditionbkg.Data(),"sames");
	bkg_histogram= new RooDataHist("bkg_histogram","bkg_histogram",x,h_);

      }

      // construct the voightian model
      // fit the Hist Dataset also
      // fill f and efwhm that are the parameter of the voightian
      constructModel(Hist, bkg_histogram, m, um, uM, WantBKGSubtract);
                       
      //if (f/2.3 < 5) continue;
      
      RooPlot *xFrame=x.frame();
      Hist.plotOn (xFrame);

      TString titlexfit = "";
      if (variablename == "pfmetx")
	titlexfit = "MET_{x} [GeV]";
      if (variablename == "pfmety")
	titlexfit = "MET_{y} [GeV]";
      xFrame->SetXTitle (titlexfit);

      int color=kBlack;
      if (xvariable == "nVert")
	color = kRed;
      if (xvariable == "met_sumEt")
	color = kGreen+3;
      if (xvariable == "zll_pt")
	color = kBlue;
      

      cout << "plot made " << endl;
      c1->cd();
      
      //Hist.plotOn(xFrame2);
      //model->plotOn(xFrame2,RooFit::LineColor(kBlack));
      if ( WantBKGSubtract  ){
        model->plotOn(xFrame);
        model->plotOn(xFrame, Components("bkg_pdf"), LineColor(kRed), LineStyle(kDashed), FillColor(kRed), DrawOption("F"));
        model->plotOn(xFrame, Components("voigt"), LineColor(kGreen), LineStyle(kDashed), FillColor(kGreen+1), DrawOption("L"));
      }  
      else {
	Hist.plotOn(xFrame);
        voigt->plotOn(xFrame, RooFit::FillColor(kGray), VisualizeError(*result,1), RooFit::Components(*voigt)); // 1 sigma band in gray
        voigt->plotOn(xFrame, RooFit::LineColor(color));
      }                             
      TString histoname = resolution[index]->GetName ();
      xFrame->GetYaxis()->SetRangeUser(1,20000);
      xFrame->GetXaxis() ->SetRangeUser(-50,50);
      xFrame->Draw();
      c1->Print ("~/www/"+DestFolder+"/METModel/" + folder + "/" + tchannel +"/" + histoname + "_" +	variablenamepng + "_vs_" + xvariable + ".png");


      //c1->Print ("~/www/"+DestFolder+"/METFits/" + folder + "/" + tchannel +"/" + histoname + "_" +	variablenamepng + "_vs_" + xvariable + ".png");

      //Print chi2/dof value

      Double_t chi2 = xFrame->chiSquare ();	//"voigt", "Hist", 3);
      //cout << "chi2 = " << chi2 << endl;

      tgraphx[index] = index;

      if (xvariable == "nVert")
	tgraphx[index] = limitup;
      if (xvariable == "met_sumEt")
	tgraphx[index] = limitup * 0.001;	//For the x axis to be in TEV
      if (xvariable == "zll_pt")
	tgraphx[index] = limitup;

      tgraphxchi2[index] = tgraphx[index];
      tgraphychi2[index] = chi2;

      if (chi2 != chi2 || chi2 >= 100)
    	tgraphychi2[index] = -0.2;
      tgraphy[index] = f / 2.3548;
      if (variablename == "met_uPara_zll/zll_pt"){
	    tgraphy[index] = -resolution[index]->GetMean ();
	    

// 	    if(tgraphy[index] > 1.0){
// 	       TString condition2="(weighttotal)*(channel=="+dileptonch +")";
//                treephys14->Draw ("met_uPara_zll >> met_uPara_zllresponse1", condition.Data());
//                treephys14->Draw ("zll_pt >> zll_ptresponse1", condition.Data(), "sames");
// 	       TString NumberStr;          // string which will contain the result
//                ostringstream convert;   // stream used for the conversion
//                convert << index;      // insert the textual representation of 'Number' in the characters in the stream
//                NumberStr = convert.str(); 
// 	       met_uPara_zllresponse1->Draw("hist");
//                met_uPara_zllresponse1->GetXaxis ()->SetTitle ("u_{|| }   [GeV]");
//                c1->Print ("~/www/"+DestFolder+"/METResolution/" + folder + "/"+tchannel +"/met_uPara_zll_response1_"+NumberStr+".png");
//                c1->SetLogy(0);
// 	       zll_ptresponse1->Draw("hist");
//                zll_ptresponse1->GetXaxis ()->SetTitle ("zll_pt   [GeV]");
//                c1->Print ("~/www/"+DestFolder+"/METResolution/" + folder + "/"+tchannel +"/zll_pt_response1_"+NumberStr+".png");
//                c1->SetLogy(0);
// 	    }
	    //cout << index << "  and mean: " << -resolution[index]->GetMean () << endl;
      }
      etgraphy[index] = efwhm / 2.3548;
      if (variablename == "met_uPara_zll/zll_pt" || variablename == "met_uPara_zllraw/zll_pt")
	    {
	    etgraphy[index] = resolution[index]->GetMeanError ();
      etgraphx[index] = 0;}


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
  
  
  if (xvariable == "met_sumEt")
    {
      gr->GetXaxis ()->SetTitle ("sumE_{T} [TeV]");
      grchi2->GetXaxis ()->SetTitle ("sumE_{T} [TeV]");
    }
  if (xvariable == "nVert")
    {
      gr->GetXaxis ()->SetTitle ("Number of Vertices");
      grchi2->GetXaxis ()->SetTitle ("Number of Vertices");
    }
  if (xvariable == "zll_pt")
    {
      gr->GetXaxis ()->SetTitle ("zll_pt [GeV]");
      grchi2->GetXaxis ()->SetTitle ("zll_pt [GeV]");
    }



  gr->GetYaxis ()->SetTitle (titley);
  if (variablename == "met_uPara_zll")
    gr->GetYaxis ()->SetTitle ("#sigma(u_{||}) [GeV]");
  if (variablename == "met_uPara_zllraw")
    gr->GetYaxis ()->SetTitle ("#sigma(u_{|| raw}) [GeV]");
  if (variablename == "met_uPara_zll+zll_pt")
    gr->GetYaxis ()->SetTitle ("#sigma(u_{||} +zll_pt) [GeV]");
  if (variablename == "met_uPara_zll/zll_pt")
    gr->GetYaxis ()->SetTitle ("-<u_{||}> /zll_pt ");
  if (variablename == "met_uPara_zllraw/zll_pt")
    gr->GetYaxis ()->SetTitle ("-<u_{|| raw}> /zll_pt ");
  if (variablename == "met_uPara_zllraw+zll_pt")
    gr->GetYaxis ()->SetTitle ("#sigma(u_{|| raw} +zll_pt) [GeV]");
  if (variablename == "met_uPerp_zll")
    gr->GetYaxis ()->SetTitle ("#sigma(u_{#perp}  ) [GeV]");
  if (variablename == "met_uPerp_zllraw")
    gr->GetYaxis ()->SetTitle ("#sigma(u_{#perp raw}  ) [GeV]");
  

  if (drawchi2) {
  grchi2->GetYaxis ()->SetTitle ("#Chi^{2}");
  grchi2->Draw ("AP");
  c1->Print ("~/www/"+DestFolder+"/METResolution/" + folder + "/" + tchannel + "/" +  variablenamepng  + "_vs_" +	     xvariable + "_chi2.png");
  c1->Clear (); }

  TFile f2 (DestFolder+folder+ "_tgraphs.root", "UPDATE");
  gr->Write (tchannel+"_"+variablenamepng + "_vs_" + xvariable);


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
  if (variablename!="met_uPara_zll/zll_pt" && variablename!="met_uPara_zllraw/zll_pt") gr->GetYaxis()->SetRangeUser(0,70);
  else gr->GetYaxis()->SetRangeUser(0.8,1.2);
  c1->Update();
  
  
  if (variablename == "met_uPara_zll/zll_pt" || variablename=="met_uPara_zllraw/zll_pt")
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

  

  c1->Print ("~/www/"+DestFolder+"/METResolution/" + folder + "/" + tchannel + "/" + variablenamepng  + "_vs_" +	     xvariable + ".png");



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

TString NtoString(Float_t type) 
{ 
   TString name; name.Form("%f",type); 
      cout << name << endl;   
         return name;
         
            } 
            
            
