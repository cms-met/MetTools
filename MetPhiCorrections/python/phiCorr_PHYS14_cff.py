import FWCore.ParameterSet.Config as cms

from MetTools.MetPhiCorrections.phiCorr_PHYS14_cfi import phiCorr_PHYS14 as phiCorr

##____________________________________________________________________________||

metPhiCorrInfoWriter = cms.EDAnalyzer("metPhiCorrInfoWriter",
    vertexCollection = cms.untracked.InputTag('offlinePrimaryVertices'),
    srcPFlow = cms.InputTag('particleFlow', ''),
    parameters = phiCorr
)
metPhiCorrInfoWriterSequence = cms.Sequence( metPhiCorrInfoWriter )
