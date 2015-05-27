from optparse import OptionParser
import time
parser = OptionParser()
parser.add_option("--input", dest="input", default='', type="string", action="store", help="input pickle file created by findMyRelvalFilesFromEOS.py")
parser.add_option("--pattern", dest="pattern", default='', type="string", action="store", help="pattern to define a subset of the keys in the input file")
parser.add_option("--inputDir", dest="inputDir", default='', type="string", action="store", help="use input directory instead of input file and pattern")
parser.add_option("--outputFile", dest="outputFile", default='', type="string", action="store", help="use this outputFile instead of /tmp/<defaultFileName>")
parser.add_option("--run", dest="run", default=-1, type="int", action="store", help="restrict to run?")
parser.add_option("--tmpDir", dest="tmpDir", default='/tmp/', type="string", action="store", help="temporary directory")
parser.add_option("--release", dest="release", default='', type="string", action="store", help="override release (if it is not in the dict key)")
parser.add_option("--maxEvents", dest="maxEvents", default=-1, type="int", action="store", help="how many events (max)")
parser.add_option("--p", dest="pretend", action="store_true", help="just pretend")
parser.add_option("--overWrite", dest="overWrite", action="store_true", default=False, help="overwrite output file?")
parser.add_option("--miniAOD", dest="miniAOD", action="store_false", help="If set, assume miniAOD. Otherwise determine from inputDir or key.")

(options, args) = parser.parse_args()

import ROOT
from DataFormats.FWLite import Events, Handle
from PhysicsTools.PythonAnalysis import *
from math import *
import sys, os, copy, random, subprocess, datetime
import pickle

if options.inputDir!='':
  from helpers import getContent
  if options.inputDir.startswith('/eos/cms/'):
    print "Searching EOS: %s"%options.inputDir
    fileList = getContent(options.inputDir, eos=True)
  else:
    print "Searching local file system: %s"%options.inputDir
    fileList = getContent(options.inputDir)
  assert options.release!='', "Need to specify release when --inputDir is set!"
  key =  options.inputDir.replace('~','').rstrip('/').lstrip('/').replace('/','_')
  allRelVals = {key:[x for x in fileList if '.root' in x] }
  print "Using directory %s, found %i files"%(options.inputDir,len(fileList))
else: 
  assert options.input!='', "inputDir not specified: instead specify input file created with findMyRelvalsFromEOS.py. Syntax: --input=fileName.pkl"
  allRelVals=pickle.load(file(options.input))

small = False
maxEvts=-1

from helpers import architectures
archDict = architectures()

print

def dressFileName(x, prefix='root://eoscms.cern.ch/'):
  if x.startswith('root://'):
    return x
  else: 
    return prefix+x.replace('~','')

for k in allRelVals.keys():
  if options.pattern not in k:continue
  os.system('mkdir -p '+options.tmpDir)
  if options.outputFile!='':
    fname=options.outputFile
  else:
    ofile= k.replace('/eos/cms/store/','').replace('/','_').replace('~','')
    if options.run>0:
      ofile+="_run"+str(options.run)
    fname = '/tmp/relValData_'+ofile+'.root'
  if os.path.isfile(fname):
    if options.overWrite==True:
      print "Overwriting",fname
    else:
      print "Found",fname,"->skipping!"
      continue
  if options.release!='':
    release=options.release
  else:
    try:
      release = filter(lambda x:x.startswith('CMSSW_'), k.split('/'))[0]
    except:
      print "Could not determine release from key",k
#  assert archDict.has_key('release'), "No architecture found for release %s"%release
  try:
    arch = archDict[release]
  except:
    arch = "slc6_amd64_gcc481"
    print "No information on release %s using default %s"%(release,arch)

  if options.pretend: 
    print "Pretending for: release %s Architecture %s tmpDir %s key %s"%(release, arch, options.tmpDir, k)
  else:
    print "Now doing:",k,"in release:",release,"arch",arch
    execFileName = options.tmpDir+'/exec.sh'
    sfile = file(execFileName,'w')
    sfile.write('#!/bin/sh\n')
    sfile.write("export chm=$PWD\n")
    sfile.write("export SCRAM_ARCH="+arch+"\n")
  #  sfile.write('source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.sh\n')
    sfile.write("cd "+options.tmpDir+'\n')
    sfile.write("rm -rf "+release+'\n')
    sfile.write("scramv1 project CMSSW "+release+'\n')
    sfile.write('cd '+release+'/src\n')
    sfile.write('eval `scramv1 runtime -sh`\n')
    assert len(allRelVals[k])>0, "Empty file list! Check access rights."
    opts = ['--inputFiles='+','.join([dressFileName(x) for x in allRelVals[k]]), '--outputFile='+fname, '--run='+str(options.run), '--maxEvents='+str(options.maxEvents)]
    if 'miniaod' in k.lower() or options.miniAOD:
      opts.append('--miniAOD')
    print 'python $chm/writeEventInformation.py '+' '.join(opts)
    sfile.write('python $chm/writeEventInformation.py '+' '.join(opts)+'\n')
    sfile.close()
    time.sleep(1)
    os.system('chmod +x '+execFileName+';sh '+execFileName+';rm '+execFileName)
