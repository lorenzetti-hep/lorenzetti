
#include "CaloRec/CaloCellMaker.h"
#include "TLorentzVector.h"



CaloCellMaker::CaloCellMaker( std::string name ) : 
  AlgTool( name ),
  m_card("data/detector.card"), 
  m_bc_id_start( 0),
  m_bc_id_end( 0),
  m_bc_nsamples( 1),
  m_bc_duration(25),
  m_shaper_nsamples( 1),
  m_shaper_card()
{;}



CaloCellMaker::~CaloCellMaker()
{;}




StatusCode CaloCellMaker::initialize()
{
  auto store = getStoreGateSvc();

  // This configuration is fixed for each event and can not be changed
  float start = m_bc_id_start * m_bc_duration;
  float step  = m_bc_duration / m_bc_nsamples;
  int nbins   = (m_bc_id_end - m_bc_id_start) + 1;

  std::vector<float> tbins;
  for (int b=0; b< nbins; ++b){
    tbins.push_back( (start + step*b) );
  }
  
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
      m_collection.push_back( cell );
    
    // Layer configuration
    }else if (command=="L"){

      int sampling;
		  float  eta_min, eta_max, eta_bins, phi_min, phi_max, phi_bins, rmin, rmax;
      file >> sampling >> eta_min >> eta_max >> eta_bins >> phi_min >> phi_max >> phi_bins >> rmin >> rmax;
      // Create the layer accessor to generate the correct cell hash
      auto *acc = new xAOD::CaloCellAccessor(eta_min, eta_max, eta_bins, phi_min, phi_max, phi_bins, rmin, rmax, sampling );
      m_collection.push_back(acc);
     
      // Crrate the histogram name
      std::stringstream ss; ss << "layer_" << sampling;
      
      // Create the histogram
      store->AddHistogram(ss.str(), "", eta_bins, eta_min, eta_max, phi_bins, phi_min, phi_max);
    }
	}
  file.close();



  // Pulse generator from: https://gitlab.cern.ch/ginaciog/calopulsekit
  // Initalize the pulse generator for eletromagnetic and hadronic calorimeters
  // Add shaper for LAr (eletromagnetic calorimeter)
  m_pulseGenerator[ CaloSampling::CaloLayer::LAr ] new ROOT::CPK::TPulseGenerator(m_em_nsamples, m_emshaper_card)
  // Add shaper for hadronic layer
  m_pulseGenerator[ CaloSampling::CaloLayer::Tile] new ROOT::CPK::TPulseGenerator(m_em_nsamples, m_emshaper_card)


  return SUCCESS;
}



StatusCode CaloCellMaker::finalize()
{
  // Destroy all cells into the collection
  m_collection->release();
  delete m_emPulseGenerator;
  delete m_hadPulseGenerator;
  return SUCCESS;
}





StatusCode CaloCellMaker::pre_execute( EventContext *ctx )
{
  // Zeroize all cells into the collection
  m_collection->clear();

  return SUCCESS;
}




StatusCode CaloCellMaker::execute( EventContext *ctx )
{
  const G4Step* step = nullptr; 
    
  ctx->retrieve( step );
  if( !step ){
    MSG_ERROR("It's not possible to retrieve the G4Step.")
    return FAILURE;
  }
  // Get the position
  G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();

  // Apply all necessary transformation (x,y,z) to (eta,phi,r) coordinates
  // Get ATLAS coordinates (in transverse plane xy)
  auto vpos = TLorentzVector( pos.x(), pos.y(), pos.z(), 0);

  // This object can not be const since we will change the intenal value
  xAOD::CaloCell *cell=nullptr;
  m_collection->retrieve( vpos, cell );
  
  if(cell)  cell->Fill( step );
  
  return SUCCESS;
}





StatusCode CaloCellMaker::post_execute( EventContext *ctx )
{
  
  xAOD::CaloCellContainer *container=nullptr;


  // Calculate the pulse and estimate the energy
  for ( const auto& c : m_collection )
  {
    xAOD::CaloCell *cell = c->clone();

    GeneratePulse( cell );
    CalculateEnergy( cell );

    container->push_back( cell );
  }



  ctx->attach( cell, "xAOD__CaloCellContainer__Digitalized" );

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
    // Skip cells with energy equal zero
    if (cell->rawEnergy()==0)  continue; 

    std::stringstream ss; ss << "layer_" << (int)cell->sampling();
    int x = store->hist2(ss.str())->GetXaxis()->FindBin(cell->eta());
    int y = store->hist2(ss.str())->GetYaxis()->FindBin(cell->phi());
    int bin = store->hist2(ss.str())->GetBin(x,y,0);
    float energy = store->hist2(ss.str())->GetBinContent( bin );
    // move average
    store->hist2(ss.str())->SetBinContent( bin, (energy + cell->rawEnergy())/2 );
  }
  return SUCCESS;
}




void CaloCellMaker::GeneratePulse( xAOD::CaloCell *cell )
{
  // Get all energies for each bunch crossing 
  auto rawEnergySamples = cell->rawEnergySamples();
  // Create an pulse with zeros with n samples
  std::vector<float> pulse_sum(m_shaper_nsamples, 0.0);
  // Loop over each bunch crossing
  for ( unsigned bc = m_bc_id_stard, i=0;  bc <= m_bc_id_end; ++bc, ++i )
  {
    // Generate the pulse
    auto pulse = m_pulseGenerator[cell->layer()]->GenerateDeterministicPulse( rawEnergySamples[i], 0, bc*m_bc_duration );
    // Add gaussian noise
    m_pulseGenerator[cell->layer()]->AddGaussianNoise(pulse);
    // Accumulate into pulse sum (Sum all pulses)
    for ( unsigned j=0; j < m_shaper_nsamples; ++j )
      pulse_sum[j] += (float)pulse[j];
  }
  // Add the pulse into the cell
  cell->setPulse( pulse_sum );
}



void CaloCellMaker::CalCulateEnergy( xAOD::CaloCell * /*cell*/ )
{

}




