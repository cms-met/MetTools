#!/bin/sh
if [ -n "$1" ]; then
    argum=$1
  else
    argum="tmp"
fi
mode=$4
outputScriptFile="multPhiCorr_${argum}_${mode}_cfi.py"
outputTxtFile="multPhiCorr_${argum}_${mode}.txt"
inputRoot=$2
plotoutPutDirString=$3_$1
isBatch=$5
rm -rf ${plotoutPutDirString}_Old
mv $plotoutPutDirString ${plotoutPutDirString}_Old
mkdir $plotoutPutDirString

#inputRoot="/data/easilar/METPhiCorr/RootFiles/DY_Fall15/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1_V2.root"
#plotoutPutDirString="/afs/hephy.at/user/e/easilar/www/METPhiCorr/WJets_Spring15/"
rm -rf $outputScriptFile
rm -rf $outputTxtFile
echo "import FWCore.ParameterSet.Config as cms">>$plotoutPutDirString/$outputScriptFile
echo "multPhiCorr_${argum} = cms.VPSet(">>$plotoutPutDirString/$outputScriptFile

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hEtaPlus_mult.pdf"          --map=hEtaPlus          --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,1200   --rebin=0 --yZoomRange=-150,150 --xZoomRange=0,1200 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hEtaMinus_mult.pdf"          --map=hEtaMinus          --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,1200   --rebin=0 --yZoomRange=-150,150 --xZoomRange=0,1200 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="h0Barrel_mult.pdf"          --map=h0Barrel          --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,250   --rebin=5 --yZoomRange=-150,150 --xZoomRange=0,250 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="h0EndcapPlus_mult.pdf"          --map=h0EndcapPlus          --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,250   --rebin=5 --yZoomRange=-150,150 --xZoomRange=0,250 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="h0EndcapMinus_mult.pdf"          --map=h0EndcapMinus          --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,250   --rebin=5 --yZoomRange=-150,150 --xZoomRange=0,250 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="gammaBarrel_mult.pdf"          --map=gammaBarrel          --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,750   --rebin=0 --yZoomRange=-150,150 --xZoomRange=0,750 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="gammaEndcapPlus_mult.pdf"          --map=gammaEndcapPlus          --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,400   --rebin=0 --yZoomRange=-150,150 --xZoomRange=0,400 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="gammaEndcapMinus_mult.pdf"          --map=gammaEndcapMinus          --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,400   --rebin=0 --yZoomRange=-150,150 --xZoomRange=0,400 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="gammaForwardPlus_mult.pdf"          --map=gammaForwardPlus          --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,20   --rebin=0 --yZoomRange=-10,10 --xZoomRange=0,20 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="gammaForwardMinus_mult.pdf"          --map=gammaForwardMinus          --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,12   --rebin=0 --yZoomRange=-10,10 --xZoomRange=0,12 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="e_mult.pdf"          --map=e          --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,50   --rebin=0 --yZoomRange=-10,10 --xZoomRange=0,50 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="mu_mult.pdf"          --map=mu          --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,50   --rebin=0 --yZoomRange=-30,30 --xZoomRange=0,50 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hHFPlus_mult.pdf"          --map=hHFPlus          --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,600   --rebin=0 --yZoomRange=-100,100 --xZoomRange=0,800 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hHFMinus_mult.pdf"          --map=hHFMinus          --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,600   --rebin=0 --yZoomRange=-100,100 --xZoomRange=0,600 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="egammaHFPlus_mult.pdf"          --map=egammaHFPlus          --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,200   --rebin=0 --yZoomRange=-100,100 --xZoomRange=0,300 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="egammaHFMinus_mult.pdf"          --map=egammaHFMinus          --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,180   --rebin=0 --yZoomRange=-100,100 --xZoomRange=0,200 --batch=$isBatch 
echo ")">>$plotoutPutDirString/$outputScriptFile
echo "Outputs written to: $plotoutPutDirString/$outputScriptFile"
