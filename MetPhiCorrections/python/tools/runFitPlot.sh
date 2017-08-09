#./fits.sh Era SampleTag varType[multiplicity,ngoodVertices,sumPt,pfType1,ptcMet,ptValence, metPhiNvtx] Input isBatch

Era=03Feb2017_V1
#varType=ptValence
#varType=ptcMet
#varType=metPhiNvtx
varType=pfType1
batch=True # background job not real batch

###############
# filter
###############
./fits.sh ${Era} Summer16DY ${varType} /u/user/salee/JetMet/CMSSW8026MEtXY/src/MetTools/MetPhiCorrections/test/JobsMC/histoMC1.root $batch
#./fits.sh ${Era} Run2016B ${varType} /u/user/salee/JetMet/CMSSW8026MEtXY/src/MetTools/MetPhiCorrections/test/SingleMu_MuFilter_MetNvtx.root $batch


###############
# No filter
###############
#./fits.sh DY /u/user/sangilpark/WorkDir/MetPhiCorrection/CMSSW_8_0_9/src/MetTools/MetPhiCorrections/test/histos/histo_mc_DYJetsToLL-M50_noFilter.root $Dir $varType $batch
#./fits.sh WJets /u/user/sangilpark/WorkDir/MetPhiCorrection/CMSSW_8_0_9/src/MetTools/MetPhiCorrections/test/histos/histo_mc_WJetsToLNu_noFilter.root $Dir $varType $batch
#./fits.sh TTJets /u/user/sangilpark/WorkDir/MetPhiCorrection/CMSSW_8_0_9/src/MetTools/MetPhiCorrections/test/histos/histo_mc_TTJets_noFilter.root $Dir $varType $batch
#./fits.sh Run2016CDEFG /u/user/sangilpark/WorkDir/MetPhiCorrection/CMSSW_8_0_9/src/MetTools/MetPhiCorrections/test/histos/histo_data_Run2016CDEFG_noFilter.root $Dir $varType  $batch


#rm -rf plotAll 
#rm -rf plotAll_${Dir}_${varType}_Zmumu
#mkdir plotAll
### plotAll.py --plotDir=Period --mode=[ngoodVertices, sumPt, multiplicity]
#python plotAll.py --plotDir=$Dir --mode=$varType
#mv plotAll plotAll_${Dir}_${varType}_Zmumu
#tar cvf plotAll_${Dir}_${varType}_Zmumu.tar plotAll_${Dir}_${varType}_Zmumu
#scp plotAll_${Dir}_${varType}_Zmumu.tar lxplus.cern.ch:www/JetMET
