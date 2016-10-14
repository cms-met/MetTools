from CRABClient.UserUtilities import config , getUsernameFromSiteDB
config = config()

config.General.requestName = 'METPhiCorr_Data_Run2016D'
config.General.workArea = 'crab_projects_MetPhiCorr_data'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../run_metPhiCorr_data.py'
config.JobType.maxJobRuntimeMin = 2750
config.JobType.outputFiles      = ['histo_data_Run2016D.root']


config.Data.inputDataset = '/DoubleMuon/Run2016D-PromptReco-v2/MINIAOD'
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 10
config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/Cert_271036-277933_13TeV_PromptReco_Collisions16_JSON.txt'
#config.Data.lumiMask = './json/Cert_271036-277933_13TeV_PromptReco_Collisions16_JSON.txt'
config.Data.outLFNDirBase = '/store/user/spak/MetPhiCorrection/Data/Run2016D/'
config.Data.publication = False

config.Site.storageSite = 'T2_KR_KNU'
