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
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hEtaPlus_ngoodVertices.pdf"          --map=hEtaPlus          --mode=$mood --func='([0] + x*[1]+x**2*[2])' --fitRange=0,80   --rebin=0 --yZoomRange=-20,20 --xZoomRange=0,100
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hEtaMinus_ngoodVertices.pdf"         --map=hEtaMinus         --mode=$mood --func='([0] + x*[1]+x**2*[2])' --fitRange=6,80   --rebin=0 --yZoomRange=-10,10 --xZoomRange=0,100
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="h0Barrel_ngoodVertices.pdf"          --map=h0Barrel          --mode=$mood --func='([0] + x*[1]+x**2*[2])' --fitRange=2,80    --rebin=0 --yZoomRange=-10,10 --xZoomRange=0,100
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="h0EndcapPlus_ngoodVertices.pdf"      --map=h0EndcapPlus      --mode=$mood --func='([0] + x*[1]+x**2*[2])' --fitRange=0,80    --rebin=0 --yZoomRange=-10,10 --xZoomRange=0,100
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="h0EndcapMinus_ngoodVertices.pdf"     --map=h0EndcapMinus     --mode=$mood --func='([0] + x*[1]+x**2*[2])' --fitRange=6,80    --rebin=0 --yZoomRange=-10,10 --xZoomRange=0,100
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="gammaBarrel_ngoodVertices.pdf"       --map=gammaBarrel       --mode=$mood --func='([0] + x*[1]+x**2*[2])' --fitRange=0,80   --rebin=0 --yZoomRange=-2,2 --xZoomRange=0,100
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="gammaEndcapPlus_ngoodVertices.pdf"   --map=gammaEndcapPlus   --mode=$mood --func='([0] + x*[1]+x**2*[2])' --fitRange=0,80    --rebin=0 --yZoomRange=-3,3 --xZoomRange=0,100
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="gammaEndcapMinus_ngoodVertices.pdf"  --map=gammaEndcapMinus  --mode=$mood --func='([0] + x*[1]+x**2*[2])' --fitRange=0,80    --rebin=0 --yZoomRange=-3,3 --xZoomRange=0,100
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hHFPlus_ngoodVertices.pdf"           --map=hHFPlus           --mode=$mood --func='([0] + x*[1]+x**2*[2])' --fitRange=0,80  --rebin=0 --yZoomRange=-10,10 --xZoomRange=0,100
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="hHFMinus_ngoodVertices.pdf"          --map=hHFMinus          --mode=$mood --func='([0] + x*[1]+x**2*[2])' --fitRange=0,80   --rebin=0 --yZoomRange=-10,10 --xZoomRange=0,100
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="egammaHFPlus_ngoodVertices.pdf"      --map=egammaHFPlus      --mode=$mood --func='([0] + (x*(x<100)+100*(x>=100))*[1]+(x*(x<100)+100*(x>=100))**2*[2])' --fitRange=0,80   --rebin=0 --yZoomRange=-2,2 --xZoomRange=0,100
python multiplicityFit.py  --textFileName=$outputTextFile --input=$inputTextFile --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$plotoutPutDirString --plotFileName="egammaHFMinus_ngoodVertices.pdf"     --map=egammaHFMinus     --mode=$mood --func='([0] + (x*(x<100)+100*(x>=100))*[1]+(x*(x<100)+100*(x>=100))**2*[2])' --fitRange=0,80   --rebin=0 --yZoomRange=-2,2 --xZoomRange=0,100
echo ")">>$outputTextFile
echo "Outputs written to: $outputTextFile"
