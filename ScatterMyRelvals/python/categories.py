from math import pi

pfTypes = ["X", "h", "e", "mu", "gamma", "h0", "h_HF", "egamma_HF"]
#Translation of pfCandidate enum to labels and vice versa
label = {"X":0,"h":1, "e":2, "mu":3,"gamma":4, 'h0':5, 'h_HF':6, 'egamma_HF':7, 0:"X",1:"h", 2:"e", 3:"mu",4:"gamma", 5:'h0', 6:'h_HF', 7:'egamma_HF'}
#Translation of variable type and vice versa 
varTypes = ["multiplicity","nvtx","sumPt"]
varType = {"multiplicity":0, "nvtx":1, "sumPt":2, 0:"multiplicity",1:"nvtx", 2:"sumPt"}

#Define categories for MET phi corrections below
h                 = {'name':'h', 'type':'h',  'etaRange':[0,2.7]}
h0Barrel          = {'name':'h0Barrel', 'type':'h0',  'etaRange':[0,1.392]}
h0Endcap      = {'name':'h0EndcapPlus', 'type':'h0',  'etaRange':[1.392,3]}
gammaBarrel       = {'name':'gammaBarrel', 'type':'gamma',  'etaRange':[0,1.479]}
gammaEndcap   = {'name':'gammaEndcapPlus', 'type':'gamma',  'etaRange':[1.479,3.]}
gammaForward  = {'name':'gammaForwardPlus', 'type':'gamma',  'etaRange':[3.,5.]}
e                 = {'name':'e', 'type':'e',  'etaRange':[0,5]}
mu                = {'name':'mu','type':'mu', 'etaRange':[0,5]}

etaMaxDepth1 = 5.2
etaMinDepth1 = 2.901376
egamma_HF         = {'name':'egamma_HFPlus', 'type':'egamma_HF',   'etaRange':[etaMinDepth1,etaMaxDepth1]}

etaMaxDepth1 = 5.2
etaMinDepth1 = 2.901376
h_HF         = {'name':'h_HFPlus', 'type':'h_HF',   'etaRange':[etaMinDepth1,etaMaxDepth1]}



allCategories = [h, h0Barrel, h0Endcap, gammaBarrel, gammaEndcap, gammaForward, e, h_HF, mu, egamma_HF   ]

