import FWCore.ParameterSet.Config as cms
process = cms.Process("phiCorr")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.source = cms.Source(
    'PoolSource',
#    fileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch//store/mc/RunIIAutumn18MiniAOD/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15_ext2-v1/50000/CFD5125B-9F57-C440-A34F-BE0018D7F594.root')
    fileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch//store/data/Run2016B/DoubleMuon/MINIAOD/17Jul2018_ver2-v1/00000/6CDED1D1-AA8B-E811-864A-003048FFD71C.root')
    )

process.TFileService = cms.Service("TFileService", fileName = cms.string("histo_MC2016.root") ,
      closeFileFast = cms.untracked.bool(True))

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = '102X_dataRun2_v10' #2018 data
#process.GlobalTag.globaltag = '102X_upgrade2018_realistic_v18' #2018 MC
#process.GlobalTag.globaltag = '102X_dataRun2_v8' #2017 data
#process.GlobalTag.globaltag = '102X_upgrade2018_realistic_v16' #2017 MC
process.GlobalTag.globaltag = '94X_dataRun2_v10' #2016 data
#process.GlobalTag.globaltag = '94X_mcRun2_asymptotic_v3' #2016 MC

process.load('MetTools.MetPhiCorrections.phiCorrBins_102X_cff')
#Replacements for mAOD
process.metPhiCorrInfoWriter.vertexCollection = cms.untracked.InputTag("offlineSlimmedPrimaryVertices")
process.metPhiCorrInfoWriter.muonsCollection = cms.untracked.InputTag("slimmedMuons")
process.metPhiCorrInfoWriter.srcPFlow = cms.untracked.InputTag("packedPFCandidates")

# RUN!
process.run = cms.Path(
  process.metPhiCorrInfoWriterSequence
)
