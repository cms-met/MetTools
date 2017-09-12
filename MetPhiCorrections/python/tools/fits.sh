#!/bin/sh
if [ $# != 5 ]; then
  echo "Usage: $0 Era SampleTag varType[multiplicity,ngoodVertices,sumPt] Input isBatch"
fi

Era=$1
SampleTag=$2
varType=$3
inputData=$4
isBatch=$5

BaseName=${Era}_${SampleTag}

outputScriptFile=multPhiCorr_${BaseName}_${varType}_cfi.py
ResultDir=Res_${BaseName}
rm -rf ${ResultDir}_Old
mv $ResultDir ${ResultDir}_Old
mkdir $ResultDir

echo "Environment parameters"
echo "Era: " $Era " Sample: " $SampleTag " varType: " $varType "inputData: " $inputData " isBatch:" $isBatch
echo "Result is installed in " $ResultDir " outParameterScript:" $outputScriptFile



#inputData="/data/easilar/METPhiCorr/RootFiles/DY_Fall15/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1_V2.root"
#ResultDir="/afs/hephy.at/user/e/easilar/www/METPhiCorr/WJets_Spring15/"
echo "import FWCore.ParameterSet.Config as cms">>$ResultDir/$outputScriptFile
echo "multPhiCorr_${BaseName} = cms.VPSet(">>$ResultDir/$outputScriptFile

basicFtn='([0]*x+[1]*x*x+[2]*x*x*x)'

if [ $varType = "pfType1" ]; then 

  python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir  --map=AllPtc   --varType=$varType --func=$basicFtn --fitRange=0,100  --rebin=0 --yZoomRange=-5,15 --xZoomRange=0,150 --batch=$isBatch 



elif [ $varType = "metPhiNvtx" ]; then 

  python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir  --map=hEtaPlus   --varType=$varType --func=$basicFtn --fitRange1=-3.141592,-2.3 --fitRange2=-2.3,0.57 --fitRange3=0.57,3.141592   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,100 --batch=$isBatch 



elif [ $varType = "ptcMet" ]; then #ptcMet, ptValence: variable is phi

  python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir  --map=hEtaPlus   --varType=$varType --func=$basicFtn --fitRange=-3.141592,3.141592   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,100 --batch=$isBatch 

  python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir   --map=hEtaMinus --varType=$varType --func=$basicFtn --fitRange=-3.141592,3.141592   --rebin=0 --yZoomRange=-9,1 --xZoomRange=0,100 --batch=$isBatch 

  python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir   --map=h0Barrel  --varType=$varType --func=$basicFtn --fitRange=-3.141592,3.141592   --rebin=0 --yZoomRange=-5,3 --xZoomRange=0,90 --batch=$isBatch 

  python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --map=h0EndcapPlus  --varType=$varType --func=$basicFtn --fitRange=-3.141592,3.141592   --rebin=0 --yZoomRange=-2,4 --xZoomRange=0,80 --batch=$isBatch 

  python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --map=h0EndcapMinus --varType=$varType --func=$basicFtn --fitRange=-3.141592,3.141592   --rebin=0 --yZoomRange=-8,8 --xZoomRange=0,80 --batch=$isBatch 

  python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --map=gammaBarrel  --varType=$varType --func=$basicFtn --fitRange=-3.141592,3.141592   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,90 --batch=$isBatch 

  python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --map=gammaEndcapPlus  --varType=$varType --func=$basicFtn --fitRange=-3.141592,3.141592   --rebin=0 --yZoomRange=-5.,5 --xZoomRange=0,80 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --map=gammaEndcapMinus  --varType=$varType --func=$basicFtn --fitRange=-3.141592,3.141592   --rebin=0 --yZoomRange=-3,3 --xZoomRange=0,80 --batch=$isBatch 

#python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="gammaForwardPlus_mult.pdf"          --map=gammaForwardPlus          --varType=$varType --func='(x*[0]+x**2*[1]+x**3*[2])' --fitRange=0,12   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,15 --batch=$isBatch 

#python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="gammaForwardMinus_mult.pdf"          --map=gammaForwardMinus          --varType=$varType --func='(x*[0]+x**2*[1]+x**3*[2])' --fitRange=0,12   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,15 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir       --map=e    --varType=$varType --func=$basicFtn --fitRange=-3.141592,3.141592   --rebin=0 --yZoomRange=-2,2 --xZoomRange=0,50 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir    --map=mu      --varType=$varType --func=$basicFtn --fitRange=-3.141592,3.141592   --rebin=0 --yZoomRange=-5,5 --xZoomRange=0,100 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir  --map=hHFMinus  --varType=$varType --func=$basicFtn --fitRange=-3.141592,3.141592   --rebin=0 --yZoomRange=-7,16 --xZoomRange=0,100 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir   --map=hHFPlus  --varType=$varType --func=$basicFtn --fitRange=-3.141592,3.141592   --rebin=0 --yZoomRange=-7,20 --xZoomRange=0,100 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --map=egammaHFMinus  --varType=$varType --func=$basicFtn --fitRange=-3.141592,3.141592   --rebin=0 --yZoomRange=-15,3 --xZoomRange=0,25 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --map=egammaHFPlus   --varType=$varType --func=$basicFtn --fitRange=-3.141592,3.141592   --rebin=0 --yZoomRange=-3,25 --xZoomRange=0,25 --batch=$isBatch 


elif false; then
#  _   _         ______              _   _                     _                                   
# | \ | |       |___  /             | | | |                   | |                                  
# |  \| | ___      / / ___ _ __ ___ | |_| |__     ___  _ __ __| | ___ _ __   _ __   __ _ _ __ __ _ 
# | . ` |/ _ \    / / / _ \ '__/ _ \| __| '_ \   / _ \| '__/ _` |/ _ \ '__| | '_ \ / _` | '__/ _` |
# | |\  | (_) |  / /_|  __/ | | (_) | |_| | | | | (_) | | | (_| |  __/ |    | |_) | (_| | | | (_| |
# |_| \_|\___/  /_____\___|_|  \___/ \__|_| |_|  \___/|_|  \__,_|\___|_|    | .__/ \__,_|_|  \__,_|
#                                                                           | |                    
#                                                                           |_|                    



#######################
# ptcMet
#######################
python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir  --map=hEtaPlus           --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,50   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,100 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir   --map=hEtaMinus         --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,50   --rebin=0 --yZoomRange=-9,1 --xZoomRange=0,100 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir   --map=h0Barrel          --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,30   --rebin=0 --yZoomRange=-5,3 --xZoomRange=0,90 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --map=h0EndcapPlus        --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,20   --rebin=0 --yZoomRange=-2,4 --xZoomRange=0,80 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --map=h0EndcapMinus       --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,15   --rebin=0 --yZoomRange=-8,8 --xZoomRange=0,80 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --map=gammaBarrel          --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,20   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,90 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --map=gammaEndcapPlus      --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,20   --rebin=0 --yZoomRange=-5.,5 --xZoomRange=0,80 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --map=gammaEndcapMinus     --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,20   --rebin=0 --yZoomRange=-3,3 --xZoomRange=0,80 --batch=$isBatch 

#python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="gammaForwardPlus_mult.pdf"          --map=gammaForwardPlus          --varType=$varType --func='(x*[0]+x**2*[1]+x**3*[2])' --fitRange=0,12   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,15 --batch=$isBatch 

#python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="gammaForwardMinus_mult.pdf"          --map=gammaForwardMinus          --varType=$varType --func='(x*[0]+x**2*[1]+x**3*[2])' --fitRange=0,12   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,15 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir       --map=e          --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,10   --rebin=0 --yZoomRange=-2,2 --xZoomRange=0,50 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir    --map=mu          --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,50   --rebin=0 --yZoomRange=-5,5 --xZoomRange=0,100 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir  --map=hHFMinus          --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,40   --rebin=0 --yZoomRange=-7,16 --xZoomRange=0,100 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir   --map=hHFPlus          --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,40   --rebin=0 --yZoomRange=-7,20 --xZoomRange=0,100 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --map=egammaHFMinus      --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,15   --rebin=0 --yZoomRange=-15,3 --xZoomRange=0,25 --batch=$isBatch 

python profileFitPlots.py --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --map=egammaHFPlus          --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,15   --rebin=0 --yZoomRange=-3,25 --xZoomRange=0,25 --batch=$isBatch 


elif false; then

####################################3
# sumpt
####################################3
python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir  --map=hEtaPlus  --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,400   --rebin=0 --yZoomRange=-2,4 --xZoomRange=0,600 --batch=$isBatch 


python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="hEtaMinus_mult.pdf"          --map=hEtaMinus          --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,400   --rebin=0 --yZoomRange=-8,4 --xZoomRange=0,600 --batch=$isBatch 

python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="h0Barrel_mult.pdf"          --map=h0Barrel          --varType=$varType --func='(x*[0]+x**2*[1]+x**3*[2])' --fitRange=0,80   --rebin=5 --yZoomRange=-2,2 --xZoomRange=0,120 --batch=$isBatch 

python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="h0EndcapPlus_mult.pdf"          --map=h0EndcapPlus          --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,100   --rebin=5 --yZoomRange=-2,4 --xZoomRange=0,120 --batch=$isBatch 

python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="h0EndcapMinus_mult.pdf"          --map=h0EndcapMinus          --varType=$varType --func='(x*[0]+x**2*[1]+x**3*[2])' --fitRange=0,100   --rebin=5 --yZoomRange=-0.5,4 --xZoomRange=0,120 --batch=$isBatch 

python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="gammaBarrel_mult.pdf"          --map=gammaBarrel          --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,300   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,300 --batch=$isBatch 

python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="gammaEndcapPlus_mult.pdf"          --map=gammaEndcapPlus          --varType=$varType --func='(x*[0]+x**2*[1]+x**3*[2])' --fitRange=0,140   --rebin=0 --yZoomRange=-2.5,2.5 --xZoomRange=0,150 --batch=$isBatch 

python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="gammaEndcapMinus_mult.pdf"          --map=gammaEndcapMinus          --varType=$varType --func='(x*[0]+x**2*[1]+x**3*[2])' --fitRange=0,140   --rebin=0 --yZoomRange=-2.5,2.5 --xZoomRange=0,150 --batch=$isBatch 

#python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="gammaForwardPlus_mult.pdf"          --map=gammaForwardPlus          --varType=$varType --func='(x*[0]+x**2*[1]+x**3*[2])' --fitRange=0,12   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,15 --batch=$isBatch 

#python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="gammaForwardMinus_mult.pdf"          --map=gammaForwardMinus          --varType=$varType --func='(x*[0]+x**2*[1]+x**3*[2])' --fitRange=0,12   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,15 --batch=$isBatch 

python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="e_mult.pdf"          --map=e          --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,50   --rebin=5 --yZoomRange=-1,1 --xZoomRange=0,50 --batch=$isBatch 

python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="mu_mult.pdf"          --map=mu          --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,50   --rebin=5 --yZoomRange=-1,1 --xZoomRange=0,50 --batch=$isBatch 

python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="hHFPlus_mult.pdf"          --map=hHFPlus          --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,400   --rebin=0 --yZoomRange=-1,4 --xZoomRange=0,400 --batch=$isBatch 

python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="hHFMinus_mult.pdf"          --map=hHFMinus          --varType=$varType --func='(x*[0]+x**2*[1])' --fitRange=0,400   --rebin=0 --yZoomRange=-1,4 --xZoomRange=0,400 --batch=$isBatch 

python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="egammaHFPlus_mult.pdf"          --map=egammaHFPlus          --varType=$varType --func='(x*[0]+x**2*[1]+x**3*[2])' --fitRange=0,80   --rebin=0 --yZoomRange=-0.5,4 --xZoomRange=0,100 --batch=$isBatch 

python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="egammaHFMinus_mult.pdf"          --map=egammaHFMinus          --varType=$varType --func='(x*[0]+x**2*[1]+x**3*[2])' --fitRange=0,80   --rebin=0 --yZoomRange=-2,2 --xZoomRange=0,100 --batch=$isBatch 


# _______ _                     _____                               _                
#|__   __| |                   |  __ \                             | |               
#   | |  | |__  _ __ ___  ___  | |__) |_ _ _ __ __ _ _ __ ___   ___| |_ ___ _ __ ___ 
#   | |  | '_ \| '__/ _ \/ _ \ |  ___/ _` | '__/ _` | '_ ` _ \ / _ \ __/ _ \ '__/ __|
#   | |  | | | | | |  __/  __/ | |  | (_| | | | (_| | | | | | |  __/ ||  __/ |  \__ \
#   |_|  |_| |_|_|  \___|\___| |_|   \__,_|_|  \__,_|_| |_| |_|\___|\__\___|_|  |___/
#                                                                                    


#
#python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="hEtaPlus_mult.pdf"          --map=hEtaPlus          --varType=$varType --func='([0]+x*[1]+x**2*[2])' --fitRange=0,400   --rebin=0 --yZoomRange=-2,4 --xZoomRange=0,600 --batch=$isBatch 
#
#python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="hEtaMinus_mult.pdf"          --map=hEtaMinus          --varType=$varType --func='([0]+x*[1]+x**2*[2])' --fitRange=0,400   --rebin=0 --yZoomRange=-8,4 --xZoomRange=0,600 --batch=$isBatch 
#
#python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="h0Barrel_mult.pdf"          --map=h0Barrel          --varType=$varType --func='([0]+x*[1]+x**2*[2]+x**3*[3])' --fitRange=0,80   --rebin=5 --yZoomRange=-2,2 --xZoomRange=0,120 --batch=$isBatch 
#
#python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="h0EndcapPlus_mult.pdf"          --map=h0EndcapPlus          --varType=$varType --func='([0]+x*[1]+x**2*[2])' --fitRange=0,100   --rebin=5 --yZoomRange=-2,4 --xZoomRange=0,120 --batch=$isBatch 
#
#python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="h0EndcapMinus_mult.pdf"          --map=h0EndcapMinus          --varType=$varType --func='([0]+x*[1]+x**2*[2]+x**3*[3])' --fitRange=0,100   --rebin=5 --yZoomRange=-0.5,4 --xZoomRange=0,120 --batch=$isBatch 
#
#python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="gammaBarrel_mult.pdf"          --map=gammaBarrel          --varType=$varType --func='([0]+x*[1]+x**2*[2])' --fitRange=0,300   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,300 --batch=$isBatch 
#
#python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="gammaEndcapPlus_mult.pdf"          --map=gammaEndcapPlus          --varType=$varType --func='([0]+x*[1]+x**2*[2]+x**3*[3])' --fitRange=0,140   --rebin=0 --yZoomRange=-2.5,2.5 --xZoomRange=0,150 --batch=$isBatch 
#
#python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="gammaEndcapMinus_mult.pdf"          --map=gammaEndcapMinus          --varType=$varType --func='([0]+x*[1]+x**2*[2]+x**3*[3])' --fitRange=0,140   --rebin=0 --yZoomRange=-2.5,2.5 --xZoomRange=0,150 --batch=$isBatch 
#
##python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="gammaForwardPlus_mult.pdf"          --map=gammaForwardPlus          --varType=$varType --func='([0]+x*[1]+x**2*[2]+x**3*[3])' --fitRange=0,12   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,15 --batch=$isBatch 
#
##python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="gammaForwardMinus_mult.pdf"          --map=gammaForwardMinus          --varType=$varType --func='([0]+x*[1]+x**2*[2]+x**3*[3])' --fitRange=0,12   --rebin=0 --yZoomRange=-4,4 --xZoomRange=0,15 --batch=$isBatch 
#
##python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="e_mult.pdf"          --map=e          --varType=$varType --func='([0]+x*[1]+x**2*[2])' --fitRange=0,50   --rebin=5 --yZoomRange=-1,1 --xZoomRange=0,50 --batch=$isBatch 
#
##python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="mu_mult.pdf"          --map=mu          --varType=$varType --func='([0]+x*[1]+x**2*[2])' --fitRange=0,50   --rebin=5 --yZoomRange=-1,1 --xZoomRange=0,50 --batch=$isBatch 
#
#python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="hHFPlus_mult.pdf"          --map=hHFPlus          --varType=$varType --func='([0]+x*[1]+x**2*[2])' --fitRange=0,400   --rebin=0 --yZoomRange=-1,4 --xZoomRange=0,400 --batch=$isBatch 
#
#python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="hHFMinus_mult.pdf"          --map=hHFMinus          --varType=$varType --func='([0]+x*[1]+x**2*[2])' --fitRange=0,400   --rebin=0 --yZoomRange=-1,4 --xZoomRange=0,400 --batch=$isBatch 
#
#python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="egammaHFPlus_mult.pdf"          --map=egammaHFPlus          --varType=$varType --func='([0]+x*[1]+x**2*[2]+x**3*[3])' --fitRange=0,80   --rebin=0 --yZoomRange=-0.5,4 --xZoomRange=0,100 --batch=$isBatch 
#
#python profileFitPlots.py  --scriptFileName=$outputScriptFile --input=$inputData --rootGDir=metPhiCorrInfoWriter --plotoutPutDir=$ResultDir --plotFileName="egammaHFMinus_mult.pdf"          --map=egammaHFMinus          --varType=$varType --func='([0]+x*[1]+x**2*[2]+x**3*[3])' --fitRange=0,80   --rebin=0 --yZoomRange=-2,2 --xZoomRange=0,100 --batch=$isBatch 

fi

echo ")">>$ResultDir/$outputScriptFile
echo "Outputs written to: $ResultDir/$outputScriptFile"

