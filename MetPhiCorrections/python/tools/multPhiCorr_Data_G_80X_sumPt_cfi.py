import FWCore.ParameterSet.Config as cms
multPhiCorr_Data_G_80X = cms.VPSet(
    cms.PSet(
      name=cms.string("hEtaPlus"),
      type=cms.int32(1),
      varType=cms.int32(2),
      etaMin=cms.double(0),
      etaMax=cms.double(2.7),
      fx=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      px=cms.vdouble(0.00392854289318,-8.85865967333e-07),
      fy=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      py=cms.vdouble(0.00152453076311,-4.1582480199e-06),
    ),
    cms.PSet(
      name=cms.string("hEtaMinus"),
      type=cms.int32(1),
      varType=cms.int32(2),
      etaMin=cms.double(-2.7),
      etaMax=cms.double(0),
      fx=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      px=cms.vdouble(-0.00858564393718,4.7414820016e-08),
      fy=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      py=cms.vdouble(0.00277893330237,-6.18480496813e-06),
    ),
    cms.PSet(
      name=cms.string("h0Barrel"),
      type=cms.int32(5),
      varType=cms.int32(2),
      etaMin=cms.double(-1.392),
      etaMax=cms.double(1.392),
      fx=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      px=cms.vdouble(-0.0155633407674,0.000126891547396),
      fy=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      py=cms.vdouble(-0.010822444451,-0.000465684858077),
    ),
    cms.PSet(
      name=cms.string("h0EndcapPlus"),
      type=cms.int32(5),
      varType=cms.int32(2),
      etaMin=cms.double(1.392),
      etaMax=cms.double(3),
      fx=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      px=cms.vdouble(-0.0208724884932,0.000116203773553),
      fy=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      py=cms.vdouble(0.0223022711136,-6.91451760451e-05),
    ),
    cms.PSet(
      name=cms.string("h0EndcapMinus"),
      type=cms.int32(5),
      varType=cms.int32(2),
      etaMin=cms.double(-3.0),
      etaMax=cms.double(-1.392),
      fx=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      px=cms.vdouble(-0.00925344218842,0.000603978077496),
      fy=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      py=cms.vdouble(0.0349558614173,-0.000481151540987),
    ),
    cms.PSet(
      name=cms.string("gammaBarrel"),
      type=cms.int32(4),
      varType=cms.int32(2),
      etaMin=cms.double(-1.479),
      etaMax=cms.double(1.479),
      fx=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      px=cms.vdouble(0.000104447203972,-1.04594157718e-05),
      fy=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      py=cms.vdouble(7.55201875447e-05,-7.56649453041e-06),
    ),
    cms.PSet(
      name=cms.string("gammaEndcapPlus"),
      type=cms.int32(4),
      varType=cms.int32(2),
      etaMin=cms.double(1.479),
      etaMax=cms.double(3.0),
      fx=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      px=cms.vdouble(-0.00184276296696,-0.00024369363316),
      fy=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      py=cms.vdouble(0.00966802421801,5.02669902731e-06),
    ),
    cms.PSet(
      name=cms.string("gammaEndcapMinus"),
      type=cms.int32(4),
      varType=cms.int32(2),
      etaMin=cms.double(-3.0),
      etaMax=cms.double(-1.479),
      fx=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      px=cms.vdouble(0.00631551958846,-9.84287094307e-06),
      fy=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      py=cms.vdouble(-0.00165282140963,8.06855600175e-05),
    ),
    cms.PSet(
      name=cms.string("hHFPlus"),
      type=cms.int32(6),
      varType=cms.int32(2),
      etaMin=cms.double(2.901376),
      etaMax=cms.double(5.2),
      fx=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      px=cms.vdouble(0.0242485864131,-3.39714522154e-05),
      fy=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      py=cms.vdouble(0.00137443339963,-4.01273222732e-06),
    ),
    cms.PSet(
      name=cms.string("hHFMinus"),
      type=cms.int32(6),
      varType=cms.int32(2),
      etaMin=cms.double(-5.2),
      etaMax=cms.double(-2.901376),
      fx=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      px=cms.vdouble(0.0230324709823,-7.71723235156e-06),
      fy=cms.string("(x*[p0]+pow(x,2)*[p1])"),
      py=cms.vdouble(-0.000976574453518,-2.38022699877e-06),
    ),
    cms.PSet(
      name=cms.string("egammaHFPlus"),
      type=cms.int32(7),
      varType=cms.int32(2),
      etaMin=cms.double(2.901376),
      etaMax=cms.double(5.2),
      fx=cms.string("((x*(x<100)+100*(x>=100))*[p0]+pow((x*(x<100)+100*(x>=100)),2)*[p1])"),
      px=cms.vdouble(-0.00167068294455,0.0004459348738),
      fy=cms.string("((x*(x<100)+100*(x>=100))*[p0]+pow((x*(x<100)+100*(x>=100)),2)*[p1])"),
      py=cms.vdouble(-0.0084261762869,5.35130057806e-05),
    ),
    cms.PSet(
      name=cms.string("egammaHFMinus"),
      type=cms.int32(7),
      varType=cms.int32(2),
      etaMin=cms.double(-5.2),
      etaMax=cms.double(-2.901376),
      fx=cms.string("((x*(x<100)+100*(x>=100))*[p0]+pow((x*(x<100)+100*(x>=100)),2)*[p1])"),
      px=cms.vdouble(0.00187412516968,-0.000374816614854),
      fy=cms.string("((x*(x<100)+100*(x>=100))*[p0]+pow((x*(x<100)+100*(x>=100)),2)*[p1])"),
      py=cms.vdouble(-0.000518090826549,0.00010361581075),
    ),
)