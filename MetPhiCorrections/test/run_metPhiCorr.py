import FWCore.ParameterSet.Config as cms
process = cms.Process("phiCorr")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(3000) )
process.source = cms.Source(
    'PoolSource',
# CSA14 RECO
#    fileNames = cms.untracked.vstring('root://eoscms.cern.ch//eos/cms/store/relval/CMSSW_7_0_5/RelValTTbar_13/GEN-SIM-RECO/POSTLS170_V6-v3/00000/0423767B-B5DD-E311-A1E0-02163E00E5B5.root') 
# 720 RECO
    fileNames = cms.untracked.vstring('root://eoscms.cern.ch//eos/cms/store/relval/CMSSW_7_2_0/RelValTTbar/GEN-SIM-DIGI-RECO/PU_PRE_STA72_V6_FastSim-v2/00000/FE55222C-7359-E411-A563-0025905A60CE.root')
# PHYS14 mAOD
    )

process.TFileService = cms.Service("TFileService", fileName = cms.string("histo.root") ,
      closeFileFast = cms.untracked.bool(True))

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'POSTLS170_V6::All'
process.load('MetTools.MetPhiCorrections.phiCorr_PHYS14_cff')

#
# RUN!
#
process.run = cms.Path(
  process.metPhiCorrInfoWriterSequence
)


