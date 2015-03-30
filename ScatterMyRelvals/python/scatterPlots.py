from optparse import OptionParser
parser = OptionParser()
parser.add_option("--xAxis", dest="xAxis", default='', type="string", action="store", help="[moniker:filname.zpkl for x-axis")
parser.add_option("--yAxis", dest="yAxis", default='', type="string", action="store", help="[moniker:filname.zpkl for y-axis]")
parser.add_option("--plotDirectory", dest="plotDirectory", default='.', type="string", action="store", help="Where should the plots go?")

(options, args) = parser.parse_args()

import ROOT
from DataFormats.FWLite import Events, Handle
from PhysicsTools.PythonAnalysis import *
from math import *
import sys, os, copy, random, subprocess, datetime

from MetTools.Commons.helpers import load

ROOT.gStyle.SetOptStat(0)
if not hasattr(ROOT, "tdrStyle"):
  ROOT.gROOT.ProcessLine(".L $CMSSW_BASE/src/MetTools/Commons/scripts/tdrstyle.C")
  ROOT.setTDRStyle()
  ROOT.tdrStyle.SetPadRightMargin(0.18)
  ROOT.gROOT.ProcessLine(".L $CMSSW_BASE/src/MetTools/Commons/scripts/useNiceColorPalette.C")
  ROOT.useNiceColorPalette(255)

maxEvts=-1

if ':' in options.xAxis:
  moniker_x, ifile_x = options.xAxis.split(':')
else:
  moniker_x, ifile_x = None, options.xAxis
if ':' in options.yAxis:
  moniker_y, ifile_y = options.yAxis.split(':')
else:
  moniker_y, ifile_y = None, options.yAxis

evts1 = load(ifile_x)
evts2 = load(ifile_y)

print "File %s for x-axis has %i events"%(ifile_x,len(evts1))
print "File %s for y-axis has %i events"%(ifile_y,len(evts2))

commonKeys =  [val for val in evts1.keys() if val in evts2.keys()]
assert len(commonKeys)>0, "0 events!"
print "Found",len(commonKeys),"common events"
ks1 = evts1[commonKeys[0]].keys()
ks1.sort()
ks2 = evts2[commonKeys[0]].keys()
ks2.sort()

if ks1!=ks2:
  print "Warning! Not the same keys!"
  print "1st:",ks1
  print "2nd:",ks2

scatterPlots = [val for val in ks1 if val in ks2]

for s in scatterPlots:
  maxVal = max([evts1[k][s] for k in commonKeys] + [evts2[k][s] for k in commonKeys])
  if not  maxVal>0:
    print "maxVal non-positive %f"%maxVal
    continue
  if 'mult' in s:
    binning=[min(100,maxVal),0,maxVal]
    pbinning=[min(20,maxVal),0,maxVal]
  else:
    scale = 10**(int(log(maxVal,10))-1)
    maximum= (floor(maxVal/scale)+1)*scale
    binning=[100,0,maximum]
    pbinning=[20,0,maximum]
  histo   = ROOT.TH2F('h'+s,'h'+s,*(binning+binning))
  profile = ROOT.TProfile('p'+s,'p'+s,*(pbinning+[0,maximum]))
  for k in commonKeys:
    histo.Fill(evts1[k][s], evts2[k][s])
    profile.Fill(evts1[k][s], evts2[k][s])
  mstr_x = ' ('+moniker_x+')' if moniker_x else ''
  mstr_y = ' ('+moniker_y+')' if moniker_y else ''
  histo.GetXaxis().SetTitle(s+mstr_x)
  histo.GetXaxis().SetLabelSize(0.04)
  histo.GetYaxis().SetTitle(s+mstr_y)
  histo.GetYaxis().SetLabelSize(0.04)
  profile.GetXaxis().SetTitle(s+mstr_x)
  profile.GetXaxis().SetLabelSize(0.04)
  profile.GetYaxis().SetTitle(s+mstr_y)
  profile.GetYaxis().SetLabelSize(0.04)
  profile.SetLineColor(ROOT.kGray)
  profile.SetMarkerStyle(0)
  profile.SetMarkerSize(0)

  c1 = ROOT.TCanvas()
  histo.Draw('colz')
  fit=True
  tf1=ROOT.TF1("lin","pol1",0,histo.GetXaxis().GetXmax())
  try:
    frs=profile.Fit(tf1,'S')
#    print "Status", frs.Status()
    fit=(frs.Status()==0)
  except:
    fit=False 
  c1.SetLogz()
  histo.Draw('colz')
  profile.Draw("eh1same")
  l=ROOT.TLine(0,0,histo.GetXaxis().GetXmax(),histo.GetXaxis().GetXmax())
  l.Draw()
  if fit:
    text=ROOT.TLatex()
    text.SetNDC()
    text.SetTextSize(0.04)
    text.SetTextAlign(11)
    text.SetTextColor(ROOT.kRed)
    text.DrawLatex(0.2,0.9,"Fit: "+str(round(tf1.GetParameter(0),4))+"+("+str(round(tf1.GetParameter(1),4))+")*x")
  c1.Print(options.plotDirectory+'/'+s+'.png')
  c1.Print(options.plotDirectory+'/'+s+'.pdf')
  c1.Print(options.plotDirectory+'/'+s+'.root')
  c1.RedrawAxis()
  del profile
  del histo
  del c1
#
###
####  if run not in runs:
####    runs.append(run)
####    print "Added run",run
####  if run==208352:
####    nf = fileNames[events.fileIndex()]
####    if nf not in usedFiles:
####      print "Found file",run,nf
####      usedFiles.append(nf)
###
####  print ":".join([str(x) for x in [event, run, lumi]] ),products['pfMet'][0].pt(),  products['pfMet'][0].sumEt()
  

#print "Have",len(commonKeys),"events in common"
#outliers={'met':[], 'sumEt':[],'met_ecal':[],'met_hcal':[],'met_ho':[],'sumEt_ecal':[],'sumEt_hcal':[],'sumEt_ho':[]}
#outliers.update({"sumPt_"+p:[] for p in pfTypes})
#outliers.update({"MET_"+p:[] for p in pfTypes})
#for k in commonKeys:
#  outliers['met'].append(   [res['73X'][k]['met'] - res['53X'][k]['met'],k,res['73X'][k]['met'],  res['53X'][k]['met']] )
#  outliers['sumEt'].append( [res['73X'][k]['sumEt'] - res['53X'][k]['sumEt'],k, res['73X'][k]['sumEt'], res['53X'][k]['sumEt']] )
#  outliers["met_ecal"].append([sqrt(res['73X'][k]["MEx_ecal"]**2 +res['73X'][k]["MEy_ecal"]**2) - sqrt(res['53X'][k]["MEx_ecal"]**2 +res['53X'][k]["MEy_ecal"]**2),k,sqrt(res['73X'][k]["MEx_ecal"]**2 +res['73X'][k]["MEy_ecal"]**2), sqrt(res['53X'][k]["MEx_ecal"]**2 +res['53X'][k]["MEy_ecal"]**2)])
#  outliers["sumEt_ecal"].append([res['73X'][k]["sumEt_ecal"] -  res['53X'][k]["sumEt_ecal"], k, res['73X'][k]["sumEt_ecal"], res['53X'][k]["sumEt_ecal"]] )
#  outliers["met_hcal"].append([sqrt(res['73X'][k]["MEx_hcal"]**2 +res['73X'][k]["MEy_hcal"]**2) - sqrt(res['53X'][k]["MEx_hcal"]**2 +res['53X'][k]["MEy_hcal"]**2),k,sqrt(res['73X'][k]["MEx_hcal"]**2 +res['73X'][k]["MEy_hcal"]**2), sqrt(res['53X'][k]["MEx_hcal"]**2 +res['53X'][k]["MEy_hcal"]**2)])
#  outliers["sumEt_hcal"].append([res['73X'][k]["sumEt_hcal"] -  res['53X'][k]["sumEt_hcal"], k, res['73X'][k]["sumEt_hcal"], res['53X'][k]["sumEt_hcal"] ])
#  outliers["met_ho"].append([sqrt(res['73X'][k]["MEx_ho"]**2 +res['73X'][k]["MEy_ho"]**2) - sqrt(res['53X'][k]["MEx_ho"]**2 +res['53X'][k]["MEy_ho"]**2),k,sqrt(res['73X'][k]["MEx_ho"]**2 +res['73X'][k]["MEy_ho"]**2), sqrt(res['53X'][k]["MEx_ho"]**2 +res['53X'][k]["MEy_ho"]**2)])
#  outliers["sumEt_ho"].append([res['73X'][k]["sumEt_ho"] -  res['53X'][k]["sumEt_ho"], k, res['73X'][k]["sumEt_ho"], res['53X'][k]["sumEt_ho"]] )
#  for p in pfTypes:
#    outliers["sumPt_"+p].append([res['73X'][k]["sumPt_"+p] -  res['53X'][k]["sumPt_"+p], k, res['73X'][k]["sumPt_"+p], res['73X'][k]['mult_'+p], res['53X'][k]["sumPt_"+p], res['53X'][k]['mult_'+p]])
#    outliers["MET_"+p].append([sqrt(res['73X'][k]["MEx_"+p]**2 +res['73X'][k]["MEy_"+p]**2) - sqrt(res['53X'][k]["MEx_"+p]**2 +res['53X'][k]["MEy_"+p]**2),k,sqrt(res['73X'][k]["MEx_"+p]**2 +res['73X'][k]["MEy_"+p]**2), res['73X'][k]['mult_'+p], sqrt(res['53X'][k]["MEx_"+p]**2 +res['53X'][k]["MEy_"+p]**2), res['53X'][k]['mult_'+p]])
#
#outliers['sumEt'].sort(reverse=True)
#outliers['met'].sort(reverse=True)
#for p in pfTypes:
#  outliers['sumPt_'+p].sort(reverse=True)
#  outliers['MET_'+p].sort(reverse=True)
#
#print "*************************"
#print "** Outliers: total met **"
#print "*************************"
#for i, o in enumerate(filter(lambda x:abs(x[0])>100, outliers['met'][:10]+outliers['met'][-10:])):
#  print "%2i met(73X-53X) %8.1f id %20s MET %8.1f (73X) %8.1f (53X)"%tuple([i]+o)+ " Details:"+" ".join([("sumPt_"+p2+"%8.1f (53X) %8.1f (73X)  MET_"+p2+"%8.1f (53X) %8.1f (73X)") %(res['53X'][o[1]]['sumPt_'+p2],res['73X'][o[1]]['sumPt_'+p2],res['53X'][o[1]]['MET_'+p2],res['73X'][o[1]]['MET_'+p2]) for p2 in pfTypes ]) 
#print "****************************"
#print "** Outliers: total sumET **"
#print "****************************"
#for o in outliers['sumEt'][:10]+outliers['sumEt'][-10:]:
#  if abs(o[0])>100:
#    print "sumEt(73X-53X) %8.1f id %20s sumEt %8.1f (73X) %8.1f (53X)"%tuple(o) + " Details:"+" ".join([("sumPt_"+p2+"%8.1f (53X) %8.1f (73X)  MET_"+p2+"%8.1f (53X) %8.1f (73X)") %(res['53X'][o[1]]['sumPt_'+p2],res['73X'][o[1]]['sumPt_'+p2],res['53X'][o[1]]['MET_'+p2],res['73X'][o[1]]['MET_'+p2]) for p2 in pfTypes ])
#print "***************************************"
#print "** Outliers sub-sums (MET and sumET) **"
#print "***************************************"
#for p in pfTypes:
#  print "Outliers: Met "+p
#  for o in outliers['MET_'+p][:10]+outliers['MET_'+p][-10:]:
#    if abs(o[0])>100:
#      print "MET "+p+" (73X-53X) %8.1f id %20s 73X: %8.1f (n=%4i) 53X: %8.1f (n=%4i)"%tuple(o)+ " Details:"+" ".join([("sumPt_"+p2+"%8.1f (53X) %8.1f (73X)  MET_"+p2+"%8.1f (53X) %8.1f (73X)") %(res['53X'][o[1]]['sumPt_'+p2],res['73X'][o[1]]['sumPt_'+p2],res['53X'][o[1]]['MET_'+p2],res['73X'][o[1]]['MET_'+p2]) for p2 in pfTypes ]) 
#  print "Outliers: sumPt "+p
#  for o in outliers['sumPt_'+p][:10]+outliers['sumPt_'+p][-10:]:
#    if abs(o[0])>100:
#      print "sumPt "+p+" (73X-53X) %8.1f id %20s 73X: %8.1f (n=%4i) 53X: %8.1f (n=%4i)"%tuple(o) + " Details:"+" ".join([("sumPt_"+p2+"%8.1f (53X) %8.1f (73X)  MET_"+p2+"%8.1f (53X) %8.1f (73X)") %(res['53X'][o[1]]['sumPt_'+p2],res['73X'][o[1]]['sumPt_'+p2],res['53X'][o[1]]['MET_'+p2],res['73X'][o[1]]['MET_'+p2]) for p2 in pfTypes ])
#
#import ROOT
#sumEt = ROOT.TH2F('sumEt','sumEt',100,0,5000,100,0,5000)
#met = ROOT.TH2F('met','met',100,0,500,100,0,500)
#metPhi = ROOT.TH2F('met','met',100,-pi,pi,100,-pi,pi)
#metPhi50 = ROOT.TH2F('met','met',100,-pi,pi,100,-pi,pi)
#sumEts={}
#METs={}
#for p in pfTypes:
#  sumEts[p] = ROOT.TH2F('sumEt','sumEt',500,0,5000,500,0,5000)
#  METs[p] = ROOT.TH2F('MET','MET',500,0,5000,500,0,5000)
# 
#for k in commonKeys:
#  met.Fill(res['53X'][k]['met'], res['73X'][k]['met'])
#  sumEt.Fill(res['53X'][k]['sumEt'], res['73X'][k]['sumEt'])
#  metPhi.Fill(res['53X'][k]['metPhi'], res['73X'][k]['metPhi'])
#  if res['53X'][k]['met']>50:
#    metPhi50.Fill(res['53X'][k]['metPhi'], res['73X'][k]['metPhi'])
#  for p in pfTypes:
#    sumEts[p].Fill(res['53X'][k]["sumPt_"+p], res['73X'][k]["sumPt_"+p])
#    METs[p].Fill(sqrt(res['53X'][k]["MEx_"+p]**2 +res['53X'][k]["MEy_"+p]**2) , sqrt(res['73X'][k]["MEx_"+p]**2 +res['73X'][k]["MEy_"+p]**2))
#
#metPhi.GetXaxis().SetTitle("53X #phi(MET)")
#metPhi.GetXaxis().SetLabelSize(0.04)
#metPhi.GetYaxis().SetTitle("73X #phi(MET)")
#metPhi.GetYaxis().SetLabelSize(0.04)
#metPhi50.GetXaxis().SetTitle("53X #phi(MET)")
#metPhi50.GetXaxis().SetLabelSize(0.04)
#metPhi50.GetYaxis().SetTitle("73X #phi(MET)")
#metPhi50.GetYaxis().SetLabelSize(0.04)
#met.GetXaxis().SetTitle("53X MET")
#met.GetXaxis().SetLabelSize(0.04)
#met.GetYaxis().SetLabelSize(0.04)
#met.GetYaxis().SetTitle("73X MET")
#sumEt.GetXaxis().SetTitle("53X sumEt")
#sumEt.GetYaxis().SetTitle("73X sumEt")
#sumEt.GetYaxis().SetLabelSize(0.04)
#sumEt.GetXaxis().SetLabelSize(0.04)
#sumEt.GetXaxis().SetRangeUser(0,3000)
#sumEt.GetYaxis().SetRangeUser(0,3000)
#
#for p in sumEts.keys():
#  sumEts[p].GetXaxis().SetTitle("53X sumEt "+p)
#  sumEts[p].GetYaxis().SetTitle("73X sumEt "+p)
#  sumEts[p].GetYaxis().SetLabelSize(0.04)
#  sumEts[p].GetXaxis().SetLabelSize(0.04)
#  sumEts[p].GetXaxis().SetRangeUser(0,1000)
#  sumEts[p].GetYaxis().SetRangeUser(0,1000)
#sumEts['h'].GetXaxis().SetRangeUser(0,2000)
#sumEts['h'].GetYaxis().SetRangeUser(0,2000)
#for p in METs.keys():
#  METs[p].GetXaxis().SetTitle("53X MET from "+p)
#  METs[p].GetYaxis().SetTitle("73X MET from "+p)
#  METs[p].GetYaxis().SetLabelSize(0.04)
#  METs[p].GetXaxis().SetLabelSize(0.04)
#  METs[p].GetXaxis().SetRangeUser(0,800)
#  METs[p].GetYaxis().SetRangeUser(0,800)
#
#c1 = ROOT.TCanvas()
#met.Draw('colz')
#c1.Print(printDir+'/'+'met.png')
#c1.Print(printDir+'/'+'met.pdf')
#metPhi.Draw('colz')
#c1.Print(printDir+'/'+'metPhi.png')
#c1.Print(printDir+'/'+'metPhi.pdf')
#metPhi50.Draw('colz')
#c1.Print(printDir+'/'+'metPhi50.png')
#c1.Print(printDir+'/'+'metPhi50.pdf')
#sumEt.Draw('colz')
#c1.Print(printDir+'/'+'sumEt.png')
#c1.Print(printDir+'/'+'sumEt.pdf')
#for p in sumEts:
#  sumEts[p].Draw('COLZ')
#  c1.Print(printDir+'/'+p+'_sumEt.png')
#  c1.Print(printDir+'/'+p+'_sumEt.pdf')
#for p in METs:
#  METs[p].Draw('COLZ')
#  c1.Print(printDir+'/'+p+'_MET.png')
#  c1.Print(printDir+'/'+p+'_MET.pdf')
#
###
####  if run not in runs:
####    runs.append(run)
####    print "Added run",run
####  if run==208352:
####    nf = fileNames[events.fileIndex()]
####    if nf not in usedFiles:
####      print "Found file",run,nf
####      usedFiles.append(nf)
###
####  print ":".join([str(x) for x in [event, run, lumi]] ),products['pfMet'][0].pt(),  products['pfMet'][0].sumEt()
