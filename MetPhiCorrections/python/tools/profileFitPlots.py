from optparse import OptionParser
from collections import *
from array import array

parser = OptionParser()
parser.add_option("--map", dest="map", default='h', type="string", action="store", help="Which map. Default:h (charged hadrons)")
parser.add_option("--func", dest="func", default='x*[0]+x**2*[1]', type="string", action="store", help="Which function to use. Default: x*[0]+x**2*[1]")
parser.add_option("--rebin", dest="rebin", default=0, type="int", action="store", help="Rebinning to performe on tProfiles")
parser.add_option("--fitRange", dest="fitRange", default='0,2000', type="string", action="store", help="Which fitRange. Default:0,2000")
parser.add_option("--fitRange1", dest="fitRange1", default='0,2000', type="string", action="store", help="Which fitRange1. Default:0,2000")
parser.add_option("--fitRange2", dest="fitRange2", default='0,2000', type="string", action="store", help="Which fitRange2. Default:0,2000")
parser.add_option("--fitRange3", dest="fitRange3", default='0,2000', type="string", action="store", help="Which fitRange3. Default:0,2000")
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
exec("fitRange1 = (" +options.fitRange1+ ")")
exec("fitRange2 = (" +options.fitRange2+ ")")
exec("fitRange3 = (" +options.fitRange3+ ")")
if options.xZoomRange!='':
  exec("xZoomRange = (" +options.xZoomRange+ ")")
else:
  xZoomRange = None

assert options.varType in varTypes,"Mode %s not known. Must be one of [ngoodVertices/sumPt/multiplicity]"%options.varType
print 'map', options.map, 'input', options.input, 'varType', options.varType, 'yZoomrange', yZoomRange, 'fitRange', fitRange, 'xZoomRange', xZoomRange

#if options.map != 'hEtaMinus': exit()

if options.varType == 'pfType1':
  iname = options.rootGDir+'/'+options.rootGDir+'_'+options.varType.replace('pfType1', 'pfMet')
elif options.varType == 'ptValence':
  iname = options.rootGDir+'/'+options.rootGDir+'_'+'ptcMet'+'_'+map['name'].replace('h_HF','hHF').replace('egamma_HF','egammaHF')
elif options.varType in ['ptcMet']:
  iname = options.rootGDir+'/'+map['name'].replace('h_HF','hHF').replace('egamma_HF','egammaHF')
  inameFull = options.rootGDir+'/'+options.rootGDir+'_'+'ptcMet'+'_'+map['name'].replace('h_HF','hHF').replace('egamma_HF','egammaHF')
elif options.varType in ['metPhiNvtx']:
  iname = options.rootGDir+'/'
  inameFull = options.rootGDir+'/'+options.rootGDir+'_'+'ptcMet'+'_'+map['name'].replace('h_HF','hHF').replace('egamma_HF','egammaHF')
else:
  iname = options.rootGDir+'/'+options.rootGDir+'_'+options.varType+'_'+map['name'].replace('h_HF','hHF').replace('egamma_HF','egammaHF')
print 'iname', iname


print 'varType is', options.varType
ProfList_MetX = []
ProfList_MetY = []

if options.varType == 'pfType1':
  for i in range(0,36):
    ProfList_MetX.append( ROOT.TProfile())
    ProfList_MetY.append( ROOT.TProfile())
    ProfList_MetX[i]=getObjFromFile(options.input, options.rootGDir+'/'+'prof_MetX_Nvtx'+str(i))
    ProfList_MetY[i]=getObjFromFile(options.input, options.rootGDir+'/'+'prof_MetY_Nvtx'+str(i))
    assert ProfList_MetX[i], "Could not read %s from input from file %s, "%(options.rootGDir+'/'+'prof_MetX_Nvtx_Nvtx'+str(i), options.input)
    assert ProfList_MetY[i], "Could not read %s from input from file %s, "%(options.rootGDir+'/'+'prof_MetX_Nvtx_Nvtx'+str(i), options.input)
  fx = ROOT.TF1('fx', options.func, *(fitRange))
  fy = ROOT.TF1('fy', options.func, *(fitRange))
elif options.varType == 'ptValence':
  ptValence  = ROOT.TProfile()
  ptValence1 = ROOT.TProfile()
  ptValence2 = ROOT.TProfile()
  ptValence3 = ROOT.TProfile()
  ptValence4 = ROOT.TProfile()
  ptValence5 = ROOT.TProfile()
  ptValence6 = ROOT.TProfile()
  ptValence7 = ROOT.TProfile()
  ptValence8 = ROOT.TProfile()
  ptValence9 = ROOT.TProfile()
  ptValence10= ROOT.TProfile()
  ptValence11= ROOT.TProfile()
  ptValList =OrderedDict( [('ptValence1',ptValence1), ('ptValence2',ptValence2), ('ptValence3',ptValence3)
              ,('ptValence4' ,ptValence4 ), ('ptValence5' ,ptValence5 ), ('ptValence6',ptValence6)
	      ,('ptValence7' ,ptValence7 ), ('ptValence8' ,ptValence8 ), ('ptValence9',ptValence9)
	      ,('ptValence10',ptValence10), ('ptValence11',ptValence11) ] )

#  ptValList =OrderedDict( {'ptValence1':ptValence1, 'ptValence2':ptValence2, 'ptValence3':ptValence3
#              ,'ptValence4':ptValence4, 'ptValence5':ptValence5, 'ptValence6':ptValence6
#	      ,'ptValence7':ptValence7, 'ptValence8':ptValence8, 'ptValence9':ptValence9
#	      ,'ptValence10':ptValence10, 'ptValence11':ptValence11 } )

  fPhi  = ROOT.TF1('fPhi', options.func, *(fitRange))
  fPhi1 = ROOT.TF1('fPhi', options.func, *(fitRange1))
  fPhi2 = ROOT.TF1('fPhi', options.func, *(fitRange2))
  fPhi3 = ROOT.TF1('fPhi', options.func, *(fitRange3))
  fPhi.SetLineColor(ROOT.kRed);
  #fPhi = ROOT.TF1('fPhi', '([0]+x*[1])', -3.1415,3.1415)
  flag = 0
  for key in ptValList.keys():
    ptValList[key] = getObjFromFile(options.input, iname+'_'+key)
    assert ptValList[key], "Could not read %s from input from file %s, maybe --rootGDir not set correctly (is %s)"%(iname+'_'+key, options.input, options.rootGDir)
    if options.rebin:
      ptValList[key].Rebin(options.rebin)
    if flag == 0:
      ptValList[key].Copy(ptValence)
      flag = 1
    else:
      ptValence.Add(ptValList[key])
elif options.varType == 'ptcMet':
  phiPtcMet = ROOT.TProfile()
  fPhi  = ROOT.TF1('fPhi', options.func, *(fitRange))
  fPhi.SetLineColor(ROOT.kRed);
  phiPtcMet = getObjFromFile(options.input, iname) #TODO should add phiPtcMet
  assert phiPtcMet, "Could not read %s from input from file %s, maybe --rootGDir not set correctly (is %s)"%(iname, options.input, options.rootGDir)
  if options.rebin:
    phiPtcMet.Rebin(options.rebin)
elif options.varType == 'metPhiNvtx':
  metPhi = ROOT.TProfile()
  fPhi = ROOT.TF1('fPhi',options.func, *(fitRange))
  fPhi1 = ROOT.TF1('fPhi1',options.func, *(fitRange1))
  fPhi2 = ROOT.TF1('fPhi1',options.func, *(fitRange2))
  fPhi3 = ROOT.TF1('fPhi1',options.func, *(fitRange3))
  fPhi.SetLineColor(ROOT.kRed);
  fPhi1.SetLineColor(ROOT.kRed);
  fPhi2.SetLineColor(ROOT.kRed);
  fPhi3.SetLineColor(ROOT.kRed);
  print 'iname:',iname
  metPhi = getObjFromFile(options.input, iname+'pfMet_phi') #TODO should add phiPtcMet
  assert metPhi, "Could not read %s from input from file %s, maybe --rootGDir not set correctly (is %s)"%(iname+'pfMet_phi', options.input, options.rootGDir)
  if options.rebin:
    metPhi.Rebin(options.rebin)
else:
  fx = ROOT.TF1('fx', options.func, *(fitRange))
  fy = ROOT.TF1('fy', options.func, *(fitRange))
  px      = getObjFromFile(options.input, iname+'_Px')
  py      = getObjFromFile(options.input, iname+'_Py')
  assert px and py, "Could not read %s{_Px,_Py} from input from file %s, maybe --rootGDir not set correctly (is %s)"%(iname, options.input, options.rootGDir)
  if options.rebin:
    px.Rebin(options.rebin)
    py.Rebin(options.rebin)

if options.varType != 'pfType1':
  occupancy = getObjFromFile(options.input, inameFull+'_occupancy')
  energy = getObjFromFile(options.input, inameFull+'_energy')
  pt = getObjFromFile(options.input, inameFull+'_pt')
  variable = getObjFromFile(options.input, inameFull+'_variable')
  ptcMetVsNvtx_MEtX = getObjFromFile(options.input, inameFull+'_ptcMetVsNvtx_MEtX')
  ptcMetVsNvtx_MEtY = getObjFromFile(options.input, inameFull+'_ptcMetVsNvtx_MEtY')

  assert occupancy, "Could not read %s_occupancy from input from file %s, maybe --rootGDir not set correctly (is %s)"%(iname, options.input, options.rootGDir)
  assert energy, "Could not read %s_energy from input from file %s, maybe --rootGDir not set correctly (is %s)"%(iname, options.input, options.rootGDir)
  assert variable, "Could not read %s_variable from input from file %s, maybe --rootGDir not set correctly (is %s)"%(iname, options.input, options.rootGDir)
  assert ptcMetVsNvtx_MEtX, "Could not read %s_ptcMetVsNvtx_MEtX from input from file %s, maybe --rootGDir not set correctly (is %s)"%(iname, options.input, options.rootGDir)
  assert ptcMetVsNvtx_MEtY, "Could not read %s_ptcMetVsNvtx_MEtY from input from file %s, maybe --rootGDir not set correctly (is %s)"%(iname, options.input, options.rootGDir)



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
if options.varType=="pfType1":
  for i in range(0,36):
    ProfList_MetX[i].GetXaxis().SetTitle("#slash{E}")
    ProfList_MetY[i].GetXaxis().SetTitle("#slash{E}")
if options.varType in ["ptValence"]:
  ptValence.GetXaxis().SetTitle("#phi")
if options.varType in ["metPhiNvtx"]:
  metPhi.GetXaxis().SetTitle("#phi")

fitXResList = {}
fitYResList = {}
if options.varType == 'pfType1':
  for i in range(0, 36):

    ProfList_MetX[i].GetYaxis().SetTitle("<#slash{E}_{x}> (GeV)")
    ProfList_MetX[i].GetYaxis().SetRangeUser(*yZoomRange)
    ProfList_MetX[i].GetYaxis().SetTitleSize(0.05)
    ProfList_MetX[i].GetYaxis().SetTitleOffset(1.1)

    ProfList_MetY[i].GetYaxis().SetTitle("<#slash{E}_{y}> (GeV)")
    ProfList_MetY[i].GetYaxis().SetTitleSize(0.05)
    ProfList_MetY[i].GetYaxis().SetTitleOffset(1.1)
    ProfList_MetY[i].GetYaxis().SetRangeUser(*yZoomRange)

    if xZoomRange:
      ProfList_MetX[i].GetXaxis().SetRangeUser(*xZoomRange)
      ProfList_MetY[i].GetXaxis().SetRangeUser(*xZoomRange)

    ProfList_MetX[i].SetLineColor(ROOT.kBlue);
    ProfList_MetX[i].SetMarkerColor(ROOT.kBlue);
    ProfList_MetX[i].SetLineStyle(0)
    ProfList_MetX[i].SetLineWidth(2)
    ProfList_MetX[i].SetMarkerStyle(0)
    ProfList_MetX[i].SetMarkerSize(0)


    ProfList_MetY[i].SetLineColor(ROOT.kRed);
    ProfList_MetY[i].SetMarkerColor(ROOT.kRed);
    ProfList_MetY[i].SetLineStyle(0)
    ProfList_MetY[i].SetLineWidth(2)
    ProfList_MetY[i].SetMarkerStyle(0)
    ProfList_MetY[i].SetMarkerSize(0)

    print 'fitting','X'+str(i)
    ProfList_MetX[i].Fit(fx,'R')
    print 'fitting','Y'+str(i)
    ProfList_MetY[i].Fit(fy,'R')
    

    paraXList=[fx.GetNpar()]
    paraYList=[fy.GetNpar()]
    for j in range(0,fx.GetNpar()):
      print 'appending fx para:',j, fx.GetParameter(j)
      paraXList.append(fx.GetParameter(j))
      paraYList.append(fy.GetParameter(j))

    print 'paraXList', paraXList
    fitXResList[i]=paraXList
    fitYResList[i]=paraYList
    print 'fitXResList[i]',fitXResList[i]
    
    ProfList_MetX[i].Draw('h')
    #    py.GetYaxis().SetRangeUser(-20,20)
    ProfList_MetY[i].Draw('hsame')
    
    l = ROOT.TLegend(0.25,0.85,.45,.95)
    l.AddEntry(ProfList_MetX[i], "< #slash{E}_{x} >")#+etab[0].split('_')[0]+", "+shortName[etab[0].split('_')[1]])
    l.AddEntry(ProfList_MetY[i], "< #slash{E}_{y} >")
    l.SetFillColor(0)
    l.SetShadowColor(ROOT.kWhite)
    l.SetBorderSize(1)
    l.Draw()
    plotFileName=options.map+'_'+ options.varType+'_Nvtx'+str(i)+'.png'
    c1.Print(options.plotoutPutDir+"/"+plotFileName)
    c1.Print(options.plotoutPutDir+"/"+plotFileName.replace('.png', '.pdf'))
elif options.varType == 'ptValence':
  ptValence_clone = ptValence.Clone(map['name'].replace('h_HF','hHF').replace('egamma_HF','egammaHF')+'_'+'ptcMet'+'_'+options.varType)
  par0s= array('d')
  par0Errs= array('d')
  x =    [2.5, 7.5, 12.5, 17.5, 22.5, 27.5, 32.5, 37.5, 42.5, 47.5, 52.5]
  xerr = [2.5, 2.5, 2.5,  2.5,  2.5,  2.5,  2.5,  2.5,  2.5,  2.5,  2.5]
  for key in ptValList.keys():
    print 'key:',key
    ptValList[key].Fit(fPhi,'R')
    ptValList[key].Draw('h')
    l = ROOT.TLegend(0.55,0.87,.90,.95)
    l.AddEntry(ptValList[key], "#slash{E}_{+#phi} - #slash{E}_{count#phi}")
    l.SetFillColor(0)
    l.SetShadowColor(ROOT.kWhite)
    l.SetBorderSize(1)
    l.Draw() 
    plotFileName=options.map+'_ptcMet_'+key+'.png'
    c1.Print(options.plotoutPutDir+"/"+plotFileName)
    c1.Print(options.plotoutPutDir+"/"+plotFileName.replace('.png', '.pdf'))
    par0s.append( fPhi.GetParameter(0) )
    par0Errs.append( fPhi.GetParError(0) )
  gr_par0 = ROOT.TGraphErrors(len(par0s),array('d',x), par0s, array('d',xerr), par0Errs)
  gr_par0.GetXaxis().SetTitle("#slash{E}_{ptc}")
  gr_par0.GetXaxis().SetTitleSize(0.05)
  gr_par0.GetXaxis().SetTitleOffset(1.1)
  gr_par0.GetYaxis().SetRangeUser(-20, 20)
  gr_par0.Draw('alp')
  l = ROOT.TLegend(0.55,0.87,.90,.95)
  l.AddEntry(gr_par0, "pa0")
  l.SetFillColor(0)
  l.SetShadowColor(ROOT.kWhite)
  l.SetBorderSize(1)
  l.Draw()
  fPar0RngUp = 53
  if options.map == 'egammaHFMinus': fPar0RngUp =  5*4
  if options.map == 'egammaHFPlus': fPar0RngUp =  5*4
  if options.map == 'h0Barrel': fPar0RngUp =  5*7
  if options.map == 'h0EndcapMinus': fPar0RngUp =  5*7
  if options.map == 'h0EndcapPlus': fPar0RngUp =  5*5
  fPar0 = ROOT.TF1('fPar0', '(x*[0]+x*x*[1]+x*x*x*[2])', 0, fPar0RngUp)
  gr_par0.Fit(fPar0,'R')
  plotFileName=options.map+'_ptcMet_'+'ptValencePar0'+'.png'
  c1.Print(options.plotoutPutDir+"/"+plotFileName)
  c1.Print(options.plotoutPutDir+"/"+plotFileName.replace('.png', '.pdf'))

  ptValence.Fit(fPhi,'R')
  ptValence.Draw('h')
  l = ROOT.TLegend(0.55,0.87,.90,.95)
  l.AddEntry(ptValence, "#slash{E}_{+#phi} - #slash{E}_{count#phi}")
  l.SetFillColor(0)
  l.SetShadowColor(ROOT.kWhite)
  l.SetBorderSize(1)
  l.Draw() 
  plotFileName=options.map+'_ptcMet_'+options.varType+'.png'
  c1.Print(options.plotoutPutDir+"/"+plotFileName)
  c1.Print(options.plotoutPutDir+"/"+plotFileName.replace('.png', '.pdf'))
elif options.varType == 'ptcMet':
  phiPtcMet.Fit(fPhi,'R')
  phiPtcMet.Draw('h')
  meanContents = 0
  for i in range(phiPtcMet.GetNbinsX()):
    meanContents += phiPtcMet.GetBinContent(i+1)
  meanContents /= phiPtcMet.GetNbinsX()
  #stats= array('d')
  #stats.append(0)
  #stats.append(1)
  #stats.append(2)
  #stats.append(3)
  #stats.append(4)
  #stats.append(5)
  #phiPtcMet.GetStats(stats)
  #print 'stats !!!!!!!!!!!!!!!!!!!!!!', stats
  #sumwMean = stats[0]/phiPtcMet.GetNbinsX()
  l = ROOT.TLegend(0.55,0.87,.90,.95)
  l.AddEntry(phiPtcMet, "#slash{E}_{"+map['name']+"}")
  l.SetFillColor(0)
  l.SetShadowColor(ROOT.kWhite)
  l.SetBorderSize(1)
  l.Draw() 
  plotFileName=options.map+'_phiPtcMet'+'.png'
  c1.Print(options.plotoutPutDir+"/"+plotFileName)
  c1.Print(options.plotoutPutDir+"/"+plotFileName.replace('.png', '.pdf'))
elif options.varType == 'metPhiNvtx':
  metPhi.Fit(fPhi1,'R')
  metPhi.Fit(fPhi2,'R+')
  metPhi.Fit(fPhi3,'R+')
  metPhi.Draw('h')
  l = ROOT.TLegend(0.75,0.87,.90,.95)
  l.AddEntry(metPhi, "#slash{E}")
  l.SetFillColor(0)
  l.SetShadowColor(ROOT.kWhite)
  l.SetBorderSize(1)
  l.Draw() 
  plotFileName='metPhi'+'.png'
  c1.Print(options.plotoutPutDir+"/"+plotFileName)
  c1.Print(options.plotoutPutDir+"/"+plotFileName.replace('.png', '.pdf'))
else :
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
  
  l = ROOT.TLegend(0.25,0.85,.45,.95)
  l.AddEntry(px, "< #slash{E}_{x} >")#+etab[0].split('_')[0]+", "+shortName[etab[0].split('_')[1]])
  l.AddEntry(py, "< #slash{E}_{y} >")
  l.SetFillColor(0)
  l.SetShadowColor(ROOT.kWhite)
  l.SetBorderSize(1)
  l.Draw()
  plotFileName=options.map+'_pxpy_'+options.varType+'.png'
  c1.Print(options.plotoutPutDir+"/"+plotFileName)
  c1.Print(options.plotoutPutDir+"/"+plotFileName.replace('.png', '.pdf'))

if options.varType != 'pfType1':
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
  ptcMetMean = variable.GetMean()
  ptcMetMeanErr = variable.GetMeanError()

for i in range(0, 36):
  print i,str(fitXResList[i][0])+','+','.join(str(fitXResList[i][1+j]) for j in range(fitXResList[i][0]))

#with open(options.plotoutPutDir+"/"+options.scriptFileName, "a") as ofile:
#  ofile.write('    cms.PSet(\n')
#  ofile.write('      name=cms.string("'+map['name'].replace('_','')+'"),\n')
#  ofile.write('      type=cms.int32('+str(label[map['type']])+'),\n')
#  ofile.write('      varType=cms.int32('+str(varType[options.varType])+'),\n')
#  ofile.write('      etaMin=cms.double('+str(map['etaPhiBinning'][1])+'),\n')
#  ofile.write('      etaMax=cms.double('+str(map['etaPhiBinning'][2])+'),\n')
#  if options.varType == 'ptValence':
#    ofile.write('      fPhi=cms.string("'+fPhi.GetExpFormula().Data()+'"),\n')
#    ofile.write('      pPhi=cms.vdouble('+','.join(str(fPhi.GetParameter(i)) for i in range(fPhi.GetNpar()))+'),\n')
#  elif options.varType in ['ptcMet','metPhiNvtx']:
#    ofile.write('      fPhi=cms.string("'+fPhi.GetExpFormula().Data()+'"),\n')
#    ofile.write('      pPhi=cms.vdouble('+','.join(str(fPhi.GetParameter(i)) for i in range(fPhi.GetNpar()))+'),\n')
#  else :
#    ofile.write('      fx=cms.string("'+fx.GetExpFormula().Data()+'"),\n')
#    ofile.write('      px=cms.vdouble('+','.join(str(fx.GetParameter(i)) for i in range(fx.GetNpar()))+'),\n')
#    ofile.write('      fy=cms.string("'+fy.GetExpFormula().Data()+'"),\n')
#    ofile.write('      py=cms.vdouble('+','.join(str(fy.GetParameter(i)) for i in range(fy.GetNpar()))+'),\n')
#  ofile.write('    ),\n')

txtFileName = options.scriptFileName.replace('_cfi.py', '.txt')
with open(options.plotoutPutDir+"/"+txtFileName, "a") as ofile:
  ofile.write('['+map['name'].replace('_','')+']\n')
  ofile.write('{'+str(label[map['type']])+' ') # ptclType
  ofile.write(str('1')+' ') # number of bin variables
  ofile.write('eta'+' ') # name of bin variables
  ofile.write(str('1')+' ') # number of parameterized variables for shift correction
  ofile.write(str(varType[options.varType])+' ') # type of parameterised variable
  ofile.write(str(options.fitRange.split(',')[0])+' '+str(options.fitRange.split(',')[1])+' ') # fit range
  if options.varType == 'pfType1':
    ofile.write(fy.GetExpFormula().Data()+'}'+'\n') # formula
    for i in range(0, 36):
      ofile.write('X'+str(i)+' ') # 
      ofile.write(str(map['etaPhiBinning'][1])+' ') # 1st bin
      ofile.write(str(map['etaPhiBinning'][2])+' ') # 2nd bin
      ofile.write(str(fitXResList[i][0])+' ') # number of parameters
      ofile.write(' '.join(str(fitXResList[i][1+j]) for j in range(fitXResList[i][0]))+'\n')
      ofile.write('Y'+str(i)+' ') # formula
      ofile.write(str(map['etaPhiBinning'][1])+' ') # 1st bin
      ofile.write(str(map['etaPhiBinning'][2])+' ') # 2nd bin
      ofile.write(str(fitYResList[i][0])+' ') # number of parameters
      ofile.write(' '.join(str(fitYResList[i][1+j]) for j in range(fitYResList[i][0]))+'\n')
  elif options.varType == 'ptValence':
    ofile.write(fPhi.GetExpFormula().Data()+'}'+'\n') # formula
    ofile.write('Phi'+' ') # 
    ofile.write(str(map['etaPhiBinning'][1])+' ') # 1st bin
    ofile.write(str(map['etaPhiBinning'][2])+' ') # 2nd bin
    ofile.write(str(fPhi.GetNpar())+' ') # number of parameters
    ofile.write(' '.join(str(fPhi.GetParameter(i)) for i in range(fPhi.GetNpar()))+'\n')
    ofile.write('Par0'+' ')
    ofile.write(str(0)+' ')
    ofile.write(str(fPar0RngUp)+' ')
    ofile.write(str(fPar0.GetNpar())+' ') # number of parameters
    ofile.write(' '.join(str(fPar0.GetParameter(i)) for i in range(fPar0.GetNpar()))+'\n')
  elif options.varType == 'ptcMet':
    ofile.write(fPhi.GetExpFormula().Data().split(')')[0]+'-'+str(meanContents)+')'+'}'+'\n') # formula
    ofile.write('Phi'+' ') # 
    ofile.write(str(map['etaPhiBinning'][1])+' ') # 1st bin
    ofile.write(str(map['etaPhiBinning'][2])+' ') # 2nd bin
    ofile.write(str(fPhi.GetNpar())+' ') # number of parameters
    ofile.write(' '.join(str(fPhi.GetParameter(i)) for i in range(fPhi.GetNpar()))+'\n')
    ofile.write('PtcMet'+' ')
    ofile.write(str(0)+' ')
    ofile.write(str(ptcMetMean*2)+' ')
    ofile.write(str(1)+' ')
    ofile.write(str(1/ptcMetMean)+'\n')
  elif options.varType == 'metPhiNvtx':
    #ofile.write(fPhi1.GetExpFormula().Data()+'}'+'\n') # formula
    ofile.write(fPhi.GetExpFormula().Data().split(')')[0]+'-'+str(34.7)+')'+'}'+'\n') # formula
    ofile.write('Phi1'+' ') # 
    ofile.write(str(options.fitRange1.split(',')[0])+' '+str(options.fitRange1.split(',')[1])+' ') # fit range
    ofile.write(str(fPhi1.GetNpar())+' ') # number of parameters
    ofile.write(' '.join(str(fPhi1.GetParameter(i)) for i in range(fPhi1.GetNpar()))+'\n')
    ofile.write('Phi2'+' ') # 
    ofile.write(str(options.fitRange2.split(',')[0])+' '+str(options.fitRange2.split(',')[1])+' ') # fit range
    ofile.write(str(fPhi2.GetNpar())+' ') # number of parameters
    ofile.write(' '.join(str(fPhi2.GetParameter(i)) for i in range(fPhi2.GetNpar()))+'\n')
    ofile.write('Phi3'+' ') # 
    ofile.write(str(options.fitRange3.split(',')[0])+' '+str(options.fitRange3.split(',')[1])+' ') # fit range
    ofile.write(str(fPhi3.GetNpar())+' ') # number of parameters
    ofile.write(' '.join(str(fPhi3.GetParameter(i)) for i in range(fPhi3.GetNpar()))+'\n')
  else :
    ofile.write(fy.GetExpFormula().Data()+'}'+'\n') # formula
    ofile.write('X'+' ') # 
    ofile.write(str(map['etaPhiBinning'][1])+' ') # 1st bin
    ofile.write(str(map['etaPhiBinning'][2])+' ') # 2nd bin
    ofile.write(str(fx.GetNpar())+' ') # number of parameters
    ofile.write(' '.join(str(fx.GetParameter(i)) for i in range(fx.GetNpar()))+'\n')
    ofile.write('Y'+' ') # formula
    ofile.write(str(map['etaPhiBinning'][1])+' ') # 1st bin
    ofile.write(str(map['etaPhiBinning'][2])+' ') # 2nd bin
    ofile.write(str(fy.GetNpar())+' ') # number of parameters
    ofile.write(' '.join(str(fy.GetParameter(i)) for i in range(fy.GetNpar()))+'\n')

"""
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



c_ptcMetVsNvtx_MEtX = ROOT.TCanvas()
ptcMetVsNvtx_MEtX.GetYaxis().SetTitle("nVtx")
ptcMetVsNvtx_MEtX.GetYaxis().SetTitleSize(0.05)
ptcMetVsNvtx_MEtX.GetYaxis().SetTitleOffset(1.1)
ptcMetVsNvtx_MEtX.GetXaxis().SetTitle("ptcMet")
ptcMetVsNvtx_MEtX.GetXaxis().SetTitleSize(0.05)
ptcMetVsNvtx_MEtX.GetXaxis().SetTitleOffset(1.1)
#ptcMetVsNvtx_MEtX.Draw("colz2")
ptcMetVsNvtx_MEtX.Draw("cont")
plotFileName=options.map+'_'+'ptcMetVsNvtx_MEtX'+'.png'
c_ptcMetVsNvtx_MEtX.SaveAs(options.plotoutPutDir+"/"+plotFileName)
c_ptcMetVsNvtx_MEtX.SaveAs(options.plotoutPutDir+"/"+plotFileName.replace('.png','.pdf'))


c_ptcMetVsNvtx_MEtY = ROOT.TCanvas()
ptcMetVsNvtx_MEtY.GetYaxis().SetTitle("nVtx")
ptcMetVsNvtx_MEtY.GetYaxis().SetTitleSize(0.05)
ptcMetVsNvtx_MEtY.GetYaxis().SetTitleOffset(1.1)
ptcMetVsNvtx_MEtY.GetXaxis().SetTitle("ptcMet")
ptcMetVsNvtx_MEtY.GetXaxis().SetTitleSize(0.05)
ptcMetVsNvtx_MEtY.GetXaxis().SetTitleOffset(1.1)
#ptcMetVsNvtx_MEtY.Draw("colz2")
ptcMetVsNvtx_MEtY.Draw("cont")
plotFileName=options.map+'_'+'ptcMetVsNvtx_MEtY'+'.png'
c_ptcMetVsNvtx_MEtY.SaveAs(options.plotoutPutDir+"/"+plotFileName)
c_ptcMetVsNvtx_MEtY.SaveAs(options.plotoutPutDir+"/"+plotFileName.replace('.png','.pdf'))
#c_ptcMetVsNvtx_MEtY.Print(options.plotoutPutDir+"/"+plotFileName)
#c_ptcMetVsNvtx_MEtY.Print(options.plotoutPutDir+"/"+plotFileName.replace('.png','.pdf'))


outRootF = ROOT.TFile(options.plotoutPutDir+"/"+options.map+"_"+options.varType+".root","recreate")
if options.varType == 'ptValence':
  fPhi.Write()
  ptValence_clone.Write()
elif options.varType == 'ptcMet':
  fPhi.Write()
elif options.varType == 'metPhiNvtx':
  fPhi.Write()
else:
  fx.Write()
  fy.Write()
  px_clone.Write()
  py_clone.Write()
#outRootF.Write()
outRootF.Close()
"""
