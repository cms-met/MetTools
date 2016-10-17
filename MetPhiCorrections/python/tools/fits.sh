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
rm -rf ${plotoutPutDirString}_Old
mv $plotoutPutDirString ${plotoutPutDirString}_Old
mkdir $plotoutPutDirString

#inputRoot="/data/easilar/METPhiCorr/RootFiles/DY_Fall15/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1_V2.root"
#plotoutPutDirString="/afs/hephy.at/user/e/easilar/www/METPhiCorr/WJets_Spring15/"
rm -rf $outputScriptFile
rm -rf $outputTxtFile
echo "import FWCore.ParameterSet.Config as cms">>$outputScriptFile
echo "multPhiCorr_${argum} = cms.VPSet(">>$outputScriptFile
#python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=pfMEtMultCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="h_DY_mult.pdf"                 --map=h                 --mode=multiplicity --func='(x*[0]+x**2*[1])' --fitRange=0,3000 --rebin=5 --yZoomRange=-40,40 --xZoomRange=0,4000
python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hEtaPlus_mult.pdf"          --map=hEtaPlus          --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,500   --rebin=0 --yZoomRange=-5,5 --xZoomRange=0,800  
python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hEtaMinus_mult.pdf"         --map=hEtaMinus         --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,500   --rebin=0 --yZoomRange=-5,5 --xZoomRange=0,800  
python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="h0Barrel_mult.pdf"          --map=h0Barrel          --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,30    --rebin=5 --yZoomRange=-3,3 --xZoomRange=0,50   
python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="h0EndcapPlus_mult.pdf"      --map=h0EndcapPlus      --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,55    --rebin=5 --yZoomRange=-3,3 --xZoomRange=0,50   
python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="h0EndcapMinus_mult.pdf"     --map=h0EndcapMinus     --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,50    --rebin=5 --yZoomRange=-3,3 --xZoomRange=0,50   
python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="gammaBarrel_mult.pdf"       --map=gammaBarrel       --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,250   --rebin=0 --yZoomRange=-2,2 --xZoomRange=0,500  
python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="gammaEndcapPlus_mult.pdf"   --map=gammaEndcapPlus   --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,80    --rebin=0 --yZoomRange=-3,3 --xZoomRange=0,100  
python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="gammaEndcapMinus_mult.pdf"  --map=gammaEndcapMinus  --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,70    --rebin=0 --yZoomRange=-3,3 --xZoomRange=0,100  
python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hHFPlus_mult.pdf"           --map=hHFPlus           --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=20,250  --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,300  
python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hHFMinus_mult.pdf"          --map=hHFMinus          --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,250   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,300  
python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="egammaHFPlus_mult.pdf"      --map=egammaHFPlus      --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,170   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,300  
python multiplicityFit.py  --scriptFileName=$outputScriptFile --input=$inputRoot --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="egammaHFMinus_mult.pdf"     --map=egammaHFMinus     --mode=$mode --func='(x*[0]+x**2*[1])' --fitRange=0,180   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,300  
echo ")">>$outputScriptFile
echo "Outputs written to: $outputScriptFile"
