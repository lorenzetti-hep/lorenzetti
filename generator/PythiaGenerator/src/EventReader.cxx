
#include "EventReader.h"
#include "EventInfo/EventInfo.h"
#include "G4Kernel/EventLoop.h"
#include "G4Kernel/constants.h"
#include "GaugiKernel/PrettyTable.h"
#include "G4LorentzVector.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4TransportationManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include <math.h>
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"

using namespace Gaugi;


EventReader::EventReader(std::string name):
    IMsgService(name),
    PrimaryGenerator(),
    m_evt(0)
{
  ROOT::EnableThreadSafety();
  declareProperty( "FileName", m_filename=""          );
  declareProperty( "EventKey", m_eventKey="EventInfo" );

}

PrimaryGenerator* EventReader::copy()
{
  auto *gun = new EventReader(getLogName());
  gun->setProperty( "FileName", m_filename );
  gun->setProperty( "EventKey", m_eventKey );
  return gun;
}



EventReader::~EventReader()
{
  if( m_f ) delete m_f;
}


StatusCode EventReader::initialize()
{
  MSG_INFO( "Open the root file: " << m_filename );
  m_f = new TFile( m_filename.c_str() , "read" );

  m_ttree = (TTree*)m_f->Get("particles");
  m_evt=0;
  link( m_ttree );
  allocate();
  return StatusCode::SUCCESS;
}


StatusCode EventReader::finalize()
{
  release();
  m_f->Close();
  return StatusCode::SUCCESS;
}



template <class T> 
void EventReader::InitBranch(TTree* fChain, std::string branch_name, T* param, bool message)
{
  std::string bname = branch_name;
  if (fChain->GetAlias(bname.c_str()))
     bname = std::string(fChain->GetAlias(bname.c_str()));

  if (!fChain->FindBranch(bname.c_str()) && message) {
    MSG_WARNING( "unknown branch " << bname );
    return;  
  }
  fChain->SetBranchStatus(bname.c_str(), 1.);
  fChain->SetBranchAddress(bname.c_str(), param);
}


void EventReader::link(TTree *t)
{  
  InitBranch( t, "avg_mu"		  ,&m_avgmu		    );
  InitBranch( t, "p_isMain"	  ,&m_p_isMain		);
  InitBranch( t, "p_pdg_id"	  ,&m_p_pdg_id	  );
  InitBranch( t, "p_bc_id"	  ,&m_p_bc_id	    );
  InitBranch( t, "bc_mu"			,&m_bc_mu				);
  InitBranch( t, "bc_id_nhits",&m_bc_id_nhits	);
  InitBranch( t, "p_px"				,&m_p_px				);
  InitBranch( t, "p_py"       ,&m_p_py        );
  InitBranch( t, "p_pz"		    ,&m_p_pz		    );
  InitBranch( t, "p_prod_x"		,&m_p_prod_x		);
  InitBranch( t, "p_prod_y"		,&m_p_prod_y		);
  InitBranch( t, "p_prod_z"		,&m_p_prod_z		);
  InitBranch( t, "p_prod_t"		,&m_p_prod_t		);
  InitBranch( t, "p_eta"			,&m_p_eta		    );
  InitBranch( t, "p_phi"			,&m_p_phi		    );
  InitBranch( t, "p_e"			  ,&m_p_e		      );
  InitBranch( t, "p_et"			  ,&m_p_et		    );
}


void EventReader::clear()
{ 
  m_avgmu       = 0.0;
  m_p_isMain		->clear();
  m_p_pdg_id	  ->clear();
  m_p_bc_id	    ->clear();
  m_bc_mu				->clear(); 
  m_bc_id_nhits	->clear();
  m_p_px				->clear();
  m_p_py        ->clear();
  m_p_pz		    ->clear();
  m_p_prod_x		->clear();
  m_p_prod_y		->clear();
  m_p_prod_z		->clear();
  m_p_prod_t		->clear();
  m_p_eta		    ->clear();
  m_p_phi		    ->clear();
  m_p_e		      ->clear();
  m_p_et		    ->clear();
}


void EventReader::allocate()
{
  m_p_isMain		= new std::vector<int>();
  m_p_pdg_id	  = new std::vector<int>();
  m_p_bc_id	    = new std::vector<int>();
  m_bc_mu				= new std::vector<float>();
  m_bc_id_nhits	= new std::vector<int>();
  m_p_px				= new std::vector<float>();
  m_p_py        = new std::vector<float>();
  m_p_pz		    = new std::vector<float>();
  m_p_prod_x		= new std::vector<float>();
  m_p_prod_y		= new std::vector<float>();
  m_p_prod_z		= new std::vector<float>();
  m_p_prod_t		= new std::vector<float>();
  m_p_eta		    = new std::vector<float>();
  m_p_phi		    = new std::vector<float>();
  m_p_e		      = new std::vector<float>();
  m_p_et		    = new std::vector<float>();
}


void EventReader::release()
{
  delete m_p_isMain		;
  delete m_p_pdg_id	  ;
  delete m_p_bc_id	  ;
  delete m_bc_mu			;
  delete m_bc_id_nhits;	
  delete m_p_px				;
  delete m_p_py       ;
  delete m_p_pz		    ;
  delete m_p_prod_x		;
  delete m_p_prod_y		;
  delete m_p_prod_z		;
  delete m_p_prod_t		;
  delete m_p_eta		  ;
  delete m_p_phi		  ;
  delete m_p_e		    ;
  delete m_p_et		    ;
}


// Call by geant
void EventReader::GeneratePrimaryVertex( G4Event* anEvent )
{
  clear();
  m_evt = anEvent->GetEventID();
  
  EventLoop *loop = static_cast<EventLoop*> (G4RunManager::GetRunManager()->GetNonConstCurrentRun());

  if ( m_evt <  m_ttree->GetEntries() ){

    MSG_INFO( "Get event (EventReader) with number " << m_evt )
    m_ttree->GetEntry(m_evt);
    SG::WriteHandle<xAOD::EventInfoContainer>  event(m_eventKey, loop->getContext());
    event.record( std::unique_ptr<xAOD::EventInfoContainer>( new xAOD::EventInfoContainer() ) );

    xAOD::EventInfo *evt = new xAOD::EventInfo();
    evt->setEventNumber( m_evt );
    evt->setAvgmu( m_avgmu );
    Load( anEvent, evt );

    MSG_INFO( "Event id         : " << evt->eventNumber() );
    MSG_INFO( "Avgmu            : " << evt->avgmu() );
    MSG_INFO( "Number of seeds  : " << evt->size() );
    event->push_back(evt);

  }else{
    MSG_INFO( "EventReader: no generated particles. run terminated..." );
    G4RunManager::GetRunManager()->AbortRun();
  }
}


bool EventReader::CheckVertexInsideWorld(const G4ThreeVector& pos) const
{
  G4Navigator* navigator= G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  G4VPhysicalVolume* world= navigator->GetWorldVolume();
  G4VSolid* solid= world-> GetLogicalVolume()->GetSolid();
  EInside qinside= solid->Inside(pos);
  if( qinside != kInside) return false;
  else return true;
}


void EventReader::Load( G4Event* g4event, xAOD::EventInfo *event )
{
  float totalEnergy = 0.0;
  
  // Add all particles into the Geant event
  for ( unsigned int i=0; i < m_p_e->size(); ++i )
  {
    int bc_id = m_p_bc_id->at(i);
    if(m_p_pdg_id->at(i)==0){
      
      event->push_back( xAOD::seed_t{m_p_et->at(i), m_p_eta->at(i), m_p_phi->at(i), m_p_px->at(i), m_p_py->at(i), m_p_pz->at(i), 0} );
    }
    if( m_p_isMain->at(i) ){
      if(m_p_pdg_id->at(i)!=0){
        totalEnergy+= m_p_et->at(i);
        Add( g4event, i, bc_id );
        Add( g4event, i, special_bcid_for_truth_reconstruction );
      }
    }else{
      Add( g4event, i, bc_id );
    }
  }
}


bool EventReader::Add( G4Event* g4event , int i, int bc_id )
{
  G4LorentzVector xvtx( m_p_prod_x->at(i), m_p_prod_y->at(i), m_p_prod_z->at(i), m_p_prod_t->at(i) + (bc_id*25*c_light)  );
  if (! CheckVertexInsideWorld(xvtx.vect()*mm)) return false;
  G4PrimaryVertex* g4vtx= new G4PrimaryVertex(  xvtx.x()*mm, xvtx.y()*mm, xvtx.z()*mm, xvtx.t()  );
  G4int pdgcode= m_p_pdg_id->at(i);
  G4LorentzVector p( m_p_px->at(i), m_p_py->at(i), m_p_pz->at(i),  m_p_e->at(i) );
  G4PrimaryParticle* g4prim = new G4PrimaryParticle(pdgcode, p.x()*GeV, p.y()*GeV, p.z()*GeV);
  g4vtx->SetPrimary(g4prim);
  g4event->AddPrimaryVertex(g4vtx);
  return true;
}

