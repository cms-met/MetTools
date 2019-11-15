import FWCore.ParameterSet.Config as cms
multPhiCorr_Data_2017B = cms.VPSet(
    cms.PSet(
      name=cms.string("hEtaPlus"),
      type=cms.int32(1),
      varType=cms.int32(1),
      etaMin=cms.double(0),
      etaMax=cms.double(2.7),
      fx=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      px=cms.vdouble(0.281249114131,-0.213729275278,-0.000570761374153),
      fy=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      py=cms.vdouble(-0.618215532454,0.254366842309,0.000720216178231),
    ),
    cms.PSet(
      name=cms.string("hEtaMinus"),
      type=cms.int32(1),
      varType=cms.int32(1),
      etaMin=cms.double(-2.7),
      etaMax=cms.double(0),
      fx=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      px=cms.vdouble(0.83624671858,-0.0652952186328,-0.000800857416981),
      fy=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      py=cms.vdouble(-0.311231486233,0.0389956639429,0.000801906579575),
    ),
    cms.PSet(
      name=cms.string("h0Barrel"),
      type=cms.int32(5),
      varType=cms.int32(1),
      etaMin=cms.double(-1.392),
      etaMax=cms.double(1.392),
      fx=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      px=cms.vdouble(-0.0484091390078,-0.0100033537974,0.000432168280291),
      fy=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      py=cms.vdouble(-0.0136953487703,-0.00883965926692,0.000156515988133),
    ),
    cms.PSet(
      name=cms.string("h0EndcapPlus"),
      type=cms.int32(5),
      varType=cms.int32(1),
      etaMin=cms.double(1.392),
      etaMax=cms.double(3),
      fx=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      px=cms.vdouble(0.45092914992,0.00607119465604,2.89003950643e-05),
      fy=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      py=cms.vdouble(-0.579291341752,0.000641724152125,-0.000314416562466),
    ),
    cms.PSet(
      name=cms.string("h0EndcapMinus"),
      type=cms.int32(5),
      varType=cms.int32(1),
      etaMin=cms.double(-3.0),
      etaMax=cms.double(-1.392),
      fx=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      px=cms.vdouble(-0.204574219161,0.0496557417315,-2.31472681396e-05),
      fy=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      py=cms.vdouble(0.0736253557145,0.0137326615811,-0.000389138367037),
    ),
    cms.PSet(
      name=cms.string("gammaBarrel"),
      type=cms.int32(4),
      varType=cms.int32(1),
      etaMin=cms.double(-1.479),
      etaMax=cms.double(1.479),
      fx=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      px=cms.vdouble(0.0586385088063,-0.00526415256445,0.000181328043507),
      fy=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      py=cms.vdouble(-0.0421162530437,0.00840646857355,-0.000103643192342),
    ),
    cms.PSet(
      name=cms.string("gammaEndcapPlus"),
      type=cms.int32(4),
      varType=cms.int32(1),
      etaMin=cms.double(1.479),
      etaMax=cms.double(3.0),
      fx=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      px=cms.vdouble(-0.0341092953197,0.00182611844558,-0.000134844366988),
      fy=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      py=cms.vdouble(-0.0187686447096,0.00277102541184,-0.000179236582536),
    ),
    cms.PSet(
      name=cms.string("gammaEndcapMinus"),
      type=cms.int32(4),
      varType=cms.int32(1),
      etaMin=cms.double(-3.0),
      etaMax=cms.double(-1.479),
      fx=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      px=cms.vdouble(-0.0293634853937,0.00198931509938,-5.36318306133e-05),
      fy=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      py=cms.vdouble(-0.0208098254995,0.00351248440624,-0.000111551563709),
    ),
    cms.PSet(
      name=cms.string("hHFPlus"),
      type=cms.int32(6),
      varType=cms.int32(1),
      etaMin=cms.double(2.901376),
      etaMax=cms.double(5.2),
      fx=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      px=cms.vdouble(0.135124313642,-0.00120378161567,0.00014315794846),
      fy=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      py=cms.vdouble(0.235219287088,0.0320243433311,0.000216004873749),
    ),
    cms.PSet(
      name=cms.string("hHFMinus"),
      type=cms.int32(6),
      varType=cms.int32(1),
      etaMin=cms.double(-5.2),
      etaMax=cms.double(-2.901376),
      fx=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      px=cms.vdouble(-0.537945781305,0.0982829644858,-0.000483500988429),
      fy=cms.string("([p0]+x*[p1]+pow(x,2)*[p2])"),
      py=cms.vdouble(-0.020079436239,-0.0378031309243,0.000231740990574),
    ),
    cms.PSet(
      name=cms.string("egammaHFPlus"),
      type=cms.int32(7),
      varType=cms.int32(1),
      etaMin=cms.double(2.901376),
      etaMax=cms.double(5.2),
      fx=cms.string("([p0]+(x*(x<100)+100*(x>=100))*[p1]+pow((x*(x<100)+100*(x>=100)),2)*[p2])"),
      px=cms.vdouble(0.0213749066644,0.0304929396277,-0.000297082632293),
      fy=cms.string("([p0]+(x*(x<100)+100*(x>=100))*[p1]+pow((x*(x<100)+100*(x>=100)),2)*[p2])"),
      py=cms.vdouble(-0.0277331284307,-0.018687840485,0.000204233718189),
    ),
    cms.PSet(
      name=cms.string("egammaHFMinus"),
      type=cms.int32(7),
      varType=cms.int32(1),
      etaMin=cms.double(-5.2),
      etaMax=cms.double(-2.901376),
      fx=cms.string("([p0]+(x*(x<100)+100*(x>=100))*[p1]+pow((x*(x<100)+100*(x>=100)),2)*[p2])"),
      px=cms.vdouble(0.214082398099,-0.0223215698502,0.000126757351401),
      fy=cms.string("([p0]+(x*(x<100)+100*(x>=100))*[p1]+pow((x*(x<100)+100*(x>=100)),2)*[p2])"),
      py=cms.vdouble(0.332517192036,-0.013698227718,0.000666514667607),
    ),
)