

__all__ = ['recordable']

recodable_keys = []

def recordable( key:str , container: str) -> str:
  name = (container+'_'+key)   
  if name in recodable_keys:
    raise RuntimeError(f"Key {name} repeated. please use another key.")
  recodable_keys.append(name)
  return name
