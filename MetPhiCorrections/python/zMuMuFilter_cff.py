import FWCore.ParameterSet.Config as cms

zMuMuFilter = cms.EDFilter("ZMuMuFilter",
   MuonLabel = cms.InputTag("slimmedMuons"),
   MetLabel = cms.InputTag("slimmedMETs"),
)

