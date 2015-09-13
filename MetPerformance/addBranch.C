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
#include "TVector2.h"
#include "TF1.h"
#include "TCutG.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TMultiGraph.h"
#include "TPaveText.h"
#include "THStack.h"
#include "TStyle.h"
#include "TLatex.h"



void addBranch(TString fileinput) {


TFile finput(fileinput);

TFile foutput("jes_"+fileinput,"recreate");
TH1F * Count = (TH1F*)finput.Get("Count"); 


 
TTree *inputtree = (TTree*)finput.Get("METtree");
TTree *METtree= new TTree("METtree","METtree");

//Read Branches you are interested in from the input tree

Float_t met_JetEnDown_Pt;        inputtree->SetBranchAddress("met_JetEnDown_Pt"       , &met_JetEnDown_Pt);
Float_t met_JetEnUp_Pt;        inputtree->SetBranchAddress("met_JetEnUp_Pt"       , &met_JetEnUp_Pt);
Float_t met_JetEnDown_Phi;        inputtree->SetBranchAddress("met_JetEnDown_Phi"       , &met_JetEnDown_Phi);
Float_t met_JetEnUp_Phi;        inputtree->SetBranchAddress("met_JetEnUp_Phi"       , &met_JetEnUp_Phi);

Float_t zll_pt_normal;        inputtree->SetBranchAddress("zll_pt"       , &zll_pt_normal);
Int_t nVert_normal;     inputtree->SetBranchAddress("nVert"       , &nVert_normal);
Float_t met_sumEt_normal; inputtree->SetBranchAddress("met_sumEt"       , &met_sumEt_normal);
Float_t zll_phi_normal;        inputtree->SetBranchAddress("zll_phi"       , &zll_phi_normal);

Float_t xsec_normal;        inputtree->SetBranchAddress("xsec"       , &xsec_normal);
Float_t puWeight_normal;        inputtree->SetBranchAddress("puWeight"       , &puWeight_normal);


//New branch in bdt tree
Float_t met_uPerp_zll_up_value; TBranch *met_uPerp_zll_up = METtree-> Branch("met_uPerp_zll_up",&met_uPerp_zll_up_value,"met_uPerp_zll_up/F");
Float_t met_uPerp_zll_down_value; TBranch *met_uPerp_zll_down = METtree-> Branch("met_uPerp_zll_down",&met_uPerp_zll_down_value,"met_uPerp_zll_down/F");
Float_t met_uPara_zll_up_value; TBranch *met_uPara_zll_up = METtree-> Branch("met_uPara_zll_up",&met_uPara_zll_up_value,"met_uPara_zll_up/F");
Float_t met_uPara_zll_down_value; TBranch *met_uPara_zll_down = METtree-> Branch("met_uPara_zll_down",&met_uPara_zll_down_value,"met_uPara_zll_down/F");

Float_t zll_pt_value;  TBranch *zll_pt = METtree->Branch ("zll_pt",&zll_pt_value,"zll_pt/F");
Int_t nVert_value;  TBranch *nVert = METtree ->Branch("nVert",&nVert_value,"nVert/I");
Float_t met_sumEt_value; TBranch *met_sumEt= METtree->Branch("met_sumEt",&met_sumEt_value, "met_sumEt/F");
Float_t xsec_value; TBranch *xsec= METtree->Branch("xsec",&xsec_value, "xsec/F");
Float_t puWeight_value; TBranch *puWeight= METtree->Branch("puWeight",&puWeight_value, "puWeight/F");


//read the number of entries in the inputtree
Int_t nentries = (Int_t)inputtree->GetEntries();
for (Int_t i = 0; i < nentries; i++){
//cout << i << endl;
inputtree->GetEntry(i);
          
	  
	Float_t Upara_Up(0), Upara_Down(0),Uperp_Up(0), Uperp_Down(0);

        TVector2 u_Up (0, 0), v_met_Up (0, 0), v_qt (0, 0);
        v_met_Up.SetMagPhi (met_JetEnUp_Pt, met_JetEnUp_Phi);
        v_qt.SetMagPhi (zll_pt_normal, zll_phi_normal);
        u_Up -= (v_met_Up + v_qt);
       
        Upara_Up = (float) ((v_qt * u_Up) / v_qt.Mod ()); 
        float udphi_Up = fabs (u_Up.DeltaPhi (v_qt));  
        u_Up = u_Up.Rotate (TMath::Pi () / 2);
        Uperp_Up = (float) ((v_qt * u_Up) / v_qt.Mod ());
	
	
       TVector2 u_Down (0, 0), v_met_Down (0, 0);
        v_met_Down.SetMagPhi (met_JetEnDown_Pt, met_JetEnDown_Phi);
        u_Down -= (v_met_Down + v_qt);
       
        Upara_Down = (float) ((v_qt * u_Down) / v_qt.Mod ()); 
        float udphi_Down = fabs (u_Down.DeltaPhi (v_qt));  
        u_Down = u_Down.Rotate (TMath::Pi () / 2);
        Uperp_Down = (float) ((v_qt * u_Down) / v_qt.Mod ());
	
	
	


zll_pt_value= zll_pt_normal;
nVert_value=nVert_normal;
met_sumEt_value=met_sumEt_normal;
met_uPerp_zll_up_value=Uperp_Up;
met_uPerp_zll_down_value=Uperp_Down;
met_uPara_zll_up_value=Upara_Up;
met_uPara_zll_down_value=Upara_Down;
xsec_value=xsec_normal;
puWeight_value=puWeight_normal;






METtree->Fill();
}


Count->Write();
METtree->Write("",TObject::kOverwrite);


}
