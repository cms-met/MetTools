# Performing a new MVA MET training

A training needs three two:
1. Creating a training n-tuple on the grid
2. Performing the training itself on a local machine

#### 1. Creating the training n-tuple

The MVAMET plugin internally stores all input variables in a map. By setting

```bash
process.MVAMET.saveMap = cms.bool(True)
```
the MVAMET plugin writes out a vector of strings and a vector of floats that can be writte out to a root n-tuple with the MAPAnalyzer. An example can be found in test/makeMVAMETTrainingNtuple.py.

Since most DY samples are inclusive, it's important to select only genuine Z to mu-mu events on generator level. This is done with the GenParticleSelector.

Add all output files with hadd. A training sample should have O(10^6) events.

#### 2. Performing the training
The GBRTrain implementation can be run very fast on a multicore machine. This works very well on modern Intel machines. Be aware that on AMD machines this does not work. To activate multi-core support uncomment the corresponding line in BuildFile.xml

The training itself is steered by the json file configs/config.json.

The four trainings are performed one after the other, while directly being evaluated. This is necessary because they depend on each other.

The training result is stored in the file specified in the "weightfile" section in the json config.

A typical training takes O(1h) with multicore being used and several hours in single-core mode.

Run the training with

```bash
regress3 configs/config.json
```
