
#ifndef RunManager_h
#define RunManager_h

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
    
    void run( std::string="");

    void push_back( Gaugi::Algorithm* );

  private:

    int m_nThreads;

    std::string m_visMacro;
    
    std::string m_output;

    std::vector< Gaugi::Algorithm* > m_acc;
};
#endif
