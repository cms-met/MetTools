#!/bin/bash
export X509_USER_PROXY=/u/user/salee/.proxy
export SCRAM_ARCH=slc6_amd64_gcc530
source $VO_CMS_SW_DIR/cmsset_default.sh
cd /u/user/salee/JetMet/CMSSW8026MEtXY/
eval `scramv1 ru -sh`
cd /u/user/salee/JetMet/CMSSW8026MEtXY/src/MetTools/MetPhiCorrections/test/JobsMC
cmsRun run_metPhiCorr_filterMC1.py
