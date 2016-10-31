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

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hEtaPlus_mult.pdf"          --map=hEtaPlus          --mode=$mode --func='([0]+x*[1]+x**2*[2])' --fitRange=0,400   --rebin=0 --yZoomRange=-2,4 --xZoomRange=0,600 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hEtaMinus_mult.pdf"          --map=hEtaMinus          --mode=$mode --func='([0]+x*[1]+x**2*[2])' --fitRange=0,400   --rebin=0 --yZoomRange=-8,4 --xZoomRange=0,600 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="h0Barrel_mult.pdf"          --map=h0Barrel          --mode=$mode --func='([0]+x*[1]+x**2*[2]+x**3*[3])' --fitRange=0,80   --rebin=5 --yZoomRange=-2,2 --xZoomRange=0,120 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="h0EndcapPlus_mult.pdf"          --map=h0EndcapPlus          --mode=$mode --func='([0]+x*[1]+x**2*[2])' --fitRange=0,100   --rebin=5 --yZoomRange=-2,4 --xZoomRange=0,120 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="h0EndcapMinus_mult.pdf"          --map=h0EndcapMinus          --mode=$mode --func='([0]+x*[1]+x**2*[2]+x**3*[3])' --fitRange=0,100   --rebin=5 --yZoomRange=-0.5,4 --xZoomRange=0,120 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="gammaBarrel_mult.pdf"          --map=gammaBarrel          --mode=$mode --func='([0]+x*[1]+x**2*[2])' --fitRange=0,300   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,300 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="gammaEndcapPlus_mult.pdf"          --map=gammaEndcapPlus          --mode=$mode --func='([0]+x*[1]+x**2*[2]+x**3*[3])' --fitRange=0,140   --rebin=0 --yZoomRange=-2.5,2.5 --xZoomRange=0,150 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="gammaEndcapMinus_mult.pdf"          --map=gammaEndcapMinus          --mode=$mode --func='([0]+x*[1]+x**2*[2]+x**3*[3])' --fitRange=0,140   --rebin=0 --yZoomRange=-2.5,2.5 --xZoomRange=0,150 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="gammaForwardPlus_mult.pdf"          --map=gammaForwardPlus          --mode=$mode --func='([0]+x*[1]+x**2*[2]+x**3*[3])' --fitRange=0,12   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,15 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="gammaForwardMinus_mult.pdf"          --map=gammaForwardMinus          --mode=$mode --func='([0]+x*[1]+x**2*[2]+x**3*[3])' --fitRange=0,12   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,15 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="e_mult.pdf"          --map=e          --mode=$mode --func='([0]+x*[1]+x**2*[2])' --fitRange=0,50   --rebin=5 --yZoomRange=-1,1 --xZoomRange=0,50 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="mu_mult.pdf"          --map=mu          --mode=$mode --func='([0]+x*[1]+x**2*[2])' --fitRange=0,50   --rebin=5 --yZoomRange=-1,1 --xZoomRange=0,50 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hHFPlus_mult.pdf"          --map=hHFPlus          --mode=$mode --func='([0]+x*[1]+x**2*[2])' --fitRange=0,400   --rebin=0 --yZoomRange=-1,4 --xZoomRange=0,400 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hHFMinus_mult.pdf"          --map=hHFMinus          --mode=$mode --func='([0]+x*[1]+x**2*[2])' --fitRange=0,400   --rebin=0 --yZoomRange=-1,4 --xZoomRange=0,400 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="egammaHFPlus_mult.pdf"          --map=egammaHFPlus          --mode=$mode --func='([0]+x*[1]+x**2*[2]+x**3*[3])' --fitRange=0,80   --rebin=0 --yZoomRange=-0.5,4 --xZoomRange=0,100 --batch=$isBatch 

python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="egammaHFMinus_mult.pdf"          --map=egammaHFMinus          --mode=$mode --func='([0]+x*[1]+x**2*[2]+x**3*[3])' --fitRange=0,80   --rebin=0 --yZoomRange=-2,2 --xZoomRange=0,100 --batch=$isBatch 
echo ")">>$plotoutPutDirString/$outputScriptFile
echo "Outputs written to: $plotoutPutDirString/$outputScriptFile"
