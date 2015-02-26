from MetTools.MetPhiCorrections.tools.categories import *

#def getShiftCorr(sample, mapName):
#  res = pickle.load(file('/data/schoef/tools/metPhiShifts/shift_'+sample+'_'+mapName+'.pkl'))
#  return res

from optparse import OptionParser
parser = OptionParser()
parser.add_option("--in", dest="infile",  default="", type="string", action="store", help="Inputfile. If none is given, will write output file for multMETCorrInfoWriter")
parser.add_option("--postfix", dest="postfix", default="PHYS14", type="string", action="store", help="identifier (e.g. PHYS14)")
(options, args) = parser.parse_args()

if options.infile=="":
  fileName = "phiCorrBins_"+options.postfix+"_cfi.py" 
  ofile = file(fileName, 'w')
  ofile.write('import FWCore.ParameterSet.Config as cms\n')
  ofile.write("phiCorrBins_"+options.postfix+' = cms.VPSet(\n')
  for v in varTypes:
    for map in allMaps:
      ofile.write('    cms.PSet(\n')
      ofile.write('      name=cms.string("'+map['name'].replace('_','')+'"),\n')
      ofile.write('      type=cms.int32('+str(label[map['type']])+'),\n')
      ofile.write('      varType=cms.int32('+str(varType[v])+'),\n')
      ofile.write('      nbins=cms.double('+str(map['binning'][v][0])+'),\n')
      ofile.write('      nMin=cms.int32('+str(map['binning'][v][1])+'),\n')
      ofile.write('      nMax=cms.int32('+str(map['binning'][v][2])+'),\n')
      ofile.write('      etaNBins=cms.int32('+str(map['etaPhiBinning'][0])+'),\n')
      ofile.write('      etaMin=cms.double('+str(map['etaPhiBinning'][1])+'),\n')
      ofile.write('      etaMax=cms.double('+str(map['etaPhiBinning'][2])+'),\n')
      ofile.write('      phiNBins=cms.int32('+str(map['etaPhiBinning'][3])+'),\n')
      ofile.write('      phiMin=cms.double('+str(map['etaPhiBinning'][4])+'),\n')
      ofile.write('      phiMax=cms.double('+str(map['etaPhiBinning'][5])+'),\n')
      ofile.write('    ),\n')
  ofile.write(')\n')
  ofile.close()
  print "Written", fileName
else:
  print "Got infile",options.infile,"--> not yet implemented to write correction cfg"
#name = 'phiCorrCorr_'+sample
#filename = name+'_cfi.py'
#ofile = file(filename, 'w')
#ofile.write('import FWCore.ParameterSet.Config as cms\n')
#ofile.write(name+' = cms.VPSet(\n')
#for map in allMaps:
#  res = getShiftCorr(sample, map['name'])
#  ofile.write('    cms.PSet(\n')
#  ofile.write('      name=cms.string("'+map['name'].replace('_','')+'"),\n')
#  ofile.write('      type=cms.int32('+str(label[map['type']])+'),\n')
#  ofile.write('      etaMin=cms.double('+str(map['binning'][1])+'),\n')
#  ofile.write('      etaMax=cms.double('+str(map['binning'][2])+'),\n')
#  ofile.write('      fx=cms.string("'+res['fx'].GetExpFormula().Data()+'"),\n')
#  ofile.write('      px=cms.vdouble('+','.join(str(res['fx'].GetParameter(i)) for i in range(res['fx'].GetNpar()))+'),\n')
#  ofile.write('      fy=cms.string("'+res['fy'].GetExpFormula().Data()+'"),\n')
#  ofile.write('      py=cms.vdouble('+','.join(str(res['fy'].GetParameter(i)) for i in range(res['fy'].GetNpar()))+'),\n')
#  
#  ofile.write('    ),\n')
#ofile.write(')\n')
#ofile.close()
