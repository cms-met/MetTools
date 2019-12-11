from CRABClient.UserUtilities import config , getUsernameFromSiteDB
config = config()

config.General.requestName = 'METPhiCorr_analysis_Data2018B_102_v3'
config.General.workArea = 'crab_projects_2018'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'run_metPhiCorr_2018.py'

config.Data.inputDataset = '/DoubleMuon/Run2018B-17Sep2018-v1/MINIAOD'
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 80
config.Data.lumiMask = '/pnfs/iihe/cms/store/user/npostiau/json_2018.json'
config.Data.outLFNDirBase = '/store/user/npostiau/METPhi_2018'
config.Data.publication = True
config.Data.outputDatasetTag = 'CRAB3_Data2018B_102X_v3'

config.Site.storageSite = 'T2_BE_IIHE'
