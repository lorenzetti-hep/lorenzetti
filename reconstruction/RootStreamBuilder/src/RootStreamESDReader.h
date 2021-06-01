#ifndef RootStreamESDReader_h
#define RootStreamESDReader_h

#include "GaugiKernel/ComponentReader.h"
#include "CaloCell/enumeration.h"
#include "EventInfo/EventInfo.h"
#include "TruthParticle/TruthParticle.h"
#include "TFile.h"
#include "TTree.h"



class RootStreamESDReader : public Gaugi::ComponentReader
{

  public:
    /** Constructor **/
    RootStreamESDReader( std::string );
    
    virtual ~RootStreamESDReader();
    
    virtual StatusCode initialize() override;

    virtual StatusCode GeneratePrimaryVertex( int evt, SG::EventContext &ctx ) const override;
    
    virtual StatusCode finalize() override;

    virtual int GetEntries() const override;

  private:
 

    StatusCode deserialize( int evt, SG::EventContext &ctx ) const;

    template <class T> void InitBranch(TTree* fChain, std::string branch_name, T* param) const;
    
    std::string m_cellsKey;
    std::string m_eventKey;
    std::string m_truthKey;
    std::string m_inputFile;
    std::string m_ntupleName;

    int m_outputLevel;
    TFile *m_file;
    TTree *m_tree;
    int m_entries;
};

#endif

