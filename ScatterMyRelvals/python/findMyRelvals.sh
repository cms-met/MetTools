#!/bin/sh
das_client.py --query="dataset dataset=/RelValTTbar/*/*-RECO" --limit=1000|grep CMSSW>& $1.txt
