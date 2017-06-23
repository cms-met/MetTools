#!/bin/bash
#$ -N mkShapes__Met_Feb2017summer16_MC12_ha_bSF_l2tight__met_13TeV_of__WW
export X509_USER_PROXY=/u/user/salee/.proxy
export SCRAM_ARCH=slc6_amd64_gcc530
source $VO_CMS_SW_DIR/cmsset_default.sh
cd /u/user/salee/JetMet/CMSSW8026MEtXY/
eval `scramv1 ru -sh`
cd /u/user/salee/JetMet/CMSSW8026MEtXY/src/MetTools/MetPhiCorrections/test 
cmsRun run_metPhiCorr_filter.py
