#! /bin/bash

INPUTDIR=/data/blue/sabrandt/Bacon/wz_flat_2015D


# muons
 #use fitRecoilZmm.C+(inputFile, nSigma for U1 fits, nSigma for U2 fits, 1, name of u1 variable, name of u2 variable, name appended to output file, output folder)
 root -l -q fitRecoil.C+\(\"${INPUTDIR}/Zmumu/ntuples/zmm_select.raw.root\",2,2,1,\"u1\",\"u2\",\"pf\",\"Zmm_test\"\) 

rm *.so *.d *.pcm
