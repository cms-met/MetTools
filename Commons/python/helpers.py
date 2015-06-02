def translatePdgIdToType(pdg): 
  apdg = abs(pdg)
  if apdg==211: return 1
  if apdg==11:  return 2
  if apdg==13:  return 3
  if apdg==22:  return 4
  if apdg==130: return 5
  if apdg==1:   return 6
  if apdg==2:   return 7
  return 0

    

