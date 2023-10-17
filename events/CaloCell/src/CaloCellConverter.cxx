
#include "CaloCell/CaloCellConverter.h"
#include "EventInfo/EventSeedContainer.h"
#include "G4Kernel/CaloPhiRange.h"

using namespace xAOD;


bool CaloCellConverter::convert( const CaloCell *cell, CaloCell_t &cell_t, int link) const
{
  if(cell){
      cell_t.e       = cell->e();
      cell_t.tau     = cell->tau(); 
      cell_t.et      = cell->et();
      cell_t.eta     = cell->eta();
      cell_t.phi     = cell->phi();
      cell_t.deta    = cell->deltaEta();
      cell_t.dphi    = cell->deltaPhi();
      cell_t.descriptor_link = link;
      return true;
  }
  return false;
}


bool CaloCellConverter::convert( const CaloDetDescriptor *descriptor, CaloDetDescriptor_t &descriptor_t, int link ) const
{

  if(descriptor){
    descriptor_t.sampling    = (int)descriptor->sampling();
    descriptor_t.detector    = (int)descriptor->detector();
    descriptor_t.eta         = descriptor->eta();
    descriptor_t.phi         = descriptor->phi();
    descriptor_t.deta        = descriptor->deltaEta(); 
    descriptor_t.dphi        = descriptor->deltaPhi();
    descriptor_t.e           = descriptor->e();
    descriptor_t.tau         = descriptor->tau(); // estimated time of flight  
    descriptor_t.edep        = descriptor->edep(); // truth energy into the bunch crossing zero
    descriptor_t.tof         = descriptor->tof(); // 'truth' time of flight in the cell at BC=0  
    descriptor_t.bcid_start  = descriptor->bcid_start();
    descriptor_t.bcid_end    = descriptor->bcid_end();
    descriptor_t.bc_duration = descriptor->bc_duration();
    descriptor_t.hash        = descriptor->hash();
    descriptor_t.pulse       = descriptor->pulse();
    descriptor_t.cell_link   = link; // cross link to cell


    for ( int bcid = descriptor->bcid_start();  bcid <= descriptor->bcid_end(); ++bcid )
    {
      descriptor_t.edep_per_bunch.push_back( descriptor->edep(bcid) );
      descriptor_t.tof_per_bunch.push_back( descriptor->tof(bcid)   );
    }

    return true;
  }
  return false;
}



bool CaloCellConverter::convert( const CaloDetDescriptor_t &descriptor_t, CaloDetDescriptor *&descriptor ) const
{

  descriptor = new xAOD::CaloDetDescriptor( descriptor_t.eta, 
                                            descriptor_t.phi,
                                            descriptor_t.deta,
                                            descriptor_t.dphi,
                                            descriptor_t.hash,
                                            (CaloSampling)descriptor_t.sampling, 
                                            (Detector)descriptor_t.detector,
                                            descriptor_t.bc_duration,
                                            descriptor_t.bcid_start,
                                            descriptor_t.bcid_end );

  descriptor->setE(descriptor_t.e); // estimated energy from OF
  descriptor->setTau(descriptor_t.tau); // 
  descriptor->setPulse( descriptor_t.pulse); // pulse from generator
  
  for ( int bcid = descriptor->bcid_start();  bcid <= descriptor->bcid_end(); ++bcid )
  {
    descriptor->edep( bcid, descriptor_t.edep_per_bunch[bcid] ); // truth energy for each bunch crossing
    descriptor->tof ( bcid, descriptor_t.tof_per_bunch[bcid]  ); //
  }
  return true;
}


bool CaloCellConverter::convert( const CaloCell_t &cell_t, CaloCell *&cell ) const
{
  cell = new xAOD::CaloCell( cell_t.eta, 
                             cell_t.phi, 
                             cell_t.deta, 
                             cell_t.dphi );
  cell->setE(cell_t.e); // estimated energy from OF. Same as descriptor->e()
  cell->setTau(cell_t.tau); // estimated time of flight from OF. Same as descriptor->tau()     
  cell->setEt(cell_t.et); // estimated transverse energy from OF
  return true;
}


bool CaloCellConverter::serialize( std::string &key, SG::EventContext &ctx, TTree *tree) const
{
  std::vector<xAOD::CaloCell_t> *cells_t=nullptr;
  std::vector<xAOD::CaloDetDescriptor_t> *descriptors_t=nullptr;


  std::string branch_cells_name = "CaloCell";
  if ( tree->FindBranch(branch_cells_name.c_str())){
    tree->SetBranchAddress( (branch_cells_name).c_str() , &cells_t     );
    tree->GetBranch(branch_cells_name.c_str() );
  }else{
    tree->Branch( (branch_cells_name).c_str(), &cells_t     );
  }

  std::string branch_desc_name = "CaloDetDescriptor";
  if ( tree->FindBranch(branch_desc_name.c_str())){
    tree->SetBranchAddress( (branch_desc_name).c_str() , &descriptors_t     );
    tree->GetBranch(branch_desc_name.c_str() );
  }else{
    tree->Branch( (branch_desc_name).c_str(), &descriptors_t     );
  }


  xAOD::cell_links_t cell_links;

  SG::ReadHandle<xAOD::CaloCellContainer> container(key, ctx);
  if( !container.isValid() )
  {
    return false;
  }
    
  SG::ReadHandle<xAOD::EventSeedContainer> seeds( m_seedKey, ctx);
  if( !seeds.isValid() )
  {
    return false;
  }

  int link = 0; // decorate all cells 
  for (const auto seed : **seeds.ptr() )
  {
      for (const auto cell : **container.ptr() ){
          const xAOD::CaloDetDescriptor *descriptor = cell->descriptor();
      
          float deltaEta = std::abs( seed->eta() - descriptor->eta());
          float deltaPhi = std::abs( CaloPhiRange::diff(seed->phi(), descriptor->phi()) );
          if ( deltaEta < m_etaWindow/2 && deltaPhi < m_phiWindow/2 )
          {
              xAOD::CaloCell_t cell_t;                
              xAOD::CaloDetDescriptor_t descriptor_t;
              if (cell_links.count(cell)){
                  convert(cell, cell_t, cell_links[cell]);
                  convert( descriptor, descriptor_t, cell_links[cell]);
              }else{
                  cell_links[cell] = link;
                  convert(cell, cell_t, link);
                  convert( descriptor, descriptor_t, link);
                  link++;
              }
              cells_t->push_back(cell_t);
              descriptors_t->push_back(descriptor_t);
          }// check if cell is inside of the window
      }// loop over all cells
  }// loop over all seeds

  tree->Fill();

  return true;
}


bool CaloCellConverter::deserialize( std::string &key , int &evt, TTree* tree, SG::EventContext &ctx) const
{
  std::vector<xAOD::CaloCell_t> cells_t;
  std::vector<xAOD::CaloDetDescriptor_t> descriptors_t;
  
  tree->SetBranchAddress( "CaloCell"          , &cells_t       );
  tree->SetBranchAddress( "CaloDetDescriptor" , &descriptors_t );
  tree->GetEntry( evt );

  std::map<int, xAOD::CaloDetDescriptor*> descriptor_links;

  int link=0;
  for (auto &descriptor_t : descriptors_t )
  {
    xAOD::CaloDetDescriptor *descriptor = nullptr;
    convert(descriptor_t, descriptor); // alloc memory
    descriptor_links[link] = descriptor;
    link++;
  }
  
  SG::WriteHandle<xAOD::CaloCellContainer> container(key, ctx);
  container.record( std::unique_ptr<xAOD::CaloCellContainer>(new xAOD::CaloCellContainer()));

  for( auto &cell_t : cells_t )
  {
    xAOD::CaloCell *cell = nullptr;
    convert(cell_t, cell); // alloc memory
    cell->setDescriptor( descriptor_links[cell_t.descriptor_link] );
    container->push_back(cell);
  }

  return true;
}
