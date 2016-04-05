import FWCore.ParameterSet.Config as cms
metstrings = []
postfixes = [ "Cov00", "Cov11", "Phi", "dPhi", "Pt", "sumEt", "sumEtFraction", "PerpZ", "LongZ"]
for met in ["patpfMET", "patpfMETT1", "patpfTrackMET", "patpfTrackMETT1", "patpfNoPUMET", "patpfNoPUMETT1", "patpfPUCorrectedMET", "patpfPUCorrectedMETT1", "patpfPUMET", "patpfPUMETT1", "patpfChargedPUMET", "patpfNeutralPUMET", "patpfNeutralPVMET", "patpfNeutralUnclusteredMET", "slimmedMETs", "slimmedMETsPuppi"]:
  for postfix in postfixes:
    metstrings.append("recoil" + met + "_" + postfix)

metstrings.extend([ "dmvamet_Pt", "dmvamet_Phi", "dpfmet_Pt", "dpfmet_Phi", "MVAMET_Pt", "MVAMET_sumEt", "MVAMET_Phi", "PhiTrainingResponse", "RecoilTrainingResponse",
                 "genMet_Pt",
                 "genMet_Phi",
                 "weight",
                 "select",
                 "Jet0_Eta",
                 "Jet0_M",
                 "Jet0_Phi",
                 "Jet0_Pt",
                 "Jet1_Eta",
                 "Jet1_M",
                 "Jet1_Phi",
                 "Jet1_Pt",
                 "NCleanedJets",
                 "NVertex",
                 "Boson_Pt",
                 "Boson_Phi",
                 "Boson_M",
                 "Boson_Eta",
                 "Boson_sumET",
                 "Boson_daughter1",
                 "Boson_daughter2",
                 "nCombinations"
])

MAPAnalyzer =cms.EDAnalyzer('MAPAnalyzer',
                           srcVariables = cms.InputTag("MVAMET"),
                           srcVariableNames = cms.InputTag("MVAMET"),
                           variableNamesToSave = cms.vstring( metstrings ),
                           srcGenEvent = cms.InputTag("generator")
                           )
