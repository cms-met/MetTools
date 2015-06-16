
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


#include <TStyle.h>
#include<TSystem.h>

void comparetgraphs(TString filewithgraphs1, TString filewithgraphs2, TString tgraph1){


TCanvas *c1 = new TCanvas ("c1", "c1", 800, 800);


bool resp=false;
if (tgraph1.Contains("upararawqt" || "uparaqt")) resp=true; 
 
TFile * inputfile1 = new TFile(filewithgraphs1,"READ");
TGraphErrors* g1 = (TGraphErrors*) inputfile1 -> Get(tgraph1);
 g1->SetMarkerStyle(21);
    g1->SetMarkerColor(2);

TFile * inputfile2= new TFile(filewithgraphs2,"READ");
TGraphErrors* g2 =  (TGraphErrors*) inputfile2 -> Get(tgraph1);
 g2->SetMarkerStyle(22);
    g2->SetMarkerColor(3);
          
                   TMultiGraph *g = new TMultiGraph();
                      g->Add(g1);
                         g->Add(g2);
                         //c1->BuildLegend();
                             g->Draw("AP");
 
                        //  g->SetMaximum(1.5);
                       //   g->SetMinimum(0);
                          if (tgraph1.Contains("sumEt")){   g->GetXaxis()->SetLimits(0,1.5);   g->GetXaxis()->SetTitle("#sumE_{T} [TeV]");}
                          if (tgraph1.Contains("nvtx")){   g->GetXaxis()->SetLimits(5,30);   g->GetXaxis()->SetTitle("Number of Vertices");}
                          if (tgraph1.Contains("qt")){ g->GetXaxis()->SetTitle("qt [GeV]");}
                          if (resp) g->GetYaxis()->SetTitle("-<u_{||}>/qt ");
                          if (tgraph1.Contains("upararaw")) g->GetYaxis()->SetTitle("#sigma(u_{||})");
                          if (tgraph1.Contains("uperp")) g->GetYaxis()->SetTitle("#sigma(u_{#perp}  ) [GeV]");
                          
                          
                          if (tgraph1.Contains("uparaqt")){
                          TLine *lineR =  new TLine ( g->GetHistogram ()->GetXaxis ()->GetXmin (), 1, g->GetHistogram ()->GetXaxis ()->GetXmax (), 1);
                                lineR->SetLineColor (kBlue + 1);
                                      lineR->SetLineWidth (2);
                                            lineR->SetLineStyle (2);
                                                  lineR->Draw ();
                                                  
                          }
                          else {
                          
                          TLine *lineR =  new TLine ( g->GetHistogram ()->GetXaxis ()->GetXmin (), 0, g->GetHistogram ()->GetXaxis ()->GetXmax (), 0);
                                lineR->SetLineColor (kBlue + 1);
                                      lineR->SetLineWidth (2);
                                            lineR->SetLineStyle (2);
                                                  lineR->Draw ();
                                                  
                          }
                          
                          
                          if (!tgraph1.Contains("uparaqt")) g->GetYaxis()->SetRangeUser(-100,100);
                           c1->Update();
                          
                           TLegend* leg(0);
                           leg = new TLegend(0.50,0.13,0.79,0.25);
                             
                          leg->SetFillColor(0);
                          leg->AddEntry(g1, "PseudoData", "lp");
                          leg->AddEntry(g2, "DY", "lp");
                         leg->Draw();
                          c1->Modified();
                          c1->Print(tgraph1+".png");
                          

}
