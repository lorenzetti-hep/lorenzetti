
__all__ = ["RootStreamEDMList", "recordable"]

from RootStreamBuilder import RootStreamFlags as flags

RootStreamEDMList = [
    # namespace::container#key (unique) , EDMs , key dependence
    ("xAOD::EventInfoContainer#EventInfo"     , ['HIT','ESD','AOD'], []            ),
    ("xAOD::EventSeedContainer#Seeds"         , ['HIT','ESD','AOD'], []            ),
    ("xAOD::TruthParticleContainer#Particles" , ['HIT','ESD','AOD'], []            ),
    ("xAOD::CaloCellContainer#Cells"          , ['ESD','AOD']      , []            ),
    ("xAOD::CaloCellContainer#TruthCells"     , ['ESD','AOD']      , []            ),
    ("xAOD::CaloHitContainer#Hits"            , ['HIT']            , ['Seeds']     ),
    ("xAOD::CaloClusterContainer#Clusters"    , ['ESD', 'AOD']     , ['Cells']     ),
    ("xAOD::CaloRingsContainer#Rings"         , ['AOD']            , []            ),
]

RootStreamConverters = [

    EventInfoConverter( key="EventInfo" ),
    EventSeedConverter( key="Seeds" ),
    TruthParticleConverter( key="Particles" ),
    CaloHitConverter( key="Hits", OnlyRoI=flags.OnlyRoI, EtaWindow=0.4, PhiWindow=0.4 )
    CaloCellConverter( key="Cells" ),
    CaloCellConverter( key="TruthCells" ),

    #CaloClusterConverter( key="Cluster", Cells="Cells", DumpCells=RootStreamFlags.DumpCells )

]


def recordable( key: str ) -> str:
    if key not in [stream[0].split('#')[1] for stream in RootStreamEDMList]:
        raise RuntimeError (f"Key {key} not inside of EDM stream list.")
    return key

