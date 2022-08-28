
__all__ = ["recordable", "treatPropertyValue"]


from GaugiKernel import list2stdvector


def recordable( key ):

  keys = [
            # CaloCellMaker
            "Collection_EM1",
            "Collection_EM2",
            "Collection_EM3",
            "Collection_HAD1",
            "Collection_HAD2",
            "Collection_HAD3",
            "Hits",
            # CaloCellMerge
            "Cells",
            "TruthCells",
            # CaloClusterMaker
            "EventInfo",
            "Clusters",
            "TruthClusters",
            "Particles",
            "TruthParticles",
            # CaloRingerBuilder
            "Rings",
            "TruthRings",
            ]



  #if key in keys:
  #  return key
  #else:
  #  RuntimeError("The key %s it's not allow to the accetable key list. Please check the key.")

  return key


def treatPropertyValue( value ):

  if (type(value) is list) and (type(value[0]) is str):
    return list2stdvector('string', value)
  elif (type(value) is list) and (type(value[0]) is int):
    return list2stdvector('int', value)
  elif (type(value) is list) and (type(value[0]) is float):
    return list2stdvector('float', value)
  elif (type(value) is list) and (type(value[0]) is bool):
    return list2stdvector('bool', value)
  # list of list with ints, should be vector<vector<int>>
  elif (type(value) is list) and (type(value[0]) is list) and (type(value[0][0]) is int) :
    from ROOT.std import vector
    vec = vector("vector<int>")()
    for v in value:
      vec.push_back( list2stdvector('int', v) )
    return vec
  else:
    return value



