import sys, re, os, os.path

if __name__ == '__main__':
    print 'haha'

    path_dir = '/d1/scratch/Garden1/Data/data2016/RunIISummer16MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/'
    file_list = os.listdir(path_dir)
    file_list.sort()

    #file_count = len(os.walk(path_dir).next()[2])
    file_count = 1

    for item in file_list:
	if item.find('root') is not -1:
	    cfgfile = open('run_metPhiCorr_filterMC'+str(file_count)+'.py','w')
	    print item
	    cfgfile.write('import FWCore.ParameterSet.Config as cms\n')
	    cfgfile.write('process = cms.Process("phiCorr")\n')
	    cfgfile.write('\n')
	    cfgfile.write('process.load("FWCore.MessageService.MessageLogger_cfi")\n')
	    cfgfile.write('process.MessageLogger.cerr.FwkReport.reportEvery = 5000\n')
	    cfgfile.write('process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )\n')
	    cfgfile.write('process.source = cms.Source(\n')
	    cfgfile.write('    \'PoolSource\',\n')
	    cfgfile.write('    fileNames = cms.untracked.vstring(\'file:'+path_dir+'/'+item+'\') \n')
	    cfgfile.write('    ) \n')
	    cfgfile.write('\n')
	    cfgfile.write('process.TFileService = cms.Service("TFileService", fileName = cms.string("histoMC'+str(file_count)+'.root") ,\n')
	    cfgfile.write('      closeFileFast = cms.untracked.bool(True))\n')
	    cfgfile.write('\n')
	    #cfgfile.write('json = \'Cert_271036-284044_13TeV_PromptReco_Collisions16_JSON.txt\'\n')
	    #cfgfile.write('\n')
	    #cfgfile.write('if json != None:\n')
	    #cfgfile.write('  import os\n')
	    #cfgfile.write('  from FWCore.PythonUtilities.LumiList import LumiList\n')
	    #cfgfile.write('  import FWCore.ParameterSet.Types as CfgTypes\n')
	    #cfgfile.write('  myLumis = LumiList(filename = os.getenv(\'CMSSW_BASE\')+\'/src/MetTools/MetPhiCorrections/test/%s\'%json).getCMSSWString().split(\',\')\n')
	    #cfgfile.write('  process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())\n')
	    #cfgfile.write('  process.source.lumisToProcess.extend(myLumis)\n')
	    cfgfile.write('\n')
	    cfgfile.write('process.load(\'Configuration.StandardSequences.Services_cff\')\n')
	    cfgfile.write('process.load(\'Configuration.StandardSequences.FrontierConditions_GlobalTag_cff\')\n')
	    cfgfile.write('process.GlobalTag.globaltag = \'80X_mcRun2_asymptotic_2016_TrancheIV_v7\'\n')
	    cfgfile.write('process.load(\'MetTools.MetPhiCorrections.MuFilter_cff\')\n')
	    cfgfile.write('process.load(\'MetTools.MetPhiCorrections.phiCorrBins_Test_cff\')\n')
	    cfgfile.write('process.metPhiCorrInfoWriter.vertexCollection = cms.untracked.InputTag("offlineSlimmedPrimaryVertices")\n')
	    cfgfile.write('process.metPhiCorrInfoWriter.srcPFlow = cms.untracked.InputTag("packedPFCandidates")\n')
	    cfgfile.write('process.metPhiCorrInfoWriter.srcMet = cms.InputTag(\'slimmedMETs\')\n')
	    cfgfile.write('process.metPhiCorrInfoWriter.MuonLabel = cms.InputTag(\'slimmedMuons\')\n')
	    cfgfile.write('process.run = cms.Path(\n')
	    cfgfile.write('    process.MuFilter*\n')
	    cfgfile.write('    process.metPhiCorrInfoWriterSequence\n')
	    cfgfile.write(')\n')
	    
	    shfile = open('cmdBatch'+str(file_count)+'.sh','w')
	    shfile.write('#!/bin/bash\n')
	    shfile.write('#$ -N mkShapes__Met_Feb2017summer16_MC12_ha_bSF_l2tight__met_13TeV_of__WW\n')
	    shfile.write('export X509_USER_PROXY=/u/user/salee/.proxy\n')
	    shfile.write('export SCRAM_ARCH=slc6_amd64_gcc530\n')
	    shfile.write('source $VO_CMS_SW_DIR/cmsset_default.sh\n')
	    shfile.write('cd /u/user/salee/JetMet/CMSSW8026MEtXY/\n')
	    shfile.write('eval `scramv1 ru -sh`\n')
	    shfile.write('cd /u/user/salee/JetMet/CMSSW8026MEtXY/src/MetTools/MetPhiCorrections/test/JobsMC\n')
	    shfile.write('cmsRun run_metPhiCorr_filterMC'+str(file_count)+'.py\n')
	    os.system('chmod u+x cmdBatch'+str(file_count)+'.sh')
	    
	    file_count=file_count+1


