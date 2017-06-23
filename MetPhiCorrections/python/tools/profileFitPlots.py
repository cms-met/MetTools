from optparse import OptionParser
parser = OptionParser()
parser.add_option("--map", dest="map", default='h', type="string", action="store", help="Which map. Default:h (charged hadrons)")
parser.add_option("--func", dest="func", default='x*[0]+x**2*[1]', type="string", action="store", help="Which function to use. Default: x*[0]+x**2*[1]")
parser.add_option("--rebin", dest="rebin", default=0, type="int", action="store", help="Rebinning to performe on tProfiles")
parser.add_option("--fitRange", dest="fitRange", default='0,2000', type="string", action="store", help="Which fitRange. Default:0,2000")
parser.add_option("--xZoomRange", dest="xZoomRange", default='', type="string", action="store", help="Which xZoomRange. Default:Max")
parser.add_option("--yZoomRange", dest="yZoomRange", default='-40,40', type="string", action="store", help="Which zoom range in y Axis. Default:-40,40")
parser.add_option("--varType", dest="varType", default='sumPt', type="string", action="store", help="Which varType [multiplicity/ngoodVertices/sumPt]")
parser.add_option("--input", dest="input", default='DYJetsToLL_M-50_HT-100toInf_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_AODSIM.root', type="string", action="store", help="input file.Default:DYJetsToLL_M-50_HT-100toInf_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_AODSIM.root")
parser.add_option("--rootGDir", dest="rootGDir", default='metPhiCorrInfoWriter', type="string", action="store", help="Which gDir was used in the production of the MEx,y profile [metPhiCorrInfoWriter/pfMEtMultCorrInfoWriter].")
parser.add_option("--plotFileName", dest="plotFileName", default="plot.pdf", type="string", action="store", help="Filename the plot. Default:test.pdf")
parser.add_option("--plotoutPutDir", dest="plotoutPutDir", default="/", type="string", action="store", help="dir name Default:/")
parser.add_option("--scriptFileName", dest="scriptFileName", default="metPhiCorrections_cfi.py", type="string", action="store", help="Text file name that the corrections are appended to. Default:metPhiCorrections_cfi.py.")
parser.add_option("--batch", dest="batch", default="False", type="string", action="store", help="Choose batch varType. If True, no canvas pop-up. Default:False.")
(options, args) = parser.parse_args()

import ROOT
import pickle, os

from math import pi, cos, sin, sqrt, atan2
ROOT.gROOT.ProcessLine(".L $CMSSW_BASE/src/MetTools/Commons/scripts/tdrstyle.C")
ROOT.setTDRStyle()

if options.batch=="True":
  ROOT.gROOT.SetBatch(True)

from MetTools.MetPhiCorrections.tools.categories import *
from MetTools.MetPhiCorrections.tools.helpers import getObjFromFile


#assert options.map in [m['name'].replace('HF','HF_') for m in allMaps],("map %s not in "+",".join([m['name'].replace('HF','HF_') for m in allMaps])) % options.map
exec("map = " +options.map)
exec("yZoomRange = (" +options.yZoomRange+ ")")
exec("fitRange = (" +options.fitRange+ ")")
if options.xZoomRange!='':
  exec("xZoomRange = (" +options.xZoomRange+ ")")
else:
  xZoomRange = None

assert options.varType in varTypes,"Mode %s not known. Must be one of [ngoodVertices/sumPt/multiplicity]"%options.varType
print 'map', options.map, 'input', options.input, 'varType', options.varType, 'yZoomrange', yZoomRange, 'fitRange', fitRange, 'xZoomRange', xZoomRange



fx = ROOT.TF1('fx', options.func, *(fitRange))
fy = ROOT.TF1('fy', options.func, *(fitRange))
iname = options.rootGDir+'/'+options.rootGDir+'_'+options.varType+'_'+map['name'].replace('h_HF','hHF').replace('egamma_HF','egammaHF')
print 'iname', iname
px = getObjFromFile(options.input, iname+'_Px')
py = getObjFromFile(options.input, iname+'_Py')

occupancy = getObjFromFile(options.input, iname+'_occupancy')
energy = getObjFromFile(options.input, iname+'_energy')
pt = getObjFromFile(options.input, iname+'_pt')
variable = getObjFromFile(options.input, iname+'_variable')

assert px and py, "Could not read %s{_Px,_Py} from input from file %s, maybe --rootGDir not set correctly (is %s)"%(iname, options.input, options.rootGDir)
assert occupancy, "Could not read %s_occupancy from input from file %s, maybe --rootGDir not set correctly (is %s)"%(iname, options.input, options.rootGDir)
assert energy, "Could not read %s_energy from input from file %s, maybe --rootGDir not set correctly (is %s)"%(iname, options.input, options.rootGDir)
assert variable, "Could not read %s_variable from input from file %s, maybe --rootGDir not set correctly (is %s)"%(iname, options.input, options.rootGDir)


if options.rebin:
  px.Rebin(options.rebin)
  py.Rebin(options.rebin)

c1 = ROOT.TCanvas()  
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(0)
if options.varType=="multiplicity":
  px.GetXaxis().SetTitle("multiplicity in "+map['name'])
  py.GetXaxis().SetTitle("multiplicity in "+map['name'])
if options.varType=="sumPt":
  px.GetXaxis().SetTitle("#Sigma p_{T} of "+map['name'])
  py.GetXaxis().SetTitle("#Sigma p_{T} of "+map['name'])
if options.varType=="ngoodVertices":
  px.GetXaxis().SetTitle("ngoodVertices")
  py.GetXaxis().SetTitle("ngoodVertices")

px.GetYaxis().SetTitle("<#slash{E}_{x}> (GeV)")
px.GetXaxis().SetTitleSize(0.05)
px.GetXaxis().SetTitleOffset(1.1)
px.GetYaxis().SetRangeUser(*yZoomRange)
py.GetYaxis().SetTitle("<#slash{E}_{y}> (GeV)")
py.GetXaxis().SetTitleSize(0.05)
py.GetXaxis().SetTitleOffset(1.1)
py.GetYaxis().SetRangeUser(*yZoomRange)
if xZoomRange:
  px.GetXaxis().SetRangeUser(*xZoomRange)
  py.GetXaxis().SetRangeUser(*xZoomRange)


px_clone = px.Clone(map['name'].replace('h_HF','hHF').replace('egamma_HF','egammaHF')+'_'+options.varType+'_Px')
py_clone = py.Clone(map['name'].replace('h_HF','hHF').replace('egamma_HF','egammaHF')+'_'+options.varType+'_Py')

fx.SetLineColor(ROOT.kBlue);
fy.SetLineColor(ROOT.kRed);
px.Fit(fx,'R')
py.Fit(fy,'R')

px.SetLineColor(ROOT.kBlue)
px.SetLineStyle(0)
px.SetLineWidth(2)
px.SetMarkerStyle(0)
px.SetMarkerSize(0)
px.Draw('h')
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

plotFileName=options.map+'_pxpy_'+options.varType+'.png'
c1.Print(options.plotoutPutDir+"/"+plotFileName)
c1.Print(options.plotoutPutDir+"/"+plotFileName.replace('.png', '.pdf'))

with open(options.plotoutPutDir+"/"+options.scriptFileName, "a") as ofile:
  ofile.write('    cms.PSet(\n')
  ofile.write('      name=cms.string("'+map['name'].replace('_','')+'"),\n')
  ofile.write('      type=cms.int32('+str(label[map['type']])+'),\n')
  ofile.write('      varType=cms.int32('+str(varType[options.varType])+'),\n')
  ofile.write('      etaMin=cms.double('+str(map['etaPhiBinning'][1])+'),\n')
  ofile.write('      etaMax=cms.double('+str(map['etaPhiBinning'][2])+'),\n')
  ofile.write('      fx=cms.string("'+fx.GetExpFormula().Data()+'"),\n')
  ofile.write('      px=cms.vdouble('+','.join(str(fx.GetParameter(i)) for i in range(fx.GetNpar()))+'),\n')
  ofile.write('      fy=cms.string("'+fy.GetExpFormula().Data()+'"),\n')
  ofile.write('      py=cms.vdouble('+','.join(str(fy.GetParameter(i)) for i in range(fy.GetNpar()))+'),\n')
  ofile.write('    ),\n')

txtFileName = options.scriptFileName.replace('_cfi.py', '.txt')
with open(options.plotoutPutDir+"/"+txtFileName, "a") as ofile:
  ofile.write('['+map['name'].replace('_','')+']\n')
  ofile.write('{'+str(label[map['type']])+' ') # ptclType
  ofile.write(str('1')+' ') # number of bin variables
  ofile.write('eta'+' ') # name of bin variables
  ofile.write(str('1')+' ') # number of parameterizing variables
  ofile.write(str(varType[options.varType])+' ') # type of parameterising variable
  ofile.write(fy.GetExpFormula().Data()+'}'+'\n') # formula
  ofile.write('X'+' ') # formula
  ofile.write(str(map['etaPhiBinning'][1])+' ') # 1st bin
  ofile.write(str(map['etaPhiBinning'][2])+' ') # 2nd bin
  ofile.write(str(fx.GetNpar())+' ') # number of parameters
  ofile.write(' '.join(str(fx.GetParameter(i)) for i in range(fx.GetNpar()))+'\n')
  ofile.write('Y'+' ') # formula
  ofile.write(str(map['etaPhiBinning'][1])+' ') # 1st bin
  ofile.write(str(map['etaPhiBinning'][2])+' ') # 2nd bin
  ofile.write(str(fy.GetNpar())+' ') # number of parameters
  ofile.write(' '.join(str(fy.GetParameter(i)) for i in range(fy.GetNpar()))+'\n')

c_occupancy = ROOT.TCanvas()
occupancy.GetYaxis().SetTitle("<#phi>")
occupancy.GetYaxis().SetTitleSize(0.05)
occupancy.GetYaxis().SetTitleOffset(1.1)
occupancy.GetXaxis().SetTitle("<#eta>")
occupancy.GetXaxis().SetTitleSize(0.05)
occupancy.GetXaxis().SetTitleOffset(1.1)
occupancy.Draw("colz")
plotFileName=options.map+'_'+'occupancy'+'.png'
c_occupancy.Print(options.plotoutPutDir+"/"+plotFileName)
c_occupancy.Print(options.plotoutPutDir+"/"+plotFileName.replace('.png','.pdf'))

c_energy = ROOT.TCanvas()
energy.GetYaxis().SetTitle("<#phi>")
energy.GetYaxis().SetTitleSize(0.05)
energy.GetYaxis().SetTitleOffset(1.1)
energy.GetXaxis().SetTitle("<#eta>")
energy.GetXaxis().SetTitleSize(0.05)
energy.GetXaxis().SetTitleOffset(1.1)
energy.Draw("colz")
plotFileName=options.map+'_'+'energy'+'.png'
c_energy.Print(options.plotoutPutDir+"/"+plotFileName)
c_energy.Print(options.plotoutPutDir+"/"+plotFileName.replace('.png','.pdf'))


c_pt = ROOT.TCanvas()
pt.GetYaxis().SetTitle("<#phi>")
pt.GetYaxis().SetTitleSize(0.05)
pt.GetYaxis().SetTitleOffset(1.1)
pt.GetXaxis().SetTitle("<#eta>")
pt.GetXaxis().SetTitleSize(0.05)
pt.GetXaxis().SetTitleOffset(1.1)
pt.Draw("colz")
plotFileName=options.map+'_'+'pt'+'.png'
c_pt.Print(options.plotoutPutDir+"/"+plotFileName)
c_pt.Print(options.plotoutPutDir+"/"+plotFileName.replace('.png','.pdf'))


c_variable = ROOT.TCanvas()
variable.GetYaxis().SetTitle("nevent/"+options.varType)
variable.GetYaxis().SetTitleSize(0.05)
variable.GetYaxis().SetTitleOffset(1.1)
variable.GetXaxis().SetTitle(options.varType)
variable.GetXaxis().SetTitleSize(0.05)
variable.GetXaxis().SetTitleOffset(1.1)
variable.Draw()
plotFileName=options.map+'_'+options.varType+'.png'
c_variable.Print(options.plotoutPutDir+"/"+plotFileName)
c_variable.Print(options.plotoutPutDir+"/"+plotFileName.replace('.png','.pdf'))

outRootF = ROOT.TFile(options.plotoutPutDir+"/"+options.map+"_"+options.varType+".root","recreate")
fx.Write()
fy.Write()
px_clone.Write()
py_clone.Write()
#outRootF.Write()
outRootF.Close()
