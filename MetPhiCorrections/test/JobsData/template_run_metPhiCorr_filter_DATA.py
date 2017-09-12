import FWCore.ParameterSet.Config as cms
process = cms.Process("phiCorr")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring('file:/d1/scratch/Garden1/Data/data2016/Run2016B/SingleMuon/MINIAOD/03Feb2017_ver2-v2//000C6E52-8BEC-E611-B3FF-0025905C42FE.root') 
    ) 

process.TFileService = cms.Service("TFileService", fileName = cms.string("histo1.root") ,
      closeFileFast = cms.untracked.bool(True))

json = 'Cert_271036-284044_13TeV_PromptReco_Collisions16_JSON.txt'

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
process.load('MetTools.MetPhiCorrections.MuFilter_cff')
process.load('MetTools.MetPhiCorrections.phiCorrBins_Test_cff')
process.run = cms.Path(
    process.MuFilter*
    process.metPhiCorrInfoWriterSequence
)
