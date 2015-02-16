from math import pi

pfTypes = ["X", "h", "e", "mu", "gamma", "h0", "h_HF", "egamma_HF"]
label = {"X":0,"h":1, "e":2, "mu":3,"gamma":4, 'h0':5, 'h_HF':6, 'egamma_HF':7, 0:"X",1:"h", 2:"e", 3:"mu",4:"gamma", 5:'h0', 6:'h_HF', 7:'egamma_HF'}

#Define categories for MET phi corrections below
h             = {'name':'h', 'type':'h',  'etaPhiBinning':[108,-2.7,2.7,160,-pi,pi]}
h0Barrel      = {'name':'h0Barrel', 'type':'h0',  'etaPhiBinning':[32,-1.392,1.392,72,-pi,pi]}
h0EndcapPlus  = {'name':'h0EndcapPlus', 'type':'h0',  'etaPhiBinning':[12,1.392,3,36/2,-pi,pi]}
h0EndcapMinus = {'name':'h0EndcapMinus', 'type':'h0',  'etaPhiBinning':[12,-3.000, -1.392,36/2,-pi,pi]}
gammaBarrel      = {'name':'gammaBarrel', 'type':'gamma',  'etaPhiBinning':[2*85,-1.479,1.479,360,-pi,pi]}
gammaEndcapPlus  = {'name':'gammaEndcapPlus', 'type':'gamma',  'etaPhiBinning':[20,1.479,3.,30,-pi,pi]}
gammaEndcapMinus = {'name':'gammaEndcapMinus', 'type':'gamma',  'etaPhiBinning':[20,-3.,-1.479, 30,-pi,pi]}
gammaForwardPlus  = {'name':'gammaForwardPlus', 'type':'gamma',  'etaPhiBinning':[20,3.,5.,30,-pi,pi]}
gammaForwardMinus = {'name':'gammaForwardMinus', 'type':'gamma',  'etaPhiBinning':[20,-5.,-3., 30,-pi,pi]}
e             = {'name':'e', 'type':'e',  'etaPhiBinning':[108/4,-2.7,2.7,160/4,-pi,pi]}
mu            = {'name':'mu','type':'mu', 'etaPhiBinning':[108/4,-2.4,2.4,160/4,-pi,pi]}

etaMaxDepth1 = 5.2
etaMinDepth1 = 2.901376
nEtaBinsHF = 13
egamma_HF_Minus        = {'name':'egamma_HFMinus', 'type':'egamma_HF',  'etaPhiBinning':[nEtaBinsHF,-etaMaxDepth1,-etaMinDepth1,18,-pi,pi]}
egamma_HF_Plus         = {'name':'egamma_HFPlus', 'type':'egamma_HF',   'etaPhiBinning':[nEtaBinsHF,etaMinDepth1,etaMaxDepth1,18,-pi,pi]}

etaMaxDepth1 = 5.2
etaMinDepth1 = 2.901376
nEtaBinsHF = 13
h_HF_Minus        = {'name':'h_HFMinus', 'type':'h_HF',  'etaPhiBinning':[nEtaBinsHF,-etaMaxDepth1,-etaMinDepth1,18,-pi,pi]}
h_HF_Plus         = {'name':'h_HFPlus', 'type':'h_HF',   'etaPhiBinning':[nEtaBinsHF,etaMinDepth1,etaMaxDepth1,18,-pi,pi]}


#variable 0 (candidate multiplicity): specify binning
h['candMultiplicityBinning']                              = [5*50,0,5*1500]
h0Barrel['candMultiplicityBinning']                       = [5*50,0,5*50]
h0EndcapPlus['candMultiplicityBinning']                   = [5*50,0,5*50]
h0EndcapMinus['candMultiplicityBinning']                  = [5*50,0,5*50]
gammaBarrel['candMultiplicityBinning']                    = [5*25,0,5*500]
gammaEndcapPlus['candMultiplicityBinning']                = [5*25,0,5*150]
gammaEndcapMinus['candMultiplicityBinning']               = [5*25,0,5*150]
gammaForwardPlus['candMultiplicityBinning']               = [5*10,0,5*10]
gammaForwardMinus['candMultiplicityBinning']              = [5*10,0,5*10]
e['candMultiplicityBinning']                              = [5*10,0,5*10]
mu['candMultiplicityBinning']                             = [5*10,0,5*10]
h_HF_Minus['candMultiplicityBinning']                     = [5*25,0,5*250]
h_HF_Plus['candMultiplicityBinning']                      = [5*25,0,5*250]
egamma_HF_Minus['candMultiplicityBinning']                = [5*25,0,5*250]
egamma_HF_Plus['candMultiplicityBinning']                 = [5*25,0,5*250]

#variable 1 (vertex multiplicity): specify binning
h['nvtxBinning']                               = [50, 0, 50]
h0Barrel['nvtxBinning']                        = [50, 0, 50]
h0EndcapPlus['nvtxBinning']                    = [50, 0, 50]
h0EndcapMinus['nvtxBinning']                   = [50, 0, 50]
gammaBarrel['nvtxBinning']                     = [50, 0, 50]
gammaEndcapPlus['nvtxBinning']                 = [50, 0, 50]
gammaEndcapMinus['nvtxBinning']                = [50, 0, 50]
gammaForwardPlus['nvtxBinning']                = [50, 0, 50]
gammaForwardMinus['nvtxBinning']               = [50, 0, 50]
e['nvtxBinning']                               = [50, 0, 50]
mu['nvtxBinning']                              = [50, 0, 50]
h_HF_Minus['nvtxBinning']                      = [50, 0, 50]
h_HF_Plus['nvtxBinning']                       = [50, 0, 50]
egamma_HF_Minus['nvtxBinning']                 = [50, 0, 50]
egamma_HF_Plus['nvtxBinning']                  = [50, 0, 50]

#variable 2 (sumET): specify binning
h['sumETBinning']                              = [5*50,0,5*1500]
h0Barrel['sumETBinning']                       = [5*50,0,5*50]
h0EndcapPlus['sumETBinning']                   = [5*50,0,5*50]
h0EndcapMinus['sumETBinning']                  = [5*50,0,5*50]
gammaBarrel['sumETBinning']                    = [5*25,0,5*500]
gammaEndcapPlus['sumETBinning']                = [5*25,0,5*150]
gammaEndcapMinus['sumETBinning']               = [5*25,0,5*150]
gammaForwardPlus['sumETBinning']               = [5*10,0,5*10]
gammaForwardMinus['sumETBinning']              = [5*10,0,5*10]
e['sumETBinning']                              = [5*10,0,5*10]
mu['sumETBinning']                             = [5*10,0,5*10]
h_HF_Minus['sumETBinning']                     = [5*25,0,5*250]
h_HF_Plus['sumETBinning']                      = [5*25,0,5*250]
egamma_HF_Minus['sumETBinning']                = [5*25,0,5*250]
egamma_HF_Plus['sumETBinning']                 = [5*25,0,5*250]

allMaps = [h, h0Barrel, h0EndcapPlus, h0EndcapMinus, gammaBarrel, gammaEndcapPlus, gammaEndcapMinus, gammaForwardPlus, gammaForwardMinus, e, 
           h_HF_Minus, h_HF_Plus, \
           egamma_HF_Minus, egamma_HF_Plus, 
          ]

