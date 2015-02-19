Copy the sample file from:
/eos/cms/store/cmst3/user/dalfonso/MET/DY_phys14.root

#### How to run scriptcompare 

```bash
.L scriptcompare.C
scriptcompare("input1.root" ,"input2.root", "pfmet")
```

For this test input1.root=input2.root=DY_phys14.root

This will produce the comparison plot for the pfmet variables in the file1.root and file2.root

In principle the histogram with the comparison will be saved in the current working directory. This can be changed at the end of the script.


### How to run metresolution 

```bash
.L metresolution.C+
metresolution("input.root","pfmetx")
```
This generates the sigma(pfmetx) vs the sumET of the event (this will be customizable in next versions) and plots it in a tgraph. It also generates the png with the result of the fits per bin so that one can inspect them easily.
