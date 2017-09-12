import sys, re, os, os.path

if __name__ == '__main__':
    print 'haha'

    rootFile_dir = '/d1/scratch/Garden1/Data/data2016/RunIISummer16MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/'
    file_list = os.listdir(rootFile_dir)
    file_list.sort()

    #file_count = len(os.walk(rootFile_dir).next()[2])
    file_count = 1

    for item in file_list:
	if item.find('root') is not -1:
	    cfgfile = open('run_metPhiCorr_filterMC'+str(file_count)+'.py','w')
	    print 'for file:',item
	    fTemp = open("template_run_metPhiCorr_filter_MC.py",'r')
	    for line in fTemp:
	      if 'fileNames' in line:
	        cfgfile.write('    fileNames = cms.untracked.vstring(\'file:'+rootFile_dir+'/'+item+'\') \n')
	      elif 'TFileService' in line:
	        cfgfile.write('process.TFileService = cms.Service("TFileService", fileName = cms.string("histoMC'+str(file_count)+'.root") ,\n')
	      else:
		cfgfile.write(line)
	    
	    shfile = open('cmdBatch'+str(file_count)+'.sh','w')
	    cmdTemp = open("template_cmd.py",'r')
	    for line in cmdTemp:
	      if 'cmsRun' in line:
	        shfile.write('cmsRun run_metPhiCorr_filterMC'+str(file_count)+'.py\n')
	      else:
		shfile.write(line)

	    os.system('chmod u+x cmdBatch'+str(file_count)+'.sh')
	    
	    file_count=file_count+1


