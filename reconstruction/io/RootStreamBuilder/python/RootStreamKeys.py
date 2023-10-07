
__all__ = ["RootStreamEDMList", "recordable"]


RootStreamEDMList = [
    # common containers
    ("xAOD::EventInfoContainer#EventInfo"     , ['HIT','ESD','AOD'], "Event"    )
    ("xAOD::EventSeedContainer#Seeds"         , ['HIT','ESD','AOD'], "Event"    )
    ("xAOD::TruthParticleContainer#Particles" , ['HIT','ESD','AOD'], "Truth"    )
    ("xAOD::CaloCellContainer#Cells"          , ['ESD','AOD']      , "Cells"    )
    ("xAOD::CaloCellContainer#TruthCells"     , ['ESD','AOD']      , "Cells"    )
    ("xAOD::CaloHitContainer#Hits"            , ['HIT']            , "Hits"     )
    ("xAOD::CaloClusterContainer#Clusters"    , ['ESD', 'AOD']     , "Clusters" )
    ("xAOD::CaloRingsContainer#Rings"         , ['AOD']            , "Rings"    )
]


def recordable( key: str ) -> str:
    if key not in [stream[0].split('#')[1] for stream in RootStreamEDMList]:
        raise RuntimeError (f"Key {key} not inside of EDM stream list.")
  return key

