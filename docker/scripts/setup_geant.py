import os, glob

basepath = os.getcwd()




envs = {
    "G4NEUTRONHPDATA"       :"G4NDL*",
    "G4LEDATA"              :"G4EMLOW*",
    "G4LEVELGAMMADATA"      :"PhotonEvaporation*",
    "G4RADIOACTIVEDATA"     :"RadioactiveDecay*",
    "G4PARTICLEXSDATA"      :"G4PARTICLEXS*",
    "G4PIIDATA"             :"G4PII*",
    "G4REALSURFACEDATA"     :"RealSurface*",
    "G4SAIDXSDATA"          :"G4SAIDDATA*",
    "G4ABLADATA"            :"G4ABLA*",
    "G4INCLDATA"            :"G4INCL*",
    "G4ENSDFSTATEDATA"      :"G4ENSDFSTATE*",
}


with open("setup_geant.sh", 'w') as f:
    f.write(f"export PATH={basepath}:$PATH\n")
    libs = ":".join(list(glob.glob(f"{basepath}/BuildProducts/lib/*")))[1::]
    f.write(f"export LD_LIBRARY_PATH={libs}:$LD_LIBRARY_PATH\n")
    for key, value in envs.items():
        path = glob.glob(f"{basepath}/data/{value}")[0]
        f.write(f"export {key}={path}\n")
