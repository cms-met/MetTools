from optparse import OptionParser
import time
parser = OptionParser()
parser.add_option("--input", dest="input", default='', type="string", action="store", help="input pickle file created by findMyRelvalFilesFromEOS.py")
parser.add_option("--run", dest="run", default=-1, type="int", action="store", help="restrict to run?")
parser.add_option("--tmpDir", dest="tmpDir", default='/tmp/', type="string", action="store", help="temporary directory")
parser.add_option("--release", dest="release", default='', type="string", action="store", help="override release (if it is not in the dict key)")
parser.add_option("--maxEvents", dest="maxEvents", default=-1, type="int", action="store", help="how many events (max)")
parser.add_option("--pattern", dest="pattern", default='', type="string", action="store", help="pattern to define a subset")
parser.add_option("--p", dest="pretend", action="store_true", help="just pretend")

(options, args) = parser.parse_args()

import ROOT
from DataFormats.FWLite import Events, Handle
from PhysicsTools.PythonAnalysis import *
from math import *
import sys, os, copy, random, subprocess, datetime
import pickle

#fileNames = pickle.load(file('relValFiles_73X.pkl'))
assert options.input!='', "Need to specify input file created with findMyRelvalsFromEOS.py. Syntax: --input=fileName.pkl"
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
  ofile= k.replace('/eos/cms/store/','').replace('/','_').replace('~','')
  if options.run>0:
    ofile+="_run"+str(options.run)
  fname = '/tmp/relValData_'+ofile+'.zpkl'
  if os.path.isfile(fname):
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
    print "Release: %s Architecture: %s tmpDir: %s RelVal: %s"%(release, arch, options.tmpDir, k)
  else:
    print "Now doing relval:",k,"in release:",release
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
    opts = ['--inputFiles='+','.join([dressFileName(x) for x in allRelVals[k]]), '--outputFile='+fname, '--run='+str(options.run), '--maxEvents='+str(options.maxEvents)]
    if 'miniaod' in k.lower():
      opts.append('--miniAOD')
    print 'python $chm/writeEventInformation.py '+' '.join(opts)
    sfile.write('python $chm/writeEventInformation.py '+' '.join(opts)+'\n')
    sfile.close()
    time.sleep(1)
    os.system('chmod +x '+execFileName+';sh '+execFileName+';rm '+execFileName)
