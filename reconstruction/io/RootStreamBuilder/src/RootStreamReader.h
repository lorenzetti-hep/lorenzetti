#ifndef RootStreamReader_h
#define RootStreamReader_h

#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/Algorithm.h"
#include "GaugiKernel/DataHandle.h"
#include "RootStreamConverter.h"



class RootStreamReader : public Gaugi::Algorithm
{

  public:
    /** Constructor **/
    RootStreamReader( std::string );
    
    virtual ~RootStreamReader();
    
    virtual StatusCode initialize() override;

    virtual StatusCode bookHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode pre_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode execute( SG::EventContext &ctx , const G4Step *step) const override;
    
    virtual StatusCode execute( SG::EventContext &ctx , int /*evt*/ ) const override;

    virtual StatusCode post_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode fillHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode finalize() override;

   void push_back( RootStreamConverter *converter ){m_converters.push_back(converter);};


  private:
 

    StatusCode deserialize( int evt, SG::EventContext &ctx ) const;
    
    std::string m_inputFile;
    std::vector<std::string> m_containers;
    std::vector<RootStreamConverter*> m_converters;

    int m_outputLevel;
};

#endif

