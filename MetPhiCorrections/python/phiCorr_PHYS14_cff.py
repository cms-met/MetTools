import FWCore.ParameterSet.Config as cms

from MetTools.MetPhiCorrections.phiCorrBins_PHYS14_cfi import phiCorrBins_PHYS14 as phiCorrBins

##____________________________________________________________________________||

metPhiCorrInfoWriter = cms.EDAnalyzer("metPhiCorrInfoWriter",
    vertexCollection = cms.untracked.InputTag('offlinePrimaryVertices'),
    srcPFlow = cms.untracked.InputTag('particleFlow', ''),
    parameters = phiCorrBins
)
metPhiCorrInfoWriterSequence = cms.Sequence( metPhiCorrInfoWriter )
