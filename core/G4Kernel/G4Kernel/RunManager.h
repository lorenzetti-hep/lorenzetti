
#ifndef RunManager_h
#define RunManager_h

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

  private:

    int m_nThreads;

    bool m_runVis;
    
    std::string m_output;

    std::vector< Gaugi::Algorithm* > m_acc;

    PrimaryGenerator       *m_generator;
};
#endif
