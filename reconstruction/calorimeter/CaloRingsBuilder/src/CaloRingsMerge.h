#ifndef CaloRingsMerge_h
#define CaloRingsMerge_h

#include "GaugiKernel/Algorithm.h"
#include "GaugiKernel/DataHandle.h"
#include "CaloCell/CaloCell.h"


class CaloRingsMerge : public Gaugi::Algorithm
{
  public:
  
    /** Contructor **/
    CaloRingsMerge( std::string name );
    /** Destructor **/
    ~CaloRingsMerge();
    
    /*! initialize the algorithm **/
    virtual StatusCode initialize() override;
    /*! Book all histograms into the current storegate **/
    virtual StatusCode bookHistograms( SG::EventContext &ctx ) const override;
    /*! Execute in step action step from geant core **/
    virtual StatusCode execute( SG::EventContext &ctx , const G4Step *step) const override;
    /*! Execute in ComponentAccumulator **/
    virtual StatusCode execute( SG::EventContext &ctx , int /*evt*/ ) const override;
    /*! execute before start the step action **/
    virtual StatusCode pre_execute( SG::EventContext &ctx ) const override;
    /*! execute after the step action **/ 
    virtual StatusCode post_execute( SG::EventContext &ctx ) const override;
    /*! fill hisogram in the end **/
    virtual StatusCode fillHistograms( SG::EventContext &ctx ) const override;
    /*! finalize the algorithm **/ 
    virtual StatusCode finalize() override;

  private:
    
    /*! collection key */
    std::vector<std::string> m_collectionKeys;
    /*! CaloCellContainer key for reco cells */
    std::string m_ringerKey;
 
    int m_outputLevel;
};



#endif
