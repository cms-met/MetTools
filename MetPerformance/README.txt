--- How to run scriptcompare ----

Copy the sample file from:
/eos/cms/store/cmst3/user/dalfonso/MET/DY_phys14.root

Then in a root session:

.L scriptcompare.C
scriptcompare("input1.root" ,"input2.root", "pfmet")

For this test input1.root=input2.root=DY_phys14.root

This will produce the comparison plot for the pfmet variables in the file1.root and file2.root

In principle the histogram with the comparison will be saved in the current working directory. This can be changed at the end of the script.
