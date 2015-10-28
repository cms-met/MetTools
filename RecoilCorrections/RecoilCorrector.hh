#include <vector>
#include <sstream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TF1.h"
#include "TMath.h"
#include "TRandom1.h"
#include "TGraphErrors.h"
#include <TFitResult.h>

//
// ** apply phil's recoil corrections **
// 
// usage: 
//    double met=rawMetValue;
//    double metphi=rawMetPhiValue;
//    RecoilCorrector corrector;
//    corrector->CorrectType*(met,metphi,GenZPt,GenZPhi,leptonPt,leptonPhi,u1,u2,sigma_mean,sigma_rms);
//    printf("corrected met: %10.2f%10.2f\n",met,metphi);
//
// where leptonPt, leptonPhi are dilepton kinematics for z->ll and single lepton kinematics for w->lnu
//

using namespace std;

class RecoilCorrector
{
  
public:
  RecoilCorrector(string iNameZDat, int iSeed=0xDEADBEEF);
  RecoilCorrector(string iNameZDat1, string iPrefix, int iSeed=0xDEADBEEF);
  void CorrectAll(double &met, double &metphi, double iGenPt, double iGenPhi, double iLepPt, double iLepPhi,double &iU1,double &iU2,double iFluc,double iScale=0,int njet=0);
  void CorrectType0(double &pfmet, double &pfmetphi,double iGenPt,double iGenPhi,double iLepPt,double iLepPhi,double &iU1,double &iU2,double iFluc,double iScale=0,int njet=0);
  void CorrectType1(double &pfmet, double &pfmetphi,double iGenPt,double iGenPhi,double iLepPt,double iLepPhi,double &iU1,double &iU2,double iFluc,double iScale=0,int njet=0);
  void CorrectType2(double &pfmet, double &pfmetphi,double iGenPt,double iGenPhi,double iLepPt,double iLepPhi,double &iU1,double &iU2,double iFluc,double iScale=0,int njet=0);
  void CorrectType2FromGraph(double &pfmet, double &pfmetphi,double iGenPt,double iGenPhi,double iLepPt,double iLepPhi,double &iU1,double &iU2,double iFluc,double iScale=0,int njet=0);
  void CorrectU1U2(double &pfu1, double &pfu2, double &trku1, double &trku2, 
		   double iGenPt, double iGenPhi, double iLepPt, double iLepPhi,double iFluc,double iScale=0,int njet=0);
  void addDataFile(std::string iNameDat);
  void addMCFile  (std::string iNameMC);
  void addFileWithGraph  (std::string iNameMC);
  Double_t dMean(const TF1 *fcn, const Double_t x, const TFitResult  *fs);
  Double_t dSigma(const TF1 *fcn, const Double_t x, const TFitResult *fs);
  TFitResult *fitresPFu1mean, *fitresPFu1sigma1, *fitresPFu1sigma2,  *fitresPFu1sigma0;
  TFitResult *fitresPFu2mean, *fitresPFu2sigma1, *fitresPFu2sigma2,  *fitresPFu2sigma0;
  
protected:
  enum Recoil { 
    PFU1,
    PFU2,
    PFMSU1,
    PFMSU2,
    PFS1U1,
    PFS2U1,
    PFS1U2,
    PFS2U2,
    TKU1,
    TKU2,
    TKMSU1,
    TKMSU2,
    TKS1U1,
    TKS2U1,
    TKS1U2,
    TKS2U2
  };
  
  void readRecoil(std::vector<TF1*> &iU1Fit,std::vector<TF1*> &iU1MRMSFit,std::vector<TF1*> &iU1RMS1Fit,std::vector<TF1*> &iU1RMS2Fit,
		  std::vector<TF1*> &iU2Fit,std::vector<TF1*> &iU2MRMSFit,std::vector<TF1*> &iU2RMS1Fit,std::vector<TF1*> &iU2RMS2Fit,
		  std::string iFName,std::string iPrefix); 
  
  void readRecoilMC(std::vector<TGraphErrors*> &iU1Fit,std::vector<TGraphErrors*> &iU1MRMSFit,std::vector<TGraphErrors*> &iU1RMS1Fit,std::vector<TGraphErrors*> &iU1RMS2Fit,
		  std::vector<TGraphErrors*> &iU2Fit,std::vector<TGraphErrors*> &iU2MRMSFit,std::vector<TGraphErrors*> &iU2RMS1Fit,std::vector<TGraphErrors*> &iU2RMS2Fit,
		  std::string iFName,std::string iPrefix); 

          
  void metDistributionType0(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
                double iLepPt,double iLepPhi,
                TGraphErrors *iU1Default,
                TF1 *iU1RZDatFit, 
                TF1 *iU1MSZDatFit,  
                TF1 *iU1S1ZDatFit,  
                TF1 *iU1S2ZDatFit, 
                TF1 *iU2RZDatFit,
                TF1 *iU2MSZDatFit, 
                TF1 *iU2S1ZDatFit,       
                TF1 *iU2S2ZDatFit,      
                double &iU1, double &iU2,double iFluc=0,double iScale=0);
          
  void metDistributionType2(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
			    double iLepPt,double iLepPhi,
			    TGraphErrors *iU1Default,
			    TF1 *iU1RZDatFit,  TGraphErrors *iU1RZMCFit,
			    TF1 *iU1MSZDatFit, TGraphErrors *iU1MSZMCFit, 
			    TF1 *iU1S1ZDatFit, TGraphErrors *iU1S1ZMCFit, 
			    TF1 *iU1S2ZDatFit, TGraphErrors *iU1S2ZMCFit, 
			    TF1 *iU2MSZDatFit, TGraphErrors *iU2MSZMCFit,
			    TF1 *iU2S1ZDatFit, TGraphErrors *iU2S1ZMCFit,	   
			    TF1 *iU2S2ZDatFit, TGraphErrors *iU2S2ZMCFit,	   
			    double &iU1, double &iU2,double iFluc=0,double iScale=0);

  void metDistributionType2FromGraph(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
			    double iLepPt,double iLepPhi,
			    TGraphErrors *iU1Default,
			    TGraphErrors *iU1RZDatFit,  TGraphErrors *iU1RZMCFit,
			    TGraphErrors *iU1MSZDatFit, TGraphErrors *iU1MSZMCFit, 
			    TGraphErrors *iU1S1ZDatFit, TGraphErrors *iU1S1ZMCFit, 
			    TGraphErrors *iU1S2ZDatFit, TGraphErrors *iU1S2ZMCFit, 
			    TGraphErrors *iU2MSZDatFit, TGraphErrors *iU2MSZMCFit,
			    TGraphErrors *iU2S1ZDatFit, TGraphErrors *iU2S1ZMCFit,	   
			    TGraphErrors *iU2S2ZDatFit, TGraphErrors *iU2S2ZMCFit,	   
			    double &iU1, double &iU2,double iFluc=0,double iScale=0);
                
  Double_t calcErrorGraph(const TGraphErrors *graph, const Double_t x, const Double_t bins[], const Int_t bin);

  double diGausPVal    (double iVal, double iFrac,double iSimga1,double iSigma2);
  double diGausPInverse(double iPVal,double iFrac,double iSigma1,double iSigma2);
  double calculate(int iMet,double iEPt,double iEPhi,double iWPhi,double iU1,double iU2);
  double getCorError2(double iVal,TF1 *iFit);
  double mag(double iV0,double iV1,double iV2,double iV3);
  double correlatedSeed(double iVal, double iCorr1,double iCorr2,double iCorr3,double iSeed0,double iSeed1,double iSeed2,double iSeed3);
  double deCorrelate   (double iVal, double iCorr1,double iCorr2,double iCorr3,double iSeed0,double iSeed1,double iSeed2,double iSeed3);
  TF1*   getFunc(bool iMC, Recoil iType);
  double CorrVal(double iPt,double iVal,Recoil iType);

  TRandom1 *fRandom; 
  vector<TGraphErrors*> fF1U1Fit; vector<TGraphErrors*> fF1U1RMSSMFit; vector<TGraphErrors*> fF1U1RMS1Fit; vector<TGraphErrors*> fF1U1RMS2Fit; 
  vector<TGraphErrors*> fF1U2Fit; vector<TGraphErrors*> fF1U2RMSSMFit; vector<TGraphErrors*> fF1U2RMS1Fit; vector<TGraphErrors*> fF1U2RMS2Fit; 
  vector<TGraphErrors*> fF2U1Fit; vector<TGraphErrors*> fF2U1RMSSMFit; vector<TGraphErrors*> fF2U1RMS1Fit; vector<TGraphErrors*> fF2U1RMS2Fit; 
  vector<TGraphErrors*> fF2U2Fit; vector<TGraphErrors*> fF2U2RMSSMFit; vector<TGraphErrors*> fF2U2RMS1Fit; vector<TGraphErrors*> fF2U2RMS2Fit; 

  vector<TF1*> fD1U1Fit; vector<TF1*> fD1U1RMSSMFit; vector<TF1*> fD1U1RMS1Fit; vector<TF1*> fD1U1RMS2Fit; 
  vector<TF1*> fD1U2Fit; vector<TF1*> fD1U2RMSSMFit; vector<TF1*> fD1U2RMS1Fit; vector<TF1*> fD1U2RMS2Fit; 
  vector<TF1*> fD2U1Fit; vector<TF1*> fD2U1RMSSMFit; vector<TF1*> fD2U1RMS1Fit; vector<TF1*> fD2U1RMS2Fit; 
  vector<TF1*> fD2U2Fit; vector<TF1*> fD2U2RMSSMFit; vector<TF1*> fD2U2RMS1Fit; vector<TF1*> fD2U2RMS2Fit; 

  vector<TGraphErrors*> fM1U1Fit; vector<TGraphErrors*> fM1U1RMSSMFit; vector<TGraphErrors*> fM1U1RMS1Fit; vector<TGraphErrors*> fM1U1RMS2Fit; 
  vector<TGraphErrors*> fM1U2Fit; vector<TGraphErrors*> fM1U2RMSSMFit; vector<TGraphErrors*> fM1U2RMS1Fit; vector<TGraphErrors*> fM1U2RMS2Fit; 
  vector<TGraphErrors*> fM2U1Fit; vector<TGraphErrors*> fM2U1RMSSMFit; vector<TGraphErrors*> fM2U1RMS1Fit; vector<TGraphErrors*> fM2U1RMS2Fit; 
  vector<TGraphErrors*> fM2U2Fit; vector<TGraphErrors*> fM2U2RMSSMFit; vector<TGraphErrors*> fM2U2RMS1Fit; vector<TGraphErrors*> fM2U2RMS2Fit; 

  vector<TGraphErrors*> fMT1U1Fit; vector<TGraphErrors*> fMT1U1RMSSMFit; vector<TGraphErrors*> fMT1U1RMS1Fit; vector<TGraphErrors*> fMT1U1RMS2Fit; 
  vector<TGraphErrors*> fMT1U2Fit; vector<TGraphErrors*> fMT1U2RMSSMFit; vector<TGraphErrors*> fMT1U2RMS1Fit; vector<TGraphErrors*> fMT1U2RMS2Fit; 
  vector<TGraphErrors*> fMT2U1Fit; vector<TGraphErrors*> fMT2U1RMSSMFit; vector<TGraphErrors*> fMT2U1RMS1Fit; vector<TGraphErrors*> fMT2U1RMS2Fit; 
  vector<TGraphErrors*> fMT2U2Fit; vector<TGraphErrors*> fMT2U2RMSSMFit; vector<TGraphErrors*> fMT2U2RMS1Fit; vector<TGraphErrors*> fMT2U2RMS2Fit; 

  vector<TGraphErrors*> fF1U1U2Corr;     vector<TGraphErrors*> fF2U1U2Corr;
  vector<TGraphErrors*> fF1F2U1Corr;     vector<TGraphErrors*> fF1F2U2Corr;
  vector<TGraphErrors*> fF1F2U1U2Corr;   vector<TGraphErrors*> fF1F2U2U1Corr;

  vector<TGraphErrors*> fM1U1U2Corr;     vector<TGraphErrors*> fM2U1U2Corr;
  vector<TGraphErrors*> fM1M2U1Corr;     vector<TGraphErrors*> fM1M2U2Corr;
  vector<TGraphErrors*> fM1M2U1U2Corr;   vector<TGraphErrors*> fM1M2U2U1Corr;
  int fId; int fJet;
};

//-----------------------------------------------------------------------------------------------------------------------------------------
RecoilCorrector::RecoilCorrector(string iNameZDat,std::string iPrefix, int iSeed) {

  fRandom = new TRandom1(iSeed);
  readRecoilMC(fF1U1Fit,fF1U1RMSSMFit,fF1U1RMS1Fit,fF1U1RMS2Fit,fF1U2Fit,fF1U2RMSSMFit,fF1U2RMS1Fit,fF1U2RMS2Fit,iNameZDat,iPrefix);
  fId = 0; fJet = 0;
}

RecoilCorrector::RecoilCorrector(string iNameZ, int iSeed) {

  fRandom = new TRandom1(iSeed);
  // get fits for Z data
  readRecoilMC(fF1U1Fit,fF1U1RMSSMFit,fF1U1RMS1Fit,fF1U1RMS2Fit,fF1U2Fit,fF1U2RMSSMFit,fF1U2RMS1Fit,fF1U2RMS2Fit,iNameZ,"PF");
  fId = 0; fJet = 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::addDataFile(std::string iNameData) { 
  readRecoil(fD1U1Fit,fD1U1RMSSMFit,fD1U1RMS1Fit,fD1U1RMS2Fit,fD1U2Fit,fD1U2RMSSMFit,fD1U2RMS1Fit,fD1U2RMS2Fit,iNameData,"fcnPF");
  fId++;   
}
void RecoilCorrector::addMCFile  (std::string iNameMC) { 
  fId++;
  readRecoilMC(fM1U1Fit,fM1U1RMSSMFit,fM1U1RMS1Fit,fM1U1RMS2Fit,fM1U2Fit,fM1U2RMSSMFit,fM1U2RMS1Fit,fM1U2RMS2Fit,iNameMC,"fcnPF");
}

void RecoilCorrector::addFileWithGraph  (std::string iNameMC) { 
  fId++;
  readRecoilMC(fMT1U1Fit,fMT1U1RMSSMFit,fMT1U1RMS1Fit,fMT1U1RMS2Fit,fMT1U2Fit,fMT1U2RMSSMFit,fMT1U2RMS1Fit,fMT1U2RMS2Fit,iNameMC,"fcnPF");
}


void RecoilCorrector::CorrectType0(double &met, double &metphi, double lGenPt, double lGenPhi, double lepPt, double lepPhi,double &iU1,double &iU2,double iFluc,double iScale,int njet) {  
  fJet = njet; if(njet > 2) fJet = 2;
  if(fJet >= int(fF1U1Fit.size())) fJet = 0;
  metDistributionType0(met,metphi,lGenPt,lGenPhi,lepPt,lepPhi,fF1U1Fit[fJet],
               fD1U1Fit     [fJet],
               fD1U1RMSSMFit[fJet],
               fD1U1RMS1Fit [fJet],
               fD1U1RMS2Fit [fJet],
               fD1U2Fit     [fJet],
               fD1U2RMSSMFit[fJet],
               fD1U2RMS1Fit [fJet],
               fD1U2RMS2Fit [fJet], 
               iU1,iU2,iFluc,iScale);
}

void RecoilCorrector::CorrectType2(double &met, double &metphi, double lGenPt, double lGenPhi, double lepPt, double lepPhi,double &iU1,double &iU2,double iFluc,double iScale,int njet) {  
  fJet = njet; if(njet > 2) fJet = 2;
  if(fJet >= int(fF1U1Fit.size())) fJet = 0;
  metDistributionType2(met,metphi,lGenPt,lGenPhi,lepPt,lepPhi,fF1U1Fit[fJet],
		       fD1U1Fit     [fJet],fM1U1Fit     [fJet],
		       fD1U1RMSSMFit[fJet],fM1U1RMSSMFit[fJet],
		       fD1U1RMS1Fit [fJet],fM1U1RMS1Fit [fJet],
		       fD1U1RMS2Fit [fJet],fM1U1RMS2Fit [fJet],
		       fD1U2RMSSMFit[fJet],fM1U2RMSSMFit[fJet],
		       fD1U2RMS1Fit [fJet],fM1U2RMS1Fit [fJet],
		       fD1U2RMS2Fit [fJet],fM1U2RMS2Fit [fJet], 
		       iU1,iU2,iFluc,iScale);
}

void RecoilCorrector::CorrectType2FromGraph(double &met, double &metphi, double lGenPt, double lGenPhi, double lepPt, double lepPhi,double &iU1,double &iU2,double iFluc,double iScale,int njet) {  
  fJet = njet; if(njet > 2) fJet = 2;
  if(fJet >= int(fF1U1Fit.size())) fJet = 0;
  metDistributionType2FromGraph(met,metphi,lGenPt,lGenPhi,lepPt,lepPhi,fF1U1Fit[fJet],
		       fMT1U1Fit     [fJet],fM1U1Fit     [fJet],
		       fMT1U1RMSSMFit[fJet],fM1U1RMSSMFit[fJet],
		       fMT1U1RMS1Fit [fJet],fM1U1RMS1Fit [fJet],
		       fMT1U1RMS2Fit [fJet],fM1U1RMS2Fit [fJet],
		       fMT1U2RMSSMFit[fJet],fM1U2RMSSMFit[fJet],
		       fMT1U2RMS1Fit [fJet],fM1U2RMS1Fit [fJet],
		       fMT1U2RMS2Fit [fJet],fM1U2RMS2Fit [fJet], 
		       iU1,iU2,iFluc,iScale);
}

TF1* RecoilCorrector::getFunc(bool iMC, Recoil iType) { 
  if(fId == 0 || fId == 1) return 0;
  switch(iType) {
  case PFU1   : return fD1U1Fit     [fJet];
  case PFMSU1 : return fD1U1RMSSMFit[fJet]; 
  case PFS1U1 : return fD1U1RMS1Fit [fJet]; 
  case PFS2U1 : return fD1U1RMS2Fit [fJet]; 
  case PFU2   : return 0;
  case PFMSU2 : return fD1U2RMSSMFit[fJet]; 
  case PFS1U2 : return fD1U2RMS1Fit [fJet]; 
  case PFS2U2 : return fD1U2RMS2Fit [fJet]; 
  case TKU1   : return fD2U1Fit     [fJet];
  case TKMSU1 : return fD2U1RMSSMFit[fJet];
  case TKS1U1 : return fD2U1RMS1Fit [fJet];
  case TKS2U1 : return fD2U1RMS2Fit [fJet];
  case TKU2   : return 0;
  case TKMSU2 : return fD2U2RMSSMFit[fJet];
  case TKS1U2 : return fD2U2RMS1Fit [fJet];
  case TKS2U2 : return fD2U2RMS2Fit [fJet];
  }
  return 0;
}


//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::readRecoil(std::vector<TF1*> &iU1Fit,std::vector<TF1*> &iU1MRMSFit,std::vector<TF1*> &iU1RMS1Fit,std::vector<TF1*> &iU1RMS2Fit,
		                 std::vector<TF1*> &iU2Fit,std::vector<TF1*> &iU2MRMSFit,std::vector<TF1*> &iU2RMS1Fit,std::vector<TF1*> &iU2RMS2Fit,
		                 std::string iFName,std::string iPrefix) {
  TFile *lFile  = new TFile(iFName.c_str());
  int lNJet = 0;
  std::stringstream lSS; //lSS << iPrefix;
  lSS << iPrefix << "u1mean"; 
  iU1Fit.push_back    ( (TF1*) lFile->FindObjectAny(lSS.str().c_str())); lSS.str("");
  lSS << iPrefix << "u1sigma0";
  iU1MRMSFit.push_back( (TF1*) lFile->FindObjectAny(lSS.str().c_str())); lSS.str(""); 
  lSS << iPrefix << "u1sigma1"; 
  iU1RMS1Fit.push_back( (TF1*) lFile->FindObjectAny(lSS.str().c_str())); lSS.str(""); 
  lSS << iPrefix << "u1sigma2"; iU1RMS2Fit.push_back( (TF1*) lFile->FindObjectAny(lSS.str().c_str())); lSS.str(""); 
  lSS << iPrefix << "u2mean"; iU2Fit.push_back( (TF1*) lFile->FindObjectAny(lSS.str().c_str())); lSS.str("");
  lSS << iPrefix << "u2sigma0"; 
  iU2MRMSFit.push_back( (TF1*) lFile->FindObjectAny(lSS.str().c_str())); lSS.str("");
  lSS << iPrefix << "u2sigma1"; iU2RMS1Fit.push_back( (TF1*) lFile->FindObjectAny(lSS.str().c_str())); lSS.str("");
  lSS << iPrefix << "u2sigma2"; iU2RMS2Fit.push_back( (TF1*) lFile->FindObjectAny(lSS.str().c_str())); lSS.str("");
  fitresPFu1mean  = (TFitResult*)(lFile->Get("fitresPFu1mean")->Clone("fitresPFu1mean"));    
  fitresPFu2mean  = (TFitResult*)(lFile->Get("fitresPFu2mean")->Clone("fitresPFu2mean"));
  fitresPFu1sigma0  = (TFitResult*)(lFile->Get("fitresPFu1sigma0")->Clone("fitresPFu1sigma0"));
  fitresPFu2sigma0  = (TFitResult*)(lFile->Get("fitresPFu2sigma0")->Clone("fitresPFu2sigma0"));
  fitresPFu1sigma1  = (TFitResult*)(lFile->Get("fitresPFu1sigma1")->Clone("fitresPFu1sigma1"));
  fitresPFu2sigma1  = (TFitResult*)(lFile->Get("fitresPFu2sigma1")->Clone("fitresPFu2sigma1"));
  fitresPFu1sigma2  = (TFitResult*)(lFile->Get("fitresPFu1sigma2")->Clone("fitresPFu1sigma2"));
  fitresPFu2sigma2  = (TFitResult*)(lFile->Get("fitresPFu2sigma2")->Clone("fitresPFu2sigma2"));

  lFile->Close();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::readRecoilMC(std::vector<TGraphErrors*> &iU1Fit,std::vector<TGraphErrors*> &iU1MRMSFit,std::vector<TGraphErrors*> &iU1RMS1Fit,std::vector<TGraphErrors*> &iU1RMS2Fit,
		                 std::vector<TGraphErrors*> &iU2Fit,std::vector<TGraphErrors*> &iU2MRMSFit,std::vector<TGraphErrors*> &iU2RMS1Fit,std::vector<TGraphErrors*> &iU2RMS2Fit,
		                 std::string iFName,std::string iPrefix) {
  TFile *lFile  = new TFile(iFName.c_str());
  int lNJet = 0;
  std::stringstream lSS; //lSS << iPrefix;
  iPrefix = "grPF"; 
  lSS << iPrefix << "u1mean"; 
  iU1Fit.push_back    ( (TGraphErrors*) lFile->FindObjectAny(lSS.str().c_str())); lSS.str("");
  lSS << iPrefix << "u1sigma0";
  iU1MRMSFit.push_back( (TGraphErrors*) lFile->FindObjectAny(lSS.str().c_str())); lSS.str(""); 
  lSS << iPrefix << "u1sigma1"; 
  iU1RMS1Fit.push_back( (TGraphErrors*) lFile->FindObjectAny(lSS.str().c_str())); lSS.str(""); 
  lSS << iPrefix << "u1sigma2"; iU1RMS2Fit.push_back( (TGraphErrors*) lFile->FindObjectAny(lSS.str().c_str())); lSS.str(""); 
  lSS << iPrefix << "u2mean"    << lNJet; iU2Fit.push_back( (TGraphErrors*) lFile->FindObjectAny(lSS.str().c_str())); lSS.str("");
  lSS << iPrefix << "u2sigma0"; 
  iU2MRMSFit.push_back( (TGraphErrors*) lFile->FindObjectAny(lSS.str().c_str())); lSS.str("");
  lSS << iPrefix << "u2sigma1"; iU2RMS1Fit.push_back( (TGraphErrors*) lFile->FindObjectAny(lSS.str().c_str())); lSS.str("");
  lSS << iPrefix << "u2sigma2"; iU2RMS2Fit.push_back( (TGraphErrors*) lFile->FindObjectAny(lSS.str().c_str())); lSS.str("");
  lFile->Close();
}
double RecoilCorrector::diGausPVal(double iVal,double iFrac,double iSigma1,double iSigma2) { 
  return iFrac*TMath::Erf(iVal/iSigma1) + (1-iFrac)*TMath::Erf(iVal/iSigma2);
}

double RecoilCorrector::diGausPInverse(double iPVal,double iFrac,double iSigma1,double iSigma2) { 
  double lVal = TMath::ErfInverse(iPVal);
  double lMin = lVal * ((iSigma1 < iSigma2) ? iSigma1 : iSigma2);
  double lMax = lVal * ((iSigma1 < iSigma2) ? iSigma2 : iSigma1);
  double lDiff = (lMax-lMin);
  //Iterative procedure to invert a double gaussian given a PVal
  int lId = 0; int lN1 = 4;  int lN2 = 10;  // Fewer toys
  //   int lId = 0; int lN1 = 10;  int lN2 = 100;  // More toys - takes longer
  for(int i0 = 0; i0 < lN1; i0++) { 
    if(i0 != 0) lMin = lMin + (lId-1)*lDiff/lN2;
    if(i0 != 0) lDiff/=lN2;
    for(int i1 = 0; i1 < lN2; i1++) { 
      double pVal = lMin + lDiff/lN2*i1;
      pVal = diGausPVal(pVal,iFrac,iSigma1,iSigma2);
      if(pVal > iPVal) {lId = i1; break;}
    }
  }
  return (lMin + (lId-0.5)*lDiff/lN2);
}

void RecoilCorrector::metDistributionType0(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
        double iLepPt,double iLepPhi,
        TGraphErrors *iU1Default,
        TF1 *iU1RZDatFit, 
        TF1 *iU1MSZDatFit,  
        TF1 *iU1S1ZDatFit,  
        TF1 *iU1S2ZDatFit, 
        TF1 *iU2RZDatFit,
        TF1 *iU2MSZDatFit, 
        TF1 *iU2S1ZDatFit,                                              
        TF1 *iU2S2ZDatFit, 
        double &iU1,double &iU2,double iFluc,double iScale){
  
  double pfu1mean   = iU1RZDatFit->Eval(iGenPt);
  double pfu1sigma1 = iU1S1ZDatFit->Eval(iGenPt);
  double pfu1sigma2 = iU1S2ZDatFit->Eval(iGenPt);
  double pfu1sigma0 = iU1MSZDatFit->Eval(iGenPt);
  
  if(iScale!=0 || iFluc!=0) {
    pfu1mean   += iScale*dMean(iU1RZDatFit,iGenPt,fitresPFu1mean);
    pfu1sigma1 += iFluc*dSigma(iU1S1ZDatFit,iGenPt,fitresPFu1sigma1);
    pfu1sigma2 += iFluc*dSigma(iU1S2ZDatFit,iGenPt,fitresPFu1sigma2);
    pfu1sigma0 += iFluc*dSigma(iU1MSZDatFit,iGenPt,fitresPFu1sigma0);
  }
  
  double pfu2mean   = iU2RZDatFit->Eval(iGenPt);
  double pfu2sigma1 = iU2S1ZDatFit->Eval(iGenPt);
  double pfu2sigma2 = iU2S2ZDatFit->Eval(iGenPt);
  double pfu2sigma0 = iU2MSZDatFit->Eval(iGenPt);
  
  if(iScale!=0 || iFluc!=0) {
    pfu2mean   += iScale*dMean(iU2RZDatFit,iGenPt,fitresPFu2mean);
    pfu2sigma1 += iFluc*dSigma(iU2S1ZDatFit,iGenPt,fitresPFu2sigma1);
    pfu2sigma2 += iFluc*dSigma(iU2S2ZDatFit,iGenPt,fitresPFu2sigma2);
    pfu2sigma0 += iFluc*dSigma(iU2MSZDatFit,iGenPt,fitresPFu2sigma0);
  }
  
  double pfu1frac2  = (pfu1sigma0 - pfu1sigma1)/(pfu1sigma2 - pfu1sigma1);
  double pfu2frac2  = (pfu2sigma0 - pfu2sigma1)/(pfu2sigma2 - pfu2sigma1);
  
  double z1 = gRandom->Gaus(0,1);
  double z2 = gRandom->Gaus(0,1);

  double pfu1 = (gRandom->Uniform(0,1) < pfu1frac2) ? z1*pfu1sigma2+pfu1mean : z1*pfu1sigma1+pfu1mean;
  double pfu2 = (gRandom->Uniform(0,1) < pfu2frac2) ? z2*pfu2sigma2+pfu2mean : z2*pfu2sigma1+pfu2mean;

  iU1 = pfu1;
  iU2 = pfu2;
  iMet  = calculate(0,iLepPt,iLepPhi,iGenPhi,iU1,iU2);
  iMPhi = calculate(1,iLepPt,iLepPhi,iGenPhi,iU1,iU2);
}

void RecoilCorrector::metDistributionType2(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
					   double iLepPt,double iLepPhi,
					   TGraphErrors *iU1Default,
					   TF1 *iU1RZDatFit,  TGraphErrors *iU1RZMCFit,
					   TF1 *iU1MSZDatFit, TGraphErrors *iU1MSZMCFit, 
					   TF1 *iU1S1ZDatFit, TGraphErrors *iU1S1ZMCFit, 
					   TF1 *iU1S2ZDatFit, TGraphErrors *iU1S2ZMCFit, 
					   TF1 *iU2MSZDatFit, TGraphErrors *iU2MSZMCFit,
					   TF1 *iU2S1ZDatFit, TGraphErrors *iU2S1ZMCFit,  		   		                              
					   TF1 *iU2S2ZDatFit, TGraphErrors *iU2S2ZMCFit, 
					   double &iU1,double &iU2,double iFluc,double iScale) {
  double pDefU1    = iU1Default->Eval(iGenPt);
  double lRescale  = sqrt((TMath::Pi())/2.);		     
  double pDU1       = iU1RZDatFit ->Eval(iGenPt);

  double pDFrac1    = iU1MSZDatFit->Eval(iGenPt);
  double pDSigma1_1 = iU1S1ZDatFit->Eval(iGenPt);
  double pDSigma1_2 = iU1S2ZDatFit->Eval(iGenPt);
  double pDFrac2    = iU2MSZDatFit->Eval(iGenPt);
  double pDSigma2_1 = iU2S1ZDatFit->Eval(iGenPt);
  double pDSigma2_2 = iU2S2ZDatFit->Eval(iGenPt);

  double pMU1       = iU1RZMCFit  ->Eval(iGenPt);
  double pMU2       = 0; 

  double pMFrac1    = iU1MSZMCFit ->Eval(iGenPt);
  double pMSigma1_1 = iU1S1ZMCFit ->Eval(iGenPt);
  double pMSigma1_2 = iU1S2ZMCFit ->Eval(iGenPt);
  double pMFrac2    = iU2MSZMCFit ->Eval(iGenPt);
  double pMSigma2_1 = iU2S1ZMCFit ->Eval(iGenPt);
  double pMSigma2_2 = iU2S2ZMCFit ->Eval(iGenPt);

  
  //Uncertainty propagation
  if(iFluc != 0 || iScale != 0) {
    fId = 0;
    
    double lEUR1    = dMean(iU1RZDatFit,iGenPt,fitresPFu1mean);
    double lEUS1_1  = dSigma(iU1S1ZDatFit,iGenPt,fitresPFu1sigma1);
    double lEUS1_2  = dSigma(iU1S2ZDatFit,iGenPt,fitresPFu1sigma2);
    double lEU1Frac = dSigma(iU1MSZDatFit,iGenPt,fitresPFu1sigma0);
    double lEUS2_1  = dSigma(iU2S1ZDatFit,iGenPt,fitresPFu2sigma1);
    double lEUS2_2  = dSigma(iU2S2ZDatFit,iGenPt,fitresPFu2sigma2);
    double lEU2Frac = dSigma(iU2MSZDatFit,iGenPt,fitresPFu2sigma0);
        
    //Modify all the different parameters the choice of signs makes it maximal
    pDU1       = pDU1       + iScale*lEUR1;            //Recoil
    pDFrac1    = pDFrac1    + iFluc*(lEU1Frac);        //Mean RMS 
    pDSigma1_1 = pDSigma1_1 + iFluc*lEUS1_1;           //Sigma 1 smalles sigma
    pDSigma1_2 = pDSigma1_2 + iFluc*lEUS1_2;           //Sigma 2 (Maximal when oppsite sigma 1)
    pDFrac2    = pDFrac2    + iFluc*(lEU2Frac);        //Mean RMS for U2
    pDSigma2_1 = pDSigma2_1 + iFluc*lEUS2_1;           //Sigma 1 U2
    pDSigma2_2 = pDSigma2_2 + iFluc*lEUS2_2;           //Sigma 2 (Maximal when oppsite sigma 1)
  }
  
  pDFrac1           = (pDFrac1-pDSigma1_2)/(pDSigma1_1-pDSigma1_2);
  pDFrac2           = (pDFrac2-pDSigma2_2)/(pDSigma2_1-pDSigma2_2);
  pMFrac1           = (pMFrac1-pMSigma1_2)/(pMSigma1_1-pMSigma1_2);
  pMFrac2           = (pMFrac2-pMSigma2_2)/(pMSigma2_1-pMSigma2_2);

  double pUX   = iMet*cos(iMPhi) + iLepPt*cos(iLepPhi);
  double pUY   = iMet*sin(iMPhi) + iLepPt*sin(iLepPhi);
  double pU    = sqrt(pUX*pUX+pUY*pUY);
  double pCos  = - (pUX*cos(iGenPhi) + pUY*sin(iGenPhi))/pU;
  double pSin  =   (pUX*sin(iGenPhi) - pUY*cos(iGenPhi))/pU;
  double pU1   = pU*pCos;
  double pU2   = pU*pSin;
  double pU1Diff  = pU1-pDefU1;
  double pU2Diff  = pU2;

  double p1Charge        = pU1Diff/fabs(pU1Diff);
  double p2Charge        = pU2Diff/fabs(pU2Diff);
  double pTU1Diff        = pU1Diff;
  double pU1ValM         = diGausPVal(fabs(pU1Diff),pMFrac1,pMSigma1_1,pMSigma1_2);
  double pU2ValM         = diGausPVal(fabs(pU2Diff),pMFrac2,pMSigma2_1,pMSigma2_2);
  double pU1ValD         = diGausPInverse(pU1ValM  ,pDFrac1,pDSigma1_1,pDSigma1_2);
  double pU2ValD         = diGausPInverse(pU2ValM  ,pDFrac2,pDSigma2_1,pDSigma2_2);
  
  pU1ValD*=p1Charge;
  pU2ValD*=p2Charge;
  pDefU1 *= (pDU1/pMU1);
  
  pU1   = pDefU1             + pU1ValD;
  pU2   =                      pU2ValD;
  iMet  = calculate(0,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  iMPhi = calculate(1,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  iU1   = pU1; 
  iU2   = pU2;
  return;
}

void RecoilCorrector::metDistributionType2FromGraph(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
					   double iLepPt,double iLepPhi,
					   TGraphErrors *iU1Default,
					   TGraphErrors *iU1RZDatFit,  TGraphErrors *iU1RZMCFit,
					   TGraphErrors *iU1MSZDatFit, TGraphErrors *iU1MSZMCFit, 
					   TGraphErrors *iU1S1ZDatFit, TGraphErrors *iU1S1ZMCFit, 
					   TGraphErrors *iU1S2ZDatFit, TGraphErrors *iU1S2ZMCFit, 
					   TGraphErrors *iU2MSZDatFit, TGraphErrors *iU2MSZMCFit,
					   TGraphErrors *iU2S1ZDatFit, TGraphErrors *iU2S1ZMCFit,  		   		                              
					   TGraphErrors *iU2S2ZDatFit, TGraphErrors *iU2S2ZMCFit, 
					   double &iU1,double &iU2,double iFluc,double iScale) {

  double pDefU1    = iU1Default->Eval(iGenPt);
  double lRescale  = sqrt((TMath::Pi())/2.);		     
  double pDU1       = iU1RZDatFit ->Eval(iGenPt);

  double pDFrac1    = iU1MSZDatFit->Eval(iGenPt);
  double pDSigma1_1 = iU1S1ZDatFit->Eval(iGenPt);
  double pDSigma1_2 = iU1S2ZDatFit->Eval(iGenPt);
  double pDFrac2    = iU2MSZDatFit->Eval(iGenPt);
  double pDSigma2_1 = iU2S1ZDatFit->Eval(iGenPt);
  double pDSigma2_2 = iU2S2ZDatFit->Eval(iGenPt);

  double pMU1       = iU1RZMCFit  ->Eval(iGenPt);
  double pMU2       = 0; 

  double pMFrac1    = iU1MSZMCFit ->Eval(iGenPt);
  double pMSigma1_1 = iU1S1ZMCFit ->Eval(iGenPt);
  double pMSigma1_2 = iU1S2ZMCFit ->Eval(iGenPt);
  double pMFrac2    = iU2MSZMCFit ->Eval(iGenPt);
  double pMSigma2_1 = iU2S1ZMCFit ->Eval(iGenPt);
  double pMSigma2_2 = iU2S2ZMCFit ->Eval(iGenPt);

  if(iFluc != 0 || iScale != 0) {
    
    // get the bins for the tgraph
    Double_t ptbins[] = {0,2,6,10,20,30,40,55,70,100};
    Int_t nbins = sizeof(ptbins)/sizeof(Double_t)-1;
    
    double lEUR1    = 0;
    double lEUS1_1  = 0;
    double lEUS1_2  = 0;
    double lEU1Frac = 0;
    double lEUS2_1  = 0;
    double lEUS2_2  = 0;
    double lEU2Frac = 0;
    
    // lookup the bin which the iGenPt belongs in
    // linear interpolate this bin and the next bin?
    for(int iBin = 0; iBin < nbins; ++iBin){
      if(iGenPt < ptbins[iBin]) continue; // skip if we are not in the right bin
      if(iGenPt > ptbins[iBin+1]) continue;
      if(iGenPt > ptbins[iBin] && iGenPt < ptbins[iBin+1]){
        lEUR1    = calcErrorGraph(iU1RZDatFit, iGenPt, ptbins, iBin); 
        lEUS1_1  = calcErrorGraph(iU1S1ZDatFit,iGenPt, ptbins, iBin); 
        lEUS1_2  = calcErrorGraph(iU1S2ZDatFit,iGenPt, ptbins, iBin);
        lEU1Frac = calcErrorGraph(iU1MSZDatFit,iGenPt, ptbins, iBin);
        lEUS2_1  = calcErrorGraph(iU2S1ZDatFit,iGenPt, ptbins, iBin);
        lEUS2_2  = calcErrorGraph(iU2S2ZDatFit,iGenPt, ptbins, iBin);
        lEU2Frac = calcErrorGraph(iU2MSZDatFit,iGenPt, ptbins, iBin);
        break; 
      }
    }
       
    pDU1       = pDU1       + iScale*lEUR1;             //Recoil
    pDFrac1    = pDFrac1    + iFluc*(lEU1Frac);        //Mean RMS 
    pDSigma1_1 = pDSigma1_1 + iFluc*lEUS1_1;//lEUS1_1*pDFrac1;    //Sigma 1 smalles sigma
    pDSigma1_2 = pDSigma1_2 + iFluc*lEUS1_2;//lEUS1_2*pDFrac1;    //Sigma 2 (Maximal when oppsite sigma 1)
    pDFrac2    = pDFrac2    + iFluc*(lEU2Frac);        //Mean RMS for U2
    pDSigma2_1 = pDSigma2_1 + iFluc*lEUS2_1;//lEUS2_1*pDFrac2;    //Sigma 1 U2
    pDSigma2_2 = pDSigma2_2 + iFluc*lEUS2_2;//(lEUS2_2)*pDFrac2;
  }
  
  
  pDFrac1           = (pDFrac1-pDSigma1_2)/(pDSigma1_1-pDSigma1_2);
  pDFrac2           = (pDFrac2-pDSigma2_2)/(pDSigma2_1-pDSigma2_2);
  pMFrac1           = (pMFrac1-pMSigma1_2)/(pMSigma1_1-pMSigma1_2);
  pMFrac2           = (pMFrac2-pMSigma2_2)/(pMSigma2_1-pMSigma2_2);
  double pUX  = iMet*cos(iMPhi) + iLepPt*cos(iLepPhi);
  double pUY  = iMet*sin(iMPhi) + iLepPt*sin(iLepPhi);
  double pU   = sqrt(pUX*pUX+pUY*pUY);
  double pCos = - (pUX*cos(iGenPhi) + pUY*sin(iGenPhi))/pU;
  double pSin =   (pUX*sin(iGenPhi) - pUY*cos(iGenPhi))/pU;
  double pU1   = pU*pCos;
  double pU2   = pU*pSin;
  double pU1Diff  = pU1-pDefU1;
  double pU2Diff  = pU2;

  double p1Charge        = pU1Diff/fabs(pU1Diff);
  double p2Charge        = pU2Diff/fabs(pU2Diff);
  double pTU1Diff        = pU1Diff;
  double pU1ValM         = diGausPVal(fabs(pU1Diff),pMFrac1,pMSigma1_1,pMSigma1_2);
  double pU2ValM         = diGausPVal(fabs(pU2Diff),pMFrac2,pMSigma2_1,pMSigma2_2);
  double pU1ValD         = diGausPInverse(pU1ValM  ,pDFrac1,pDSigma1_1,pDSigma1_2);
  double pU2ValD         = diGausPInverse(pU2ValM  ,pDFrac2,pDSigma2_1,pDSigma2_2);
  
  pU1ValD*=p1Charge;
  pU2ValD*=p2Charge;
  pDefU1 *= (pDU1/pMU1);
  
  pU1   = pDefU1             + pU1ValD;
  pU2   =                      pU2ValD;
  iMet  = calculate(0,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  iMPhi = calculate(1,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  iU1   = pU1; 
  iU2   = pU2;
  return;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
double RecoilCorrector::calculate(int iMet,double iEPt,double iEPhi,double iWPhi,double iU1,double iU2) { 
  double lMX = -iEPt*cos(iEPhi) - iU1*cos(iWPhi) + iU2*sin(iWPhi);
  double lMY = -iEPt*sin(iEPhi) - iU1*sin(iWPhi) - iU2*cos(iWPhi);
  if(iMet == 0) return sqrt(lMX*lMX + lMY*lMY);
  if(iMet == 1) {if(lMX > 0) {return atan(lMY/lMX);} return (fabs(lMY)/lMY)*3.14159265 + atan(lMY/lMX); } 
  if(iMet == 2) return lMX;
  if(iMet == 3) return lMY;
  return lMY;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
double RecoilCorrector::getCorError2(double iVal,TF1 *iFit) { 
  double lE = sqrt(iFit->GetParError(0))  + iVal*sqrt(iFit->GetParError(2));
  if(fabs(iFit->GetParError(4)) > 0) lE += iVal*iVal*sqrt(iFit->GetParError(4));
  return lE*lE;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
double RecoilCorrector::mag(double iV0,double iV1,double iV2,double iV3) { 
  return sqrt(iV0+iV1*iV1+2*iV1*0.88 + iV2*iV2+2.*iV2*0.88+ iV3*iV3+2.*iV3*0.88);//
}

//-----------------------------------------------------------------------------------------------------------------------------------------
double RecoilCorrector::correlatedSeed(double iVal, double iCorr1,double iCorr2,double iCorr3,double iSeed0,double iSeed1,double iSeed2,double iSeed3) { 
  double lMag = mag(1.,iCorr1,iCorr2,iCorr3); 
  double lVal = ((1./lMag) + (iCorr1/lMag)*(iSeed1) + (iCorr2/lMag)*fabs(iSeed2) + (iCorr3/lMag)*fabs(iSeed3))*iVal;
  lVal*=iSeed0;
  return lVal;
}
double RecoilCorrector::deCorrelate(double iVal, double iCorr1,double iCorr2,double iCorr3,double iSeed0,double iSeed1,double iSeed2,double iSeed3) { 
  double lMag = mag(1.,iCorr1,iCorr2,iCorr3); 
  double lVal =  (1 - iCorr1*fabs(iSeed1) - iCorr2*fabs(iSeed2) - iCorr3*fabs(iSeed3))*lMag;
  return lVal*iVal*iSeed0;
}

Double_t RecoilCorrector::dSigma(const TF1 *fcn, const Double_t x, const TFitResult *fs) {
  Double_t df[3];
  Double_t a  = fcn->GetParameter(0);
  Double_t b  = fcn->GetParameter(1);
  Double_t c  = fcn->GetParameter(2);
  
  df[0] = x*x;
  df[1] = x;
  df[2] = 1;
  
  Double_t err2=0;
  for(Int_t i=0; i<3; i++) {
    err2 += df[i]*df[i]*(fs->GetCovarianceMatrix()[i][i]);
    for(Int_t j=i+1; j<3; j++) {
      err2 += 2.0*df[i]*df[j]*(fs->GetCovarianceMatrix()[i][j]);
    }
  }
  assert(err2>=0);
  return sqrt(err2);
}

Double_t RecoilCorrector::dMean(const TF1 *fcn, const Double_t x, const TFitResult *fs) {
  Double_t df[2];
  df[0] = 1;
  df[1] = x;
  Double_t err2 = df[0]*df[0]*(fs->GetCovarianceMatrix()[0][0]) 
                  + df[1]*df[1]*(fs->GetCovarianceMatrix()[1][1]) 
		  + 2.0*df[0]*df[1]*(fs->GetCovarianceMatrix()[0][1]);
  assert(err2>=0);
  return sqrt(err2);
}

Double_t RecoilCorrector::calcErrorGraph(const TGraphErrors *graph, const Double_t x, const Double_t bins[], const Int_t bin) {
  Double_t diff = (graph->GetErrorY(bin)*(bins[bin+1]-x) + graph->GetErrorY(bin+1)*(x - bins[bin]));
  Double_t avg = diff/((bins[bin+1]-bins[bin]));
  return avg;
}