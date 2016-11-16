#./fits.sh SampleTag input.root Period [ngoodVertices, sumPt, multiplicity]

batch=True

Dir="Summer16"
ParaVar="ngoodVertices"

###############
# Zmumu filter
###############
#./fits.sh DY /u/user/sangilpark/WorkDir/MetPhiCorrection/CMSSW_8_0_9/src/MetTools/MetPhiCorrections/test/histos/histo_mc_DYJetsToLL-M50.root $Dir $ParaVar $batch
#./fits.sh WJets /u/user/sangilpark/WorkDir/MetPhiCorrection/CMSSW_8_0_9/src/MetTools/MetPhiCorrections/test/histos/histo_mc_WJetsToLNu_noFilter.root $Dir $ParaVar $batch
#./fits.sh TTJets /u/user/sangilpark/WorkDir/MetPhiCorrection/CMSSW_8_0_9/src/MetTools/MetPhiCorrections/test/histos/histo_mc_TTJets_noFilter.root $Dir $ParaVar $batch
#./fits.sh Run2016CDEFG /u/user/sangilpark/WorkDir/MetPhiCorrection/CMSSW_8_0_9/src/MetTools/MetPhiCorrections/test/histos/histo_data_Run2016CDEFG.root $Dir $ParaVar $batch


###############
# No filter
###############
#./fits.sh DY /u/user/sangilpark/WorkDir/MetPhiCorrection/CMSSW_8_0_9/src/MetTools/MetPhiCorrections/test/histos/histo_mc_DYJetsToLL-M50_noFilter.root $Dir $ParaVar $batch
#./fits.sh WJets /u/user/sangilpark/WorkDir/MetPhiCorrection/CMSSW_8_0_9/src/MetTools/MetPhiCorrections/test/histos/histo_mc_WJetsToLNu_noFilter.root $Dir $ParaVar $batch
#./fits.sh TTJets /u/user/sangilpark/WorkDir/MetPhiCorrection/CMSSW_8_0_9/src/MetTools/MetPhiCorrections/test/histos/histo_mc_TTJets_noFilter.root $Dir $ParaVar $batch
#./fits.sh Run2016CDEFG /u/user/sangilpark/WorkDir/MetPhiCorrection/CMSSW_8_0_9/src/MetTools/MetPhiCorrections/test/histos/histo_data_Run2016CDEFG_noFilter.root $Dir $ParaVar  $batch


rm -rf plotAll 
rm -rf plotAll_${Dir}_${ParaVar}_Zmumu
mkdir plotAll
## plotAll.py --plotDir=Period --mode=[ngoodVertices, sumPt, multiplicity]
python plotAll.py --plotDir=$Dir --mode=$ParaVar
mv plotAll plotAll_${Dir}_${ParaVar}_Zmumu
tar cvf plotAll_${Dir}_${ParaVar}_Zmumu.tar plotAll_${Dir}_${ParaVar}_Zmumu
scp plotAll_${Dir}_${ParaVar}_Zmumu.tar lxplus.cern.ch:www/JetMET
