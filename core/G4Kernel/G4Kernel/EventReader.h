#ifndef EventReader_h
#define EventReader_h


#include "TFile.h"
#include "TTree.h"
#include "EventInfo/EventInfo.h"
#include "EventInfo/EventInfoContainer.h"
#include "G4Kernel/PrimaryGenerator.h"



class EventReader : public PrimaryGenerator
{
  public:

    /*! Constructor */
    EventReader(std::string name);
    /*! Destructor */
    virtual ~EventReader();
    

    virtual void GeneratePrimaryVertex(G4Event* anEvent) override;


    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;

    virtual PrimaryGenerator* copy() override;

  private:
    

    
    template <class T> void InitBranch(TTree* fChain, std::string branch_name, T* param, bool message=true);
    
    void link(TTree *t);
    void clear();
    void allocate();
    void release();

    // We  have to take care for the position of primaries because
    // primary vertices outside the world voulme give rise to G4Execption.
    // If the default implementation is not adequate, an alternative
    // can be implemented in your own class.
    bool CheckVertexInsideWorld(const G4ThreeVector& pos) const;
    
    void Load( G4Event *, xAOD::EventInfo *);

    bool Add( G4Event* g4event , int i, int bc_id );

    unsigned int           m_evt;
    std::string            m_filename;
    std::string            m_eventKey;

    TFile *m_f;
    TTree *m_ttree;

    double m_avgmu;

    std::vector<int>    *m_p_isMain		  ;
    std::vector<int>    *m_p_pdg_id	    ;
    std::vector<int>    *m_p_bc_id	    ;
    std::vector<float>  *m_bc_mu				;
    std::vector<int>    *m_bc_id_nhits  ;	
    std::vector<float>  *m_p_px				  ;
    std::vector<float>  *m_p_py         ;
    std::vector<float>  *m_p_pz		      ;
    std::vector<float>  *m_p_prod_x		  ;
    std::vector<float>  *m_p_prod_y		  ;
    std::vector<float>  *m_p_prod_z		  ;
    std::vector<float>  *m_p_prod_t		  ;
    std::vector<float>  *m_p_eta		    ;
    std::vector<float>  *m_p_phi		    ;
    std::vector<float>  *m_p_e		      ;
    std::vector<float>  *m_p_et		      ;
};

#endif
