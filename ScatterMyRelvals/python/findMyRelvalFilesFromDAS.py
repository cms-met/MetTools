import subprocess
import os, pickle

#i=0
f=open('myRelVals.txt','r')
relValFiles = {}
for line in f.readlines():
  longname= line[:-1]
  shortname = longname.split('/')[2]
  print shortname
  p = os.popen('das_client.py --query="file dataset='+longname+'" --limit=0',"r")
  files=[]
  while True:
    l = p.readline()
    if not l: break
    fn=l[:-1]
    if not fn[-5:]=='.root':continue
    files.append('root://eoscms.cern.ch/'+fn)
  print "... found",len(files),'files'
  relValFiles[shortname] =  files

pickle.dump(relValFiles, file("relValFiles.pkl",'w'))
