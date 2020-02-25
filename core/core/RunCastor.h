
#ifndef RunCastor_h
#define RunCastor_h

#include "core/IAlgTool.h"

#include <vector>
#include <string>



class RunCastor
{
  public:

    RunCastor( std::string, int );
    ~RunCastor();
    
    void vis( std::string );

    /** Run Geant **/
    void run( std::string="");

    /** Add a reconstruction alg to the main sequence **/
    void push_back( IAlgTool* );

  private:

    int m_nThreads;
    std::string m_visMacro;
    std::string m_output;

    std::vector<IAlgTool*> m_sequence;
};
#endif
