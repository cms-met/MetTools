import FWCore.ParameterSet.Config as cms

MuFilter = cms.EDFilter("MuFilter",
   MuonLabel = cms.InputTag("slimmedMuons"),
   MetLabel = cms.InputTag("slimmedMETs"),
   vtxTag = cms.InputTag("offlineSlimmedPrimaryVertices"),
)

