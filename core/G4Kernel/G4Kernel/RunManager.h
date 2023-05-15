
#ifndef RunManager_h
#define RunManager_h

#include "G4VUserDetectorConstruction.hh"
#include "G4Kernel/PrimaryGenerator.h"
#include "GaugiKernel/Algorithm.h"
#include "GaugiKernel/MsgStream.h"
#include "GaugiKernel/Property.h"


#include <vector>
#include <string>


class RunManager: public MsgService, 
                  public Gaugi::PropertyService
{
  public:

    RunManager( std::string name);
    ~RunManager();
    
    void run( int evt=10000);

    void push_back( Gaugi::Algorithm* );

    void setGenerator( PrimaryGenerator * );

    void setDetectorConstruction( G4VUserDetectorConstruction * );


  private:

    void header();

    int m_nThreads;

    bool m_runVis;
    
    float m_seed;

    int m_timeout;
    
    std::string m_vis_mac;
    std::string m_output;

    std::vector< Gaugi::Algorithm* > m_acc;

    PrimaryGenerator       *m_generator;

    G4VUserDetectorConstruction *m_detector;
};
#endif
