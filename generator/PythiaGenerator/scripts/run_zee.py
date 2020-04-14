


from PythiaGenerator import Zee


generator = Zee( "Zee",
                 OutputFile = 'output',
                 MinbiasFile = 'minbias_zee_config.cmnd',
                 MainFile = 'zee_config.cmnd',
                 )

generator.core().run()
generator.core().finalize()
del generator



