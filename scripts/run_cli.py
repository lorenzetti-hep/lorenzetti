
from G4Kernel import ComponentAccumulator






acc = ComponentAccumulator("ComponentAccumulator")
acc.VisMacro = 'init_vis.mac' 



ecal_tools = [
                PulseGeneratorTool("PulseGenerator_ECal_Barrel", nSamples = 7, path = '/path/to/card.dat' ),
                EnergyEstimatorTool( "EnergyEstimator_ECal_Barrel", path = '/path/to/card.dat' ),
              ]
          

hcal_tools = [
                PulseGeneratorTool("PulseGenerator_HCal_Barrel", nSamples = 7, path = '/path/to/card.dat' ),
                EnergyEstimatorTool( "EnergyEstimator_HCal_Barrel", path = '/path/to/card.dat' ),
              ]
 


ecal_barrel = CaloCellMaker( "CaloCellMaker_ECal_Barrel", Tools = ecal_tools )
hcal_barrel = CaloCellMaker( "CaloCellMaker_HCal_Barrel", Tools = hcal_tools )


cluster_maker = CaloClusterMaker()

rings = CaloRingsBuilder()

output = PhysValOutput()


# Make the sequence
acc += ecal_barrel
acc += hcal_barrel
#acc += ecal_extbarrel
#acc += hcal_extbarrel
acc += cluster_maker
acc += rings
acc += output


acc.run()




