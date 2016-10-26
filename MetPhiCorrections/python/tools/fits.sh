#!/bin/sh
if [ -n "$1" ]; then
    argum=$1
  else
    argum="tmp"
fi
mood=$4
outputTextFile="multPhiCorr_${argum}_${mood}_cfi.py"
inputTextFile=$2
plotoutPutDirString=$3
#inputTextFile="/data/easilar/METPhiCorr/RootFiles/DY_Fall15/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1_V2.root"
#plotoutPutDirString="/afs/hephy.at/user/e/easilar/www/METPhiCorr/WJets_Spring15/"
rm -rf $outputTextFile
echo "import FWCore.ParameterSet.Config as cms">>$outputTextFile
echo "multPhiCorr_${argum} = cms.VPSet(">>$outputTextFile
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hEtaPlus_sumPt.pdf"          --map=hEtaPlus          --mode=$mood --func='(x*[0]+x**2*[1])' --fitRange=20,500   --rebin=0 --yZoomRange=-5,5 --xZoomRange=0,800  
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hEtaMinus_sumPt.pdf"         --map=hEtaMinus         --mode=$mood --func='(x*[0]+x**2*[1])' --fitRange=50,500   --rebin=0 --yZoomRange=-5,5 --xZoomRange=0,800  
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="h0Barrel_sumPt.pdf"          --map=h0Barrel          --mode=$mood --func='(x*[0]+x**2*[1])' --fitRange=5,30    --rebin=5 --yZoomRange=-3,3 --xZoomRange=0,50   
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="h0EndcapPlus_sumPt.pdf"      --map=h0EndcapPlus      --mode=$mood --func='(x*[0]+x**2*[1])' --fitRange=5,55    --rebin=5 --yZoomRange=-3,3 --xZoomRange=0,50   
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="h0EndcapMinus_sumPt.pdf"     --map=h0EndcapMinus     --mode=$mood --func='(x*[0]+x**2*[1])' --fitRange=5,50    --rebin=5 --yZoomRange=-3,3 --xZoomRange=0,50   
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="gammaBarrel_sumPt.pdf"       --map=gammaBarrel       --mode=$mood --func='(x*[0]+x**2*[1])' --fitRange=0,250   --rebin=0 --yZoomRange=-2,2 --xZoomRange=0,500  
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="gammaEndcapPlus_sumPt.pdf"   --map=gammaEndcapPlus   --mode=$mood --func='(x*[0]+x**2*[1])' --fitRange=5,80    --rebin=0 --yZoomRange=-3,3 --xZoomRange=0,100  
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="gammaEndcapMinus_sumPt.pdf"  --map=gammaEndcapMinus  --mode=$mood --func='(x*[0]+x**2*[1])' --fitRange=5,70    --rebin=0 --yZoomRange=-3,3 --xZoomRange=0,100  
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hHFPlus_sumPt.pdf"           --map=hHFPlus           --mode=$mood --func='(x*[0]+x**2*[1])' --fitRange=20,250  --rebin=0 --yZoomRange=-4,20 --xZoomRange=0,300  
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hHFMinus_sumPt.pdf"          --map=hHFMinus          --mode=$mood --func='(x*[0]+x**2*[1])' --fitRange=10,250   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,300  
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="egammaHFPlus_sumPt.pdf"      --map=egammaHFPlus      --mode=$mood --func='( (x*(x<100)+100*(x>=100))*[0]+(x*(x<100)+100*(x>=100))**2*[1])' --fitRange=10,200   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,300  
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="egammaHFMinus_sumPt.pdf"     --map=egammaHFMinus     --mode=$mood --func='( (x*(x<100)+100*(x>=100))*[0]+(x*(x<100)+100*(x>=100))**2*[1])' --fitRange=0,180   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,300  
echo ")">>$outputTextFile
echo "Outputs written to: $outputTextFile"
