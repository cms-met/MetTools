import sys, re, os, os.path


file_list = []
files = os.listdir('./')
for filename in files:
  if 'cmdBatch' and 'sh' in filename:
    cmd = 'qsub -q cms '+filename
    print cmd
    os.system(cmd)
