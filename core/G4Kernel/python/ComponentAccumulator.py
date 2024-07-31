
__all__ = ["ComponentAccumulator"]

from GaugiKernel import Cpp, LoggingLevel
from GaugiKernel.constants import MINUTES
from G4Kernel import EventReader
import os
import time
import gc
import ROOT


class ComponentAccumulator(Cpp):
    """
    Component Accumulator for Geant4 simulations.
    Uses the G4Kernel RunManager as a backend.
    """

    def __init__(self, name, detector,
                 OutputFile: str = "output.root",
                 Seed: int = 512,
                 NumberOfThreads: int = 1,
                 RunVis: bool = False,
                 Timeout: int = 120*MINUTES,
                 OutputLevel: int = LoggingLevel.toC('INFO'),
                 ):

        Cpp.__init__(self, ROOT.RunManager(name))
        # convert python to geant4
        self.__detector = detector
        self.__detector.compile()
        # set the geant detector into the geant
        self._core.setDetectorConstruction(self.__detector.core())
        self.setProperty("OutputFile", OutputFile)
        self.setProperty("VisMac", self.__detector.VisMac)
        self.setProperty("NumberOfThreads", NumberOfThreads)
        self.setProperty("Timeout", Timeout)
        self.setProperty("RunVis", RunVis)
        self.setProperty("Seed", Seed)
        # self.setProperty( "OutputLevel"     , OutputLevel             )
        # Set the vis mac file into the manager core
        self.outputFiles = [(self.OutputFile+".%d" % thread)
                            for thread in range(self.NumberOfThreads)]

    def __del__(self):
        del self._core
        gc.collect()
        time.sleep(2)
        self.merge()

    def run(self, evt: int = None):
        """
        Runs the events in the simulation.

        Parameters
        ----------
        evt : int, optional
            Number of events to run if None, runs all, by default None
        """
        if evt is None:
            evt = self.__numberOfEvents
        elif evt > self.__numberOfEvents:
            evt = self.__numberOfEvents
        self._core.run(evt)

    def __add__(self, algs):
        if type(algs) is not list:
            algs = [algs]
        for alg in algs:
            self._core.push_back(alg.core())
        return self

    def setGenerator(self, gen: EventReader):
        """
        Sets the generator reader for the simulation.
        The reader is used to read EVT files and return the seeds for G4

        Parameters
        ----------
        gen : EventReader
            Generator reader object
        """
        self.__numberOfEvents = gen.GetEntries()
        self._core.setGenerator(gen.core())


    def detector(self):
        """
        Returns the detector construction object.
        """
        return self.__detector

    def merge(self):
        files = ' '.join(self.outputFiles)
        command = f"hadd -f {self.OutputFile} {files}"
        os.system(command)
        # remove thread files
        for fname in self.outputFiles:
            os.system('rm ' + fname)
