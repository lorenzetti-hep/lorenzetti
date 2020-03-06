

#include "CaloRec/CaloCellMaker.h"
#include "TLorentzVector.h"
#include "TPulseGenerator.h"



CaloCellMaker::CaloCellMaker( std::string name ) : 
  AlgTool( name ),
  m_inputKey(""),
  m_outputKey("")
{
}



CaloCellMaker::~CaloCellMaker()
{;}





StatusCode CaloCellMaker::initialize()
{
  auto store = getStoreGateSvc();
  
  MSG_INFO( "Pushing all configurations from: " << m_calibPath );
  
  std::ifstream bc_file( m_calibPath +"/bunch.dat");
  bc_file >> m_bc_id_start >> m_bc_id_end >> m_bc_duration >> m_bc_nsamples;
  bc_file.close();
  
  
  
  // This configuration is fixed for each event and can not be changed
  float start = m_bc_id_start * m_bc_duration;
  float step  = m_bc_duration / m_bc_nsamples;
  int nbins   = (m_bc_id_end - m_bc_id_start) + 1;

  std::vector<float> tbins;
  for (int b=0; b< nbins; ++b){
    tbins.push_back( (start + step*b) );
  }
 

  // Read the file
  std::ifstream det_file( m_calibPath +"/detector.dat");

	std::string line, command;
	while (std::getline(det_file, line))
	{
    // Get the command
    det_file >> command;

    if (command=="#"){
      // This is a comment line
      continue;
    
    // Cell configuration
    }else if (command=="C"){
      float  eta_center, phi_center, delta_eta, delta_phi, rmin, rmax;
      int sampling; // Calorimeter layer
      std::string cell_hash;
      det_file >> sampling >> eta_center >> phi_center >> delta_eta >> delta_phi >> rmin >> rmax >> cell_hash;
      // Create the calorimeter cell
      auto *cell = new xAOD::CaloCell( eta_center, phi_center, delta_eta , delta_phi, 
                                       rmin, rmax, (CaloSampling::CaloSample)sampling,
                                       tbins, cell_hash);
      m_collection.push_back( cell );
    
    // Layer configuration
    }else if (command=="L"){

      int sampling;
		  float  eta_min, eta_max, eta_bins, phi_min, phi_max, phi_bins, rmin, rmax;
      det_file >> sampling >> eta_min >> eta_max >> eta_bins >> phi_min >> phi_max >> phi_bins >> rmin >> rmax;
      // Create the layer accessor to generate the correct cell hash
      auto *acc = new xAOD::CaloCellAccessor(eta_min, eta_max, eta_bins, phi_min, phi_max, phi_bins, rmin, rmax, sampling );
      m_collection.push_back(acc);
     
      // Crrate the histogram name
      std::stringstream ss; ss << "layer_" << sampling;
      
      // Create the histogram
      store->AddHistogram(ss.str(), "", eta_bins, eta_min, eta_max, phi_bins, phi_min, phi_max);
    }
	}

  det_file.close();


  

  // Pulse generator from: https://gitlab.cern.ch/ginaciog/calopulsekit
  // Initalize the pulse generator for eletromagnetic and hadronic calorimeters
  // Add shaper for LAr (eletromagnetic calorimeter)
  m_pulseGenerator[ CaloSampling::CaloLayer::ECal ] = new CPK::TPulseGenerator( 7, (m_calibPath+"/larcalorimeter_pulse_shape.dat").c_str());
  // Add shaper for hadronic layer
  m_pulseGenerator[ CaloSampling::CaloLayer::HCal ] = new CPK::TPulseGenerator( 7, (m_calibPath+"/tilecalorimeter_pulse_shape.dat").c_str());



  return ErrorCode::SUCCESS;
}



StatusCode CaloCellMaker::finalize()
{
  // Destroy all cells into the collection
  m_collection.release();

  return ErrorCode::SUCCESS;
}





StatusCode CaloCellMaker::pre_execute( EventContext *ctx )
{
  // Zeroize all cells into the collection
  m_collection.clear();

  return ErrorCode::SUCCESS;
}




StatusCode CaloCellMaker::execute( EventContext *ctx )
{
  const G4Step* step = nullptr; 
    
  ctx->retrieve( step , m_inputKey);
  if( !step ){
    MSG_ERROR("It's not possible to retrieve the G4Step.")
    return ErrorCode::FAILURE;
  }
  // Get the position
  G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();

  // Apply all necessary transformation (x,y,z) to (eta,phi,r) coordinates
  // Get ATLAS coordinates (in transverse plane xy)
  auto vpos = TLorentzVector( pos.x(), pos.y(), pos.z(), 0);

  // This object can not be const since we will change the intenal value
  xAOD::CaloCell *cell=nullptr;
  m_collection.retrieve( vpos, cell );
  
  if(cell)  cell->Fill( step );
  
  return ErrorCode::SUCCESS;
}




StatusCode CaloCellMaker::post_execute( EventContext *ctx )
{
  
  xAOD::CaloCellContainer *container=new xAOD::CaloCellContainer();

  for ( const auto& c : m_collection.all() )
  {
    auto cell = c->copy();

    // Generate the pulse for each bunch crossing and integratet
    GeneratePulse( cell );
    // Estimate the cell energy from the pulse
    CalculateEnergy( cell );
    // Transverse energy from integrated energy for all step point
    //cell->setRawEt( cell->rawEnergy()/cosh(fabs(cell->eta())) ); 
    // Transverse energy with cell estimation from pulse
    //cell->setEt( cell->energy()/cosh(fabs(cell->eta())) ); 
    // make this const and add to the container
    container->push_back( cell );
  }

  ctx->attach( container, m_outputKey );
  return ErrorCode::SUCCESS;
}




StatusCode CaloCellMaker::fillHistograms( EventContext *ctx )
{
  const xAOD::CaloCellContainer *container = nullptr; 
  
  auto store = getStoreGateSvc();

  ctx->retrieve( container, m_outputKey );

  if( !container ){
    MSG_ERROR( "It's not possible to retrieve calo cell container");
    return ErrorCode::FAILURE;
  }


  for ( const auto& cell : container->all() ){ 
    // Skip cells with energy equal zero
    std::stringstream ss; ss << "layer_" << (int)cell->sampling();
    int x = store->hist2(ss.str())->GetXaxis()->FindBin(cell->eta());
    int y = store->hist2(ss.str())->GetYaxis()->FindBin(cell->phi());
    int bin = store->hist2(ss.str())->GetBin(x,y,0);
    float energy = store->hist2(ss.str())->GetBinContent( bin );
    // move average
    store->hist2(ss.str())->SetBinContent( bin, (energy + cell->rawEnergy()) );
  }

  return ErrorCode::SUCCESS;
}




void CaloCellMaker::GeneratePulse( xAOD::CaloCell *cell )
{
  auto pulse_size = m_pulseGenerator[cell->layer()]->GetPulseSize();
  
  // Get all energies for each bunch crossing 
  auto rawEnergySamples = cell->rawEnergySamples();
  
  // Create an pulse with zeros with n samples
  std::vector<float> pulse_sum(pulse_size, 0.0);
  // Loop over each bunch crossing
  for ( unsigned bc = m_bc_id_start, i=0;  bc <= m_bc_id_end; ++bc, ++i )
  {
    // Generate the pulse
    auto pulse = m_pulseGenerator[cell->layer()]->GenerateDeterministicPulse( rawEnergySamples[i], 0, bc*m_bc_duration );
    // Add gaussian noise
    m_pulseGenerator[cell->layer()]->AddGaussianNoise(pulse);
    // Accumulate into pulse sum (Sum all pulses)
    for ( unsigned j=0; j < pulse_size; ++j )
      pulse_sum[j] += (float)pulse->operator[](j);
  }

  // Add the pulse centered in the bunch crossing zero
  cell->setPulse( pulse_sum );
}



void CaloCellMaker::CalculateEnergy( xAOD::CaloCell * /*cell*/ )
{

}



