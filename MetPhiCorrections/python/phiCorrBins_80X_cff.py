import FWCore.ParameterSet.Config as cms

from MetTools.MetPhiCorrections.phiCorrBins_80X_cfi import phiCorrBins_80X as phiCorrBins

metPhiCorrInfoWriter = cms.EDAnalyzer("metPhiCorrInfoWriter",
	vertexCollection = cms.untracked.InputTag("offlinePrimaryVertices"),
	srcPFlow = cms.untracked.InputTag("particleFlow", ""),
	parameters = phiCorrBins
)

metPhiCorrInfoWriterSequence = cms.Sequence( metPhiCorrInfoWriter )

