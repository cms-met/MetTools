from optparse import OptionParser
parser = OptionParser()
parser.add_option("--map", dest="map", default='h', type="string", action="store", help="Which map. Default:h (charged hadrons)")
parser.add_option("--func", dest="func", default='x*[0]+x**2*[1]', type="string", action="store", help="Which function to use. Default: x*[0]+x**2*[1]")
parser.add_option("--rebin", dest="rebin", default=0, type="int", action="store", help="Rebinning to performe on tProfiles")
parser.add_option("--fitRange", dest="fitRange", default='0,2000', type="string", action="store", help="Which fitRange. Default:0,2000")
parser.add_option("--xZoomRange", dest="xZoomRange", default='', type="string", action="store", help="Which xZoomRange. Default:Max")
parser.add_option("--yZoomRange", dest="yZoomRange", default='-40,40', type="string", action="store", help="Which zoom range in y Axis. Default:-40,40")
parser.add_option("--mode", dest="mode", default='sumPt', type="string", action="store", help="Which mode [ngoodVertices/sumPt/multiplicity]")
parser.add_option("--input", dest="input", default='DYJetsToLL_M-50_HT-100toInf_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_AODSIM.root', type="string", action="store", help="input file.Default:DYJetsToLL_M-50_HT-100toInf_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_AODSIM.root")
parser.add_option("--rootGDir", dest="rootGDir", default='metPhiCorrInfoWriter', type="string", action="store", help="Which gDir was used in the production of the MEx,y profile [metPhiCorrInfoWriter/pfMEtMultCorrInfoWriter].")
parser.add_option("--plotFileName", dest="plotFileName", default="plot.pdf", type="string", action="store", help="Filename the plot. Default:test.pdf")
parser.add_option("--textFileName", dest="textFileName", default="metPhiCorrections_cfi.py", type="string", action="store", help="Text file name that the corrections are appended to. Default:metPhiCorrections_cfi.py.")
(options, args) = parser.parse_args()

import ROOT
import pickle, os

from math import pi, cos, sin, sqrt, atan2
ROOT.gROOT.ProcessLine(".L $CMSSW_BASE/src/MetTools/Commons/scripts/tdrstyle.C")
ROOT.setTDRStyle()
from MetTools.MetPhiCorrections.tools.categories import *
from MetTools.MetPhiCorrections.tools.helpers import getObjFromFile


assert options.map in [m['name'].replace('HF','HF_') for m in allMaps],("map %s not in "+",".join([m['name'].replace('HF','HF_') for m in allMaps])) % options.map
exec("map = " +options.map)
exec("yZoomRange = (" +options.yZoomRange+ ")")
exec("fitRange = (" +options.fitRange+ ")")
if options.xZoomRange!='':
  exec("xZoomRange = (" +options.xZoomRange+ ")")
else:
  xZoomRange = None

assert options.mode in ["ngoodVertices","sumPt","multiplicity"],"Mode %s not known. Must be one of [ngoodVertices/sumPt/multiplicity]"%options.mode

print 'map', options.map, 'input', options.input, 'mode', options.mode, 'yZoomrange', yZoomRange, 'fitRange', fitRange, 'xZoomRange', xZoomRange

fx = ROOT.TF1('fx', options.func, *(fitRange))
fy = ROOT.TF1('fy', options.func, *(fitRange))
iname = options.rootGDir+'/'+options.rootGDir+'_'+options.mode+'_'+map['name'].replace('h_HF','hHF').replace('egamma_HF','egammaHF')
px = getObjFromFile(options.input, iname+'_Px')
py = getObjFromFile(options.input, iname+'_Py')

assert px and py, "Could not read %s{_Px,_Py} from input from file %s, maybe --rootGDir not set correctly (is %s)"%(iname, options.input, options.rootGDir)
if options.rebin:
  px.Rebin(options.rebin)
  py.Rebin(options.rebin)
px.Fit(fx,'R')
py.Fit(fy,'R')

c1 = ROOT.TCanvas()  
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(0)
px.Draw('h')
if options.mode=="multiplicity":
  px.GetXaxis().SetTitle("multiplicity in "+map['name'])
if options.mode=="sumPt":
  px.GetXaxis().SetTitle("#Sigma p_{T} of "+map['name'])
if options.mode=="ngoodVertices":
  px.GetXaxis().SetTitle("ngoodVertices")
px.GetYaxis().SetTitle("<#slash{E}_{x,y}> (GeV)")
px.GetXaxis().SetTitleSize(0.05)
px.GetXaxis().SetTitleOffset(1.1)
px.GetYaxis().SetRangeUser(*yZoomRange)
if xZoomRange:
  px.GetXaxis().SetRangeUser(*xZoomRange)
px.SetLineColor(ROOT.kBlue)
px.SetLineStyle(0)
px.SetLineWidth(2)
px.SetMarkerStyle(0)
px.SetMarkerSize(0)
#    py.GetYaxis().SetRangeUser(-20,20)
py.SetLineColor(ROOT.kRed)
py.SetLineStyle(0)
py.SetLineWidth(2)
py.SetMarkerStyle(0)
py.SetMarkerSize(0)
py.Draw('hsame')

l = ROOT.TLegend(0.55,0.83,.95,.95)
l.AddEntry(px, "< #slash{E}_{x} >")#+etab[0].split('_')[0]+", "+shortName[etab[0].split('_')[1]])
l.AddEntry(py, "< #slash{E}_{y} >")
l.SetFillColor(0)
l.SetShadowColor(ROOT.kWhite)
l.SetBorderSize(1)
l.Draw()
c1.Print(options.plotFileName)
c1.Print(options.plotFileName.replace('.pdf', '.root'))
c1.Print(options.plotFileName.replace('.pdf', '.png'))

with open(options.textFileName, "a") as ofile:
  ofile.write('    cms.PSet(\n')
  ofile.write('      name=cms.string("'+map['name'].replace('_','')+'"),\n')
  ofile.write('      type=cms.int32('+str(label[map['type']])+'),\n')
  ofile.write('      varType=cms.int32('+str(varType[options.mode])+'),\n')
  ofile.write('      etaMin=cms.double('+str(map['etaPhiBinning'][1])+'),\n')
  ofile.write('      etaMax=cms.double('+str(map['etaPhiBinning'][2])+'),\n')
  ofile.write('      fx=cms.string("'+fx.GetExpFormula().Data()+'"),\n')
  ofile.write('      px=cms.vdouble('+','.join(str(fx.GetParameter(i)) for i in range(fx.GetNpar()))+'),\n')
  ofile.write('      fy=cms.string("'+fy.GetExpFormula().Data()+'"),\n')
  ofile.write('      py=cms.vdouble('+','.join(str(fy.GetParameter(i)) for i in range(fy.GetNpar()))+'),\n')
  ofile.write('    ),\n')
