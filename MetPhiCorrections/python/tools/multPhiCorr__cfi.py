import FWCore.ParameterSet.Config as cms
multPhiCorr_ = cms.VPSet(
    cms.PSet(
      name=cms.string("h"),
      type=cms.int32(1),
      varType=cms.int32(0),
      etaMin=cms.double(-2.7),
      etaMax=cms.double(2.7),
      fx=cms.string("(x*[0])+(sq(x)*[1])"),
      px=cms.vdouble(-0.00100488080903,-2.52390571355e-07),
      fy=cms.string("(x*[0])+(sq(x)*[1])"),
      py=cms.vdouble(-0.00861386123702,4.62378996583e-07),
    ),
)
