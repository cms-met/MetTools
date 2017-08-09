import sys, re, os, os.path


file_list = []
files = os.listdir('./')
for filename in files:
  if 'cmdBatch' in filename:
    cmd = 'qsub -q cms '+filename
    print cmd
    os.system(cmd)
