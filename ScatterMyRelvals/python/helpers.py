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

def save(object, filename, protocol = -1):
  import cPickle, gzip
  """Save an object to a compressed disk file.
     Works well with huge objects.
  """
  file = gzip.GzipFile(filename, 'wb')
  cPickle.dump(object, file, protocol)
  file.close()

def load(filename):
  import cPickle, gzip
  """Loads a compressed object from disk
  """
  file = gzip.GzipFile(filename, 'rb')
  object = cPickle.load(file)
  file.close()

  return object

