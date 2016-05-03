from math import pi

pfTypes = ["X", "h", "e", "mu", "gamma", "h0", "h_HF", "egamma_HF"]
#Translation of pfCandidate enum to labels and vice versa
label = {"X":0,"h":1, "e":2, "mu":3,"gamma":4, 'h0':5, 'h_HF':6, 'egamma_HF':7, 0:"X",1:"h", 2:"e", 3:"mu",4:"gamma", 5:'h0', 6:'h_HF', 7:'egamma_HF'}
#Translation of variable type and vice versa 
varTypes = ["multiplicity","nvtx","sumPt"]
varType = {"multiplicity":0, "nvtx":1, "sumPt":2, 0:"multiplicity",1:"nvtx", 2:"sumPt"}

#Define categories for MET phi corrections below
#h                 = {'name':'h', 'type':'h',  'etaPhiBinning':[108,-2.7,2.7,160,-pi,pi]}
hEtaPlus          = {'name':'hEtaPlus', 'type':'h',  'etaPhiBinning':[54,0,2.7,160,-pi,pi]}
hEtaMinus         = {'name':'hEtaMinus', 'type':'h', 'etaPhiBinning':[54,-2.7,0,160,-pi,pi]}
h0Barrel          = {'name':'h0Barrel', 'type':'h0',  'etaPhiBinning':[32,-1.392,1.392,72,-pi,pi]}
h0EndcapPlus      = {'name':'h0EndcapPlus', 'type':'h0',  'etaPhiBinning':[12,1.392,3,36/2,-pi,pi]}
h0EndcapMinus     = {'name':'h0EndcapMinus', 'type':'h0',  'etaPhiBinning':[12,-3.000, -1.392,36/2,-pi,pi]}
gammaBarrel       = {'name':'gammaBarrel', 'type':'gamma',  'etaPhiBinning':[2*85,-1.479,1.479,360,-pi,pi]}
gammaEndcapPlus   = {'name':'gammaEndcapPlus', 'type':'gamma',  'etaPhiBinning':[20,1.479,3.,30,-pi,pi]}
gammaEndcapMinus  = {'name':'gammaEndcapMinus', 'type':'gamma',  'etaPhiBinning':[20,-3.,-1.479, 30,-pi,pi]}
gammaForwardPlus  = {'name':'gammaForwardPlus', 'type':'gamma',  'etaPhiBinning':[20,3.,5.,30,-pi,pi]}
gammaForwardMinus = {'name':'gammaForwardMinus', 'type':'gamma',  'etaPhiBinning':[20,-5.,-3., 30,-pi,pi]}
e                 = {'name':'e', 'type':'e',  'etaPhiBinning':[108/4,-2.7,2.7,160/4,-pi,pi]}
mu                = {'name':'mu','type':'mu', 'etaPhiBinning':[108/4,-2.4,2.4,160/4,-pi,pi]}

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
#h['binning']                              = {'multiplicity':[5*50,0,5*1500] ,'sumPt':  [5*50,0,5*1500],'nvtx':[50, 0, 50]}
hEtaPlus['binning']                       = {'multiplicity':[5*50,0,5*1500] ,'sumPt':  [5*50,0,5*1500],'nvtx':[50, 0, 50]}
hEtaMinus['binning']                      = {'multiplicity':[5*50,0,5*1500] ,'sumPt':  [5*50,0,5*1500],'nvtx':[50, 0, 50]}
h0Barrel['binning']                       = {'multiplicity':[5*50,0,5*50]   ,'sumPt':  [5*50,0,5*50]  ,'nvtx':[50, 0, 50]}
h0EndcapPlus['binning']                   = {'multiplicity':[5*50,0,5*50]   ,'sumPt':  [5*50,0,5*50]  ,'nvtx':[50, 0, 50]}
h0EndcapMinus['binning']                  = {'multiplicity':[5*50,0,5*50]   ,'sumPt':  [5*50,0,5*50]  ,'nvtx':[50, 0, 50]}
gammaBarrel['binning']                    = {'multiplicity':[5*25,0,5*500]  ,'sumPt':  [5*25,0,5*500] ,'nvtx':[50, 0, 50]}
gammaEndcapPlus['binning']                = {'multiplicity':[5*25,0,5*150]  ,'sumPt':  [5*25,0,5*150] ,'nvtx':[50, 0, 50]}
gammaEndcapMinus['binning']               = {'multiplicity':[5*25,0,5*150]  ,'sumPt':  [5*25,0,5*150] ,'nvtx':[50, 0, 50]}
gammaForwardPlus['binning']               = {'multiplicity':[5*10,0,5*10]   ,'sumPt':  [5*10,0,5*10]  ,'nvtx':[50, 0, 50]}
gammaForwardMinus['binning']              = {'multiplicity':[5*10,0,5*10]   ,'sumPt':  [5*10,0,5*10]  ,'nvtx':[50, 0, 50]}
e['binning']                              = {'multiplicity':[5*10,0,5*10]   ,'sumPt':  [5*10,0,5*10]  ,'nvtx':[50, 0, 50]}
mu['binning']                             = {'multiplicity':[5*10,0,5*10]   ,'sumPt':  [5*10,0,5*10]  ,'nvtx':[50, 0, 50]}
h_HF_Minus['binning']                     = {'multiplicity':[5*25,0,5*250]  ,'sumPt':  [5*25,0,5*250] ,'nvtx':[50, 0, 50]}
h_HF_Plus['binning']                      = {'multiplicity':[5*25,0,5*250]  ,'sumPt':  [5*25,0,5*250] ,'nvtx':[50, 0, 50]}
egamma_HF_Minus['binning']                = {'multiplicity':[5*25,0,5*250]  ,'sumPt':  [5*25,0,5*250] ,'nvtx':[50, 0, 50]}
egamma_HF_Plus['binning']                 = {'multiplicity':[5*25,0,5*250]  ,'sumPt':  [5*25,0,5*250] ,'nvtx':[50, 0, 50]}


allMaps = [hEtaPlus, hEtaMinus, h0Barrel, h0EndcapPlus, h0EndcapMinus, gammaBarrel, gammaEndcapPlus, gammaEndcapMinus, gammaForwardPlus, gammaForwardMinus, e, 
           h_HF_Minus, h_HF_Plus, \
           egamma_HF_Minus, egamma_HF_Plus, 
          ]

