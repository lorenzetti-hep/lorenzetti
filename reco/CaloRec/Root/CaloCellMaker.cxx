
#include "CaloRec/CaloCellMaker.h"
#include "TLorentzVector.h"



CaloCellMaker::CaloCellMaker( std::string &name ) : 
  AlgTool( name ),
  m_card("data/detector.card"), 
  m_bc_id_start( 0),
  m_bc_duration(25),
  m_bc_nsamples( 1)
{;}



CaloCellMaker::~CaloCellMaker()
{;}




StatusCode CaloCellMaker::initialize()
{

  auto store = getStoreGateSvc();

  store->mkdir("/CaloCellMaker");
  
  // Create the container
  auto collection = new xAOD::CaloCellCollection();  

  // This configuration is fixed for each event and can not be changed
  int nsamples=1;
  float start = m_bc_id_start * m_bc_duration;
  float step  = m_bc_duration / m_bc_nsamples;
  
  std::vector<float> tbins;
  for (int sp=0; sp< (nsamples + 1); ++sp)
    tbins.push_back( (start + step*sp) );

  // Read the file
  std::ifstream file(m_card);



	std::string line, command;
	while (std::getline(file, line))
	{
      // Get the command
      file >> command;

      if (command=="#"){
        // This is a comment line
        continue;
      
      // Cell configuration
      }else if (command=="C"){
        float  eta_center, phi_center, delta_eta, delta_phi, rmin, rmax;
        int sampling; // Calorimeter layer
        std::string cell_hash;
        file >> sampling >> eta_center >> phi_center >> delta_eta >> delta_phi >> rmin >> rmax >> cell_hash;
        // Create the calorimeter cell
        auto *cell = new xAOD::CaloCell( eta_center, phi_center, delta_eta , delta_phi, 
                                         rmin, rmax, (CaloSampling::CaloSample)sampling,
                                         tbins, cell_hash);
        collection->push_back( cell );
      
      // Layer configuration
      }else if (command=="L"){

        int sampling;
			  float  eta_min, eta_max, eta_bins, phi_min, phi_max, phi_bins, rmin, rmax;
        file >> sampling >> eta_min >> eta_max >> eta_bins >> phi_min >> phi_max >> phi_bins >> rmin >> rmax;
        // Create the layer accessor to generate the correct cell hash
        auto *acc = new xAOD::CaloCellAccessor(eta_min, eta_max, eta_bins, phi_min, phi_max, phi_bins, rmin, rmax, sampling );
        collection->push_back(acc);
       
        // Crrate the histogram name
        std::stringstream ss; ss << "layer_" << sampling;
        
        // Create the histogram
        store->AddHistogram(ss.str(), "", eta_bins, eta_min, eta_max, phi_bins, phi_min, phi_max);
      }
	}
  file.close();

  getContext()->attach( collection );

  return SUCCESS;
}


StatusCode CaloCellMaker::pre_execute( EventContext *ctx )
{
  xAOD::CaloCellCollection *collection=nullptr;

  ctx->retrieve( collection );


  if(!collection){
    MSG_ERROR("It's not possible to retrieve the cell collection.");
    return FAILURE;
  }

  collection->clear();

  return SUCCESS;
}


StatusCode CaloCellMaker::execute( EventContext *ctx )
{
  const G4Step* step = nullptr; 
  xAOD::CaloCellCollection *collection=nullptr;

    
  ctx->retrieve( step );
  if( !step ){
    MSG_ERROR("It's not possible to retrieve the G4Step.")
    return FAILURE;
  }


  ctx->retrieve( collection );
  if( !collection ){
    MSG_ERROR("It's not possible to retrieve the calo cell collection.");
    return FAILURE;
  }

  // Get the position
  G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();

  // Apply all necessary transformation (x,y,z) to (eta,phi,r) coordinates
  // Get ATLAS coordinates (in transverse plane xy)
  auto vpos = TLorentzVector( pos.x(), pos.y(), pos.z(), 0);

  
  if( collection ){
    xAOD::CaloCell *cell=nullptr;
    collection->retrieve( vpos, cell );
    if(cell){
      cell->push_back( step );
    }
  }

  return SUCCESS;
}



StatusCode CaloCellMaker::fill( EventContext *ctx )
{
  xAOD::CaloCellCollection *collection = nullptr; 
  
  auto store = getStoreGateSvc();
  
  ctx->retrieve( collection );

  if( !collection ){
    MSG_ERROR( "It's not possible to retrieve calo cell collection");
    return FAILURE;
  }


  for ( const auto& cell : collection->getCollection() ){ 
    std::stringstream ss; ss << "/CaloCellMaker/layer_" << (int)cell->sampling();
    int x = store->hist2(ss.str())->GetXaxis()->FindBin(cell->eta());
    int y = store->hist2(ss.str())->GetYaxis()->FindBin(cell->phi());
    int bin = store->hist2(ss.str())->GetBin(x,y,0);
    store->hist2(ss.str())->SetBinContent( bin, cell->rawEnergy());
  }
  return SUCCESS;
}



