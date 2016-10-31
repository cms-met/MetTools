#./fits.sh SampleTag input.root Period [ngoodVertices, sumPt, multiplicity]

batch=True

./fits.sh DY /u/user/sangilpark/WorkDir/MetPhiCorrection/CMSSW_8_0_9/src/MetTools/MetPhiCorrections/test/histos/histo_mc_DYJetsToLL-M50_noFilter.root Summer16 sumPt $batch
./fits.sh WJetsToLNu /u/user/sangilpark/WorkDir/MetPhiCorrection/CMSSW_8_0_9/src/MetTools/MetPhiCorrections/test/histos/histo_mc_WJetsToLNu_noFilter.root Summer16 sumPt $batch
./fits.sh TTJets /u/user/sangilpark/WorkDir/MetPhiCorrection/CMSSW_8_0_9/src/MetTools/MetPhiCorrections/test/histos/histo_mc_TTJets_noFilter.root Summer16 sumPt $batch
./fits.sh Run2016CDEFG /u/user/sangilpark/WorkDir/MetPhiCorrection/CMSSW_8_0_9/src/MetTools/MetPhiCorrections/test/histos/histo_data_Run2016CDEFG_noFilter.root Summer16 sumPt $batch

rm -rf plotAll 
rm -rf plotAll_Summer16_sumPt
mkdir plotAll
# plotAll.py --plotDir=Period --mode=[ngoodVertices, sumPt, multiplicity]
python plotAll.py --plotDir="Summer16" --mode="sumPt"
mv plotAll plotAll_Summer16_sumPt
tar cvf plotAll_Summer16_sumPt.tar plotAll_Summer16_sumPt
scp plotAll_Summer16_sumPt.tar lxplus.cern.ch:www/JetMET
