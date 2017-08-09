import sys, re, os, os.path


file_list = []
files = os.listdir('./')
for filename in files:
  if 'cmdBatch' in filename:
    cmd = 'qsub -q short '+filename
    print cmd
    os.system(cmd)
