
#ifndef Simulator_h
#define Simulator_h

#include "core/IAlgTool.h"

#include <vector>
#include <string>



class Simulator
{
  public:

    Simulator( std::string, int );
    ~Simulator();
    
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
