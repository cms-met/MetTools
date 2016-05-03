from optparse import OptionParser
parser = OptionParser()
parser.add_option("--postfix", dest="postfix", default="76X", type="string", action="store", help="identifier (e.g. 76X)")
(options, args) = parser.parse_args()

fileName = "phiCorrBins_"+options.postfix+"_cff.py"
ofile = file(fileName, 'w')
ofile.write('import FWCore.ParameterSet.Config as cms\n')
ofile.write('\n')
ofile.write('from MetTools.MetPhiCorrections.phiCorrBins_'+options.postfix+'_cfi import phiCorrBins_'+options.postfix+' as phiCorrBins\n')
ofile.write('\n')
ofile.write('metPhiCorrInfoWriter = cms.EDAnalyzer("metPhiCorrInfoWriter",\n')
ofile.write('\tvertexCollection = cms.untracked.InputTag("offlinePrimaryVertices"),\n')
ofile.write('\tsrcPFlow = cms.untracked.InputTag("particleFlow", ''),\n')
ofile.write('\tparameters = phiCorrBins\n')
ofile.write(')\n')
ofile.write('\n')
ofile.write('metPhiCorrInfoWriterSequence = cms.Sequence( metPhiCorrInfoWriter )\n')
ofile.write('\n')
ofile.close()
print "Written", fileName

