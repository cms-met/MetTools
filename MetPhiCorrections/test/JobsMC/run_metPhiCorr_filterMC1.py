import FWCore.ParameterSet.Config as cms
process = cms.Process("phiCorr")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring('file:/d1/scratch/Garden1/Data/data2016/RunIISummer16MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1//005ED0EB-79F1-E611-B6DA-02163E011C2B.root') 
    ) 

process.TFileService = cms.Service("TFileService", fileName = cms.string("histoMC1.root") ,
      closeFileFast = cms.untracked.bool(True))


process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_2016_TrancheIV_v7'
process.load('MetTools.MetPhiCorrections.MuFilter_cff')
process.load('MetTools.MetPhiCorrections.phiCorrBins_Test_cff')
process.metPhiCorrInfoWriter.vertexCollection = cms.untracked.InputTag("offlineSlimmedPrimaryVertices")
process.metPhiCorrInfoWriter.srcPFlow = cms.untracked.InputTag("packedPFCandidates")
process.metPhiCorrInfoWriter.srcMet = cms.InputTag('slimmedMETs')
process.metPhiCorrInfoWriter.MuonLabel = cms.InputTag('slimmedMuons')
process.run = cms.Path(
    process.MuFilter*
    process.metPhiCorrInfoWriterSequence
)
