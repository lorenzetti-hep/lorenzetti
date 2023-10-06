#include "CaloHit/CaloHitConverter.h"
#include "EventInfo/EventSeedContainer.h"
#include "G4Kernel/CaloPhiRange.h"


using namespace xAOD;


bool CaloHitConverter::convert( const CaloHit *hit, CaloHit_t &hit_t ) const
{

  if(hit){
    hit_t.sampling    = (int)hit->sampling();
    hit_t.detector    = (int)hit->detector();
    hit_t.eta         = hit->eta();
    hit_t.phi         = hit->phi();
    hit_t.deta        = hit->deltaEta(); 
    hit_t.dphi        = hit->deltaPhi();
    hit_t.bcid_start  = hit->bcid_start();
    hit_t.bcid_end    = hit->bcid_end();
    hit_t.bc_duration = hit->bc_duration();
    hit_t.hash        = hit->hash();

    for ( int bcid = hit->bcid_start();  bcid <= hit->bcid_end(); ++bcid )
    {
      hit_t.edep.push_back( hit->edep(bcid) );
      hit_t.tof.push_back ( hit->tof(bcid)  ); //
    }

    return true;
  }

  return false;
}



bool CaloHitConverter::convert( const CaloHit_t &hit_t, CaloHit *&hit ) const
{
  
  hit = new xAOD::CaloHit( hit_t.eta, 
                           hit_t.phi,
                           hit_t.deta,
                           hit_t.dphi,
                           hit_t.hash,
                           (CaloSampling)hit_t.sampling, 
                           (Detector)hit_t.detector,
                           hit_t.bc_duration,
                           hit_t.bcid_start,
                           hit_t.bcid_end );

  int pos=0;
  for ( int bcid = hit->bcid_start();  bcid <= hit->bcid_end(); ++bcid)
  {
    hit->edep( bcid, hit_t.edep.at(pos) ); // truth energy for each bunch crossing
    hit->tof ( bcid, hit_t.tof.at(pos)  ); // truth time of flight (it takes the last hit in the simulation order. Need to evaluate which strategy is the best.)
    pos++;
  }

  return true;
}


bool CaloHitConverter::serialize( std::string &key, SG::EventContext &ctx, TTree *tree) const
{
  std::vector<xAOD::CaloHit_t> hits_t;

  auto branch = tree->Branch( ("CaloHitContainer_"+key).c_str(), &hits_t     );
  
  SG::ReadHandle<xAOD::EventSeedContainer> seeds( m_seedKey, ctx );
  SG::ReadHandle<xAOD::CaloHitContainer> container(key, ctx);
  
  if( !seeds.isValid() )
  {
      return false;
  }

  if( !container.isValid() )
  {
    return false;
  }

  float etot=0;
  for (const auto hit : **container.ptr() ){
       
    if(m_onlyRoi){
      bool match=false;
      for (const auto& seed : **seeds.ptr())
      {
        float deltaEta = std::abs( seed->eta() - hit->eta() );
        float deltaPhi = std::abs( CaloPhiRange::diff(seed->phi(), hit->phi()) );
        if ( deltaEta < m_etaWindow/2 && deltaPhi < m_phiWindow/2 )
        {
          match=true;
          break;
        }
      }
      if(!match) continue; // skip this hit
    }

    xAOD::CaloHit_t hit_t;
    convert(hit, hit_t);
    if(hit->hash() != hit_t.hash){
      return false;
    }
    hits_t.push_back(hit_t);
    etot+=hit->edep();
  }// check if hit is inside of the window

  branch->Fill();
  return true;
}


bool CaloHitConverter::deserialize( std::string &key , int &evt, TTree* tree, SG::EventContext &ctx) const
{
  std::vector<xAOD::CaloHit_t> *hits_t = nullptr;
  tree->SetBranchAddress( ("CaloHitContainer_"+key).c_str() , &hits_t );
  tree->GetEntry( evt );
  SG::WriteHandle<xAOD::CaloHitContainer> container(key, ctx);
  container.record( std::unique_ptr<xAOD::CaloHitContainer>(new xAOD::CaloHitContainer()));

  float etot=0;
  for( auto& hit_t : *hits_t)
  {
    xAOD::CaloHit  *hit=nullptr;
    convert(hit_t, hit);
    container->push_back(hit);
    etot+=hit->edep();
  }

  return true;
}

