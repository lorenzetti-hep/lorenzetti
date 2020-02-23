
#include "algs/CaloCellMaker.hh"
#include "TLorentzVector.h"



CaloCellMaker::CaloCellMaker( std::string &name ) : 
  IAlgTool( name ),
  m_card("data/detector.card"), 
  m_bc_id_start( 0),
  m_bc_duration(25),
  m_bc_nsamples( 1)
{;}



CaloCellMaker::~CaloCellMaker()
{;}




StatusCode CaloCellMaker::initialize()
{
  group()->mkdir("/CaloCellMaker");
  
  // Create the container
  collection = new xAOD::CaloCellCollection();  

  // This configuration is fixed for each event and can not be changed
  int nsamples=1;
  float start = m_bc_id_start * m_bc_duration;
  float step  = m_bc_duration / m_bc_nsamples;
  
  std::vector<float> tbins;
  for (int sp=0; sp< (nsamples + 1); ++sp)
    tbins.push_back( (start + step*sp) );

  // Read the file
  ifstream file(m_card);



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
        stringstream ss; ss << "layer_" << sampling;
        
        // Create the histogram
        group()->AddHistogram(ss.str(), "", eta_bins, eta_min, eta_max, phi_bins, phi_min, phi_max);
      }
	}
  file.close();

  getContext()->attach( collection );

  return StatusCode::SUCCESS;
}




StatusCode CaloCellMaker::preExecute( EventContext *ctx )
{
  const xAOD::CaloCellCollection *collection=nullptr;

  ctx->retrieve( collection );


  if(!collection){
    MSG_ERROR("It's not possible to retrieve the cell collection.");
    return StatusCode::FAILURE;
  }

  collection->clear();

  return StatusCode::SUCCESS;
}


StatusCode CaloCellMaker::execute( EventContext *ctx )
{
  const G4Step* step = nullptr; 
  const xAOD::CaloCellCollection *collection=nullptr;

    
  ctx->retrieve( step );
  if( !step ){
    MSG_ERROR("It's not possible to retrieve the G4Step.")
    return StatusCode::FAILURE;
  }


  ctx->retrieve( collection );
  if( !collection ){
    MSG_ERROR("It's not possible to retrieve the calo cell collection.");
    return StatusCode::FAILURE;
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

  return StatusCode::SUCCESS;
}



StatusCode CaloCellMaker::fill( EventContext *ctx )
{
  const xAOD::CaloClusterCollection *collection = nullptr; 
  
  ctx->retrieve( collection );

  if( !collection ){
    MSG_ERROR( "It's not possible to retrieve calo cell collection");
    return StatusCode::FAILURE;
  }


  for ( const auto& cell : collection ){ 
    stringstream ss; ss << "/CaloCellMaker/layer_" << (int)cell->sampling();
    int x = group()->hist2(ss.str())->GetXaxis()->FindBin(cell->eta());
    int y = group()->hist2(ss.str())->GetYaxis()->FindBin(cell->phi());
    int bin = group()->hist2(ss.str())->GetBin(x,y,0);
    group()->hist2(ss.str())->SetBinContent( bin, cell->rawEnergy());
  }
}



