
__all__ = ["recordable", "treatPropertyValue"]


from Gaugi import list_to_stdvector


def recordable( key ):

  keys = [
            # CaloCellMaker
            "Collection_EM1",
            "Collection_EM2",
            "Collection_EM3",
            "Collection_HAD1",
            "Collection_HAD2",
            "Collection_HAD3",
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
    return list_to_stdvector('string', value)
  elif (type(value) is list) and (type(value[0]) is int):
    return list_to_stdvector('int', value)
  elif (type(value) is list) and (type(value[0]) is float):
    return list_to_stdvector('float', value)
  elif (type(value) is list) and (type(value[0]) is bool):
    return list_to_stdvector('bool', value)
  # list of list with ints, should be vector<vector<int>>
  elif (type(value) is list) and (type(value[0]) is list) and (type(value[0][0]) is int) :
    from ROOT.std import vector
    vec = vector("vector<int>")()
    for v in value:
      vec.push_back( list_to_stdvector('int', v) )
    return vec
  else:
    return value



