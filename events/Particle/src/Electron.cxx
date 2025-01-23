#include "Particle/Electron.h"
#include "G4Kernel/CaloPhiRange.h"

using namespace xAOD;


Electron::Electron():
    EDM(), 
    m_caloCluster(nullptr) 
    m_isEM(4,false)
{}

bool Electron::isTight() const
{
    return m_isEM[0];
}

bool Electron::isMedium() const
{
    return m_isEM[1];
}

bool Electron::isLoose() const
{
    return m_isEM[2];
}

bool Electron::isVeryLoose() const
{
    return m_isEM[3];
}

bool Electron::setIsTight(bool isTight)
{
    m_isEM[0] = isTight;
}

bool Electron::setIsMedium(bool isMedium)
{
    m_isEM[1] = isMedium;
}

bool Electron::setIsLoose(bool isLoose)
{
    m_isEM[2] = isLoose;
}   

bool Electron::setIsVeryLoose(bool isVeryLoose)
{
    m_isEM[3] = isVeryLoose;
}   
