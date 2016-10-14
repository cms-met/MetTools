from CRABClient.UserUtilities import config , getUsernameFromSiteDB
config = config()

config.General.requestName = 'METPhiCorr_MC_DYJets'
#config.General.requestName = 'METPhiCorr_MC_TTJets'
#config.General.requestName = 'METPhiCorr_MC_WJetsToLNu'
config.General.workArea = 'crab_projects_MetPhiCorr_mc'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../run_metPhiCorr_mc.py'

config.Data.inputDataset = '/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'
#config.Data.inputDataset = '/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'
#config.Data.inputDataset = '/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
#config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 1
config.Data.outLFNDirBase = '/store/user/spak/MetPhiCorrection/MC/DYJets/'
config.Data.publication = False

config.Site.storageSite = 'T2_KR_KNU'
