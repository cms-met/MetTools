from math import pi

# Met bin to check the Met profile for each Met range
MetBin=6

pfTypes = ["X", "h", "e", "mu", "gamma", "h0", "h_HF", "egamma_HF"]
#Translation of pfCandidate enum to labels and vice versa
label = {"X":0,"h":1, "e":2, "mu":3,"gamma":4, 'h0':5, 'h_HF':6, 'egamma_HF':7, 'AllPtc':8, 0:"X",1:"h", 2:"e", 3:"mu",4:"gamma", 5:'h0', 6:'h_HF', 7:'egamma_HF', 8:'AllPtc'}
#Translation of variable type and vice versa 
#varTypes = ["ptcMet"]
varTypes = ["multiplicity","ngoodVertices","sumPt","pfType1", "ptcMet", "ptValence", "metPhiNvtx"]
varType = {"multiplicity":0, "ngoodVertices":1, "sumPt":2, "pfType1":3, "ptcMet":4, "ptValence":5, "metPhiNvtx":6, 0:"multiplicity",1:"ngoodVertices", 2:"sumPt", 3:"pfType1", 4:"ptcMet", 5:"ptValence", 6:"metPhiNvtx"} # keep the order

#Define categories for MET phi corrections below
#h                 = {'name':'h', 'type':'h',  'etaPhiBinning':[108,-2.7,2.7,160,-pi,pi]}
hEtaPlus          = {'name':'hEtaPlus', 'type':'h',  'etaPhiBinning':[54,0,2.7,160,-pi,pi]}
hEtaMinus         = {'name':'hEtaMinus', 'type':'h', 'etaPhiBinning':[54,-2.7,0,160,-pi,pi]}
h0Barrel          = {'name':'h0Barrel', 'type':'h0',  'etaPhiBinning':[54,-1.392,1.392,54,-pi,pi]}
h0EndcapPlus      = {'name':'h0EndcapPlus', 'type':'h0',  'etaPhiBinning':[54,1.392,3,54,-pi,pi]}
h0EndcapMinus     = {'name':'h0EndcapMinus', 'type':'h0',  'etaPhiBinning':[54,-3.000, -1.392,54,-pi,pi]}
gammaBarrel       = {'name':'gammaBarrel', 'type':'gamma',  'etaPhiBinning':[54,-1.479,1.479,54,-pi,pi]}
gammaEndcapPlus   = {'name':'gammaEndcapPlus', 'type':'gamma',  'etaPhiBinning':[54,1.479,3.,54,-pi,pi]}
gammaEndcapMinus  = {'name':'gammaEndcapMinus', 'type':'gamma',  'etaPhiBinning':[54,-3.,-1.479, 54,-pi,pi]}
gammaForwardPlus  = {'name':'gammaForwardPlus', 'type':'gamma',  'etaPhiBinning':[20,3.,5.,30,-pi,pi]}
gammaForwardMinus = {'name':'gammaForwardMinus', 'type':'gamma',  'etaPhiBinning':[20,-5.,-3., 30,-pi,pi]}
e                 = {'name':'e', 'type':'e',  'etaPhiBinning':[108/4,-2.7,2.7,160/4,-pi,pi]}
mu                = {'name':'mu','type':'mu', 'etaPhiBinning':[108/4,-2.4,2.4,160/4,-pi,pi]}
AllPtc            = {'name':'AllPtc','type':'AllPtc', 'etaPhiBinning':[108/4,-5,5,160/4,-pi,pi]}

etaMaxDepth1 = 5.2
etaMinDepth1 = 2.901376
nEtaBinsHF = 30
egammaHFMinus        = {'name':'egamma_HFMinus', 'type':'egamma_HF',  'etaPhiBinning':[nEtaBinsHF,-etaMaxDepth1,-etaMinDepth1,30,-pi,pi]}
egammaHFPlus         = {'name':'egamma_HFPlus', 'type':'egamma_HF',   'etaPhiBinning':[nEtaBinsHF,etaMinDepth1,etaMaxDepth1,30,-pi,pi]}

etaMaxDepth1 = 5.2
etaMinDepth1 = 2.901376
nEtaBinsHF = 13
hHFMinus        = {'name':'h_HFMinus', 'type':'h_HF',  'etaPhiBinning':[nEtaBinsHF,-etaMaxDepth1,-etaMinDepth1,18,-pi,pi]}
hHFPlus         = {'name':'h_HFPlus', 'type':'h_HF',   'etaPhiBinning':[nEtaBinsHF,etaMinDepth1,etaMaxDepth1,18,-pi,pi]}


#variable 0 (candidate multiplicity): specify binning
#h['binning']                 = {'multiplicity':[5*50,0,5*1500] ,'sumPt':  [5*50,0,5*1500],'ngoodVertices':[50, 0, 50]}
hEtaPlus['binning']           = {'multiplicity':[300,0,900] ,'sumPt':[200,0,1000],'ngoodVertices':[50, 0, 50],'pfType1':[100,0,300],'ptcMet':[300,0,300]}
hEtaMinus['binning']          = {'multiplicity':[300,0,900] ,'sumPt':[200,0,1000],'ngoodVertices':[50, 0, 50],'pfType1':[100,0,300],'ptcMet':[300,0,300]}
h0Barrel['binning']           = {'multiplicity':[50,0,50]   ,'sumPt':[200,0,200] ,'ngoodVertices':[50, 0, 50],'pfType1':[100,0,300],'ptcMet':[300,0,300]}
h0EndcapPlus['binning']       = {'multiplicity':[50,0,50]   ,'sumPt':[200,0,200] ,'ngoodVertices':[50, 0, 50],'pfType1':[100,0,300],'ptcMet':[300,0,300]}
h0EndcapMinus['binning']      = {'multiplicity':[50,0,50]   ,'sumPt':[200,0,200] ,'ngoodVertices':[50, 0, 50],'pfType1':[100,0,300],'ptcMet':[300,0,300]}
gammaBarrel['binning']        = {'multiplicity':[500,0,500] ,'sumPt':[100,0,700] ,'ngoodVertices':[50, 0, 50],'pfType1':[100,0,300],'ptcMet':[300,0,300]}
gammaEndcapPlus['binning']    = {'multiplicity':[150,0,150] ,'sumPt':[100,0,300] ,'ngoodVertices':[50, 0, 50],'pfType1':[100,0,300],'ptcMet':[300,0,300]}
gammaEndcapMinus['binning']   = {'multiplicity':[150,0,150] ,'sumPt':[100,0,300] ,'ngoodVertices':[50, 0, 50],'pfType1':[100,0,300],'ptcMet':[300,0,300]}
gammaForwardPlus['binning']   = {'multiplicity':[10,0,10]   ,'sumPt':[25,0,25]   ,'ngoodVertices':[50, 0, 50],'pfType1':[100,0,300],'ptcMet':[300,0,300]}
gammaForwardMinus['binning']  = {'multiplicity':[10,0,10]   ,'sumPt':[25,0,25]   ,'ngoodVertices':[50, 0, 50],'pfType1':[100,0,300],'ptcMet':[300,0,300]}
e['binning']                  = {'multiplicity':[10,0,10]   ,'sumPt':[100,0,100] ,'ngoodVertices':[50, 0, 50],'pfType1':[100,0,300],'ptcMet':[300,0,100]}
mu['binning']                 = {'multiplicity':[15,0,15]   ,'sumPt':[100,0,100] ,'ngoodVertices':[50, 0, 50],'pfType1':[100,0,300],'ptcMet':[300,0,300]}
hHFMinus['binning']           = {'multiplicity':[100,0,400] ,'sumPt':[100,0,600] ,'ngoodVertices':[50, 0, 50],'pfType1':[100,0,300],'ptcMet':[300,0,300]}
hHFPlus['binning']            = {'multiplicity':[100,0,400] ,'sumPt':[100,0,600] ,'ngoodVertices':[50, 0, 50],'pfType1':[100,0,300],'ptcMet':[300,0,300]}
egammaHFMinus['binning']      = {'multiplicity':[100,0,200] ,'sumPt':[100,0,200] ,'ngoodVertices':[50, 0, 50],'pfType1':[100,0,300],'ptcMet':[300,0,300]}
egammaHFPlus['binning']       = {'multiplicity':[100,0,200] ,'sumPt':[100,0,200] ,'ngoodVertices':[50, 0, 50],'pfType1':[100,0,300],'ptcMet':[300,0,300]}



# For Standard fitting
usedMaps = [hEtaPlus, hEtaMinus, h0Barrel, h0EndcapPlus, h0EndcapMinus, \
            gammaBarrel, gammaEndcapPlus, gammaEndcapMinus, \
            hHFPlus, hHFMinus, \
            egammaHFPlus, egammaHFMinus,
          ]

# all of elements
allMaps = [hEtaPlus, hEtaMinus, h0Barrel, h0EndcapPlus, h0EndcapMinus, \
           gammaBarrel, gammaEndcapPlus, gammaEndcapMinus, \
           hHFPlus, hHFMinus, \
           egammaHFPlus, egammaHFMinus, \
	   e, mu, \
           gammaForwardPlus, gammaForwardMinus,
          ]
