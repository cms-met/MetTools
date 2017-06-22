import FWCore.ParameterSet.Config as cms
process = cms.Process("phiCorr")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.source = cms.Source(
    'PoolSource',
# CSA14 RECO
#    fileNames = cms.untracked.vstring('root://eoscms.cern.ch//eos/cms/store/relval/CMSSW_7_0_5/RelValTTbar_13/GEN-SIM-RECO/POSTLS170_V6-v3/00000/0423767B-B5DD-E311-A1E0-02163E00E5B5.root') 
# 720 RECO
#    fileNames = cms.untracked.vstring('root://eoscms.cern.ch//eos/cms/store/relval/CMSSW_7_2_0/RelValTTbar/GEN-SIM-DIGI-RECO/PU_PRE_STA72_V6_FastSim-v2/00000/FE55222C-7359-E411-A563-0025905A60CE.root')
# PHYS14 mAOD
#    fileNames = cms.untracked.vstring('root://xrootd.unl.edu//store/mc/Phys14DR/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU30bx50_PHYS14_25_V1-v1/00000/003B6371-8D81-E411-8467-003048F0E826.root')
# PHYS14 mAOD local
#    fileNames = cms.untracked.vstring('file:/data/schoef/local/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_MINIAODSIM_PU20bx25_PHYS14_25_V1-v1.root')
# 76X mAODv2
#    fileNames = cms.untracked.vstring('root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PU25nsData2015v1_HCALDebug_76X_mcRun2_asymptotic_v12-v1/00000/006C9F73-3FB9-E511-9AFE-001E67E95C52.root')
fileNames = cms.untracked.vstring('file:/d1/scratch/Garden1/Data/data2016/Run2016B/SingleMuon/MINIAOD/03Feb2017_ver2-v2/000C6E52-8BEC-E611-B3FF-0025905C42FE.root')
#fileNames = cms.untracked.vstring('file:file:/u/user/sangilpark/RunIISample/Run2016B/SingleMuon/MINIAOD/PromptReco-v2/34A57FB8-D819-E611-B0A4-02163E0144EE.root')
    #fileNames = cms.untracked.vstring('file:/u/user/salee/Data/ReValZMM_13/CMSSW_8_1_0_pre6-PU25ns_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM/A0DE71C7-D82C-E611-88FF-0025905B85AA.root')
    )

process.TFileService = cms.Service("TFileService", fileName = cms.string("histo.root") ,
      closeFileFast = cms.untracked.bool(True))

json = 'Cert_271036-284044_13TeV_PromptReco_Collisions16_JSON.txt'
#json = None
if json != None:
  import os
  from FWCore.PythonUtilities.LumiList import LumiList
  import FWCore.ParameterSet.Types as CfgTypes
  myLumis = LumiList(filename = os.getenv('CMSSW_BASE')+'/src/MetTools/MetPhiCorrections/test/%s'%json).getCMSSWString().split(',')
  process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
  process.source.lumisToProcess.extend(myLumis)

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = '80X_dataRun2_2016SeptRepro_v7'
#process.GlobalTag.globaltag = '80X_dataRun2_v17'
#process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_2016_miniAODv2_v1'
#process.GlobalTag.globaltag = '76X_mcRun2_asymptotic_v12'

process.load('MetTools.MetPhiCorrections.MuFilter_cff')

process.load('MetTools.MetPhiCorrections.phiCorrBins_Test_cff')
#Replacements for mAOD
process.metPhiCorrInfoWriter.vertexCollection = cms.untracked.InputTag("offlineSlimmedPrimaryVertices")
process.metPhiCorrInfoWriter.srcPFlow = cms.untracked.InputTag("packedPFCandidates")
process.metPhiCorrInfoWriter.srcMet = cms.InputTag('slimmedMETs')

# RUN!
process.run = cms.Path(
    process.MuFilter*
    process.metPhiCorrInfoWriterSequence
)
