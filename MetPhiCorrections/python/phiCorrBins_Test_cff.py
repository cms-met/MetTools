import FWCore.ParameterSet.Config as cms

from MetTools.MetPhiCorrections.phiCorrBins_Test_cfi import phiCorrBins_Test as phiCorrBins

metPhiCorrInfoWriter = cms.EDAnalyzer("metPhiCorrInfoWriter",
        MuonLabel = cms.InputTag("slimmedMuons"),
        srcMet = cms.InputTag('slimmedMETs'),
	jetTag = cms.InputTag("slimmedJets"),
	vertexCollection = cms.untracked.InputTag("offlineSlimmedPrimaryVertices"),
	#vertexCollection = cms.untracked.InputTag("offlinePrimaryVertices"),
	srcPFlow = cms.untracked.InputTag("packedPFCandidates"),
	#srcPFlow = cms.untracked.InputTag("particleFlow", ""),
	parameters = phiCorrBins
)

metPhiCorrInfoWriterSequence = cms.Sequence( metPhiCorrInfoWriter )

