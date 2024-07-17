
#include "BoostedEvents.h"
#include "helper.h"
#include "TMath.h"
#include "Pythia8/Pythia.h"
#include "../../evtgen/src/Pythia8Gen.h"

using namespace Pythia8;
using namespace generator;

BoostedEvents::BoostedEvents(const std::string name, IGenerator *gen) : IMsgService(name),
                                                                        SingleParticle(name, gen)
{
  declareProperty("Energy", m_energy = -1);
  declareProperty("Particle", m_pdgid = 11);
  declareProperty("HasLifetime", m_hasLifetime = false);
  declareProperty("AtRest", m_atRest = false);
  declareProperty("DeltaR", m_deltaR = 0.2);
  declareProperty("RelEnergyMin", m_relEnergyMin = 0.0); // 0%
  declareProperty("RelEnergyMax", m_relEnergyMax = 2.0); // 200%
}

BoostedEvents::~BoostedEvents()
{
  ;
}

StatusCode BoostedEvents::initialize()
{
  setMsgLevel(m_outputLevel);

  if (generator()->initialize().isFailure())
  {
    MSG_FATAL("Its not possible to initialize the generator. Abort!");
  }

  return StatusCode::SUCCESS;
}

StatusCode BoostedEvents::execute(generator::Event &ctx)
{

  MSG_INFO("Start boosted gun...");

  // TODO: This is a hack. Force to get pythia8 object
  // generator()->core() would not compile
  Pythia8Gen *p8gen = static_cast<Pythia8Gen *>(generator());
  auto *pythia = p8gen->core();
  MSG_INFO("Got generator...");

  // For each seed inside of the event record we will
  // shoot a particle with DeltaR of distance of the
  // seed center. The energy of the particle will be a
  // fraction of the total energy of the seed.
  int initial_size = ctx.size();
  for (int i = 0; i < initial_size; i++)
  {
    generator::Seed seed = ctx[i];
    float eta, phi;
    do
    { // Generate eta/phi until the position is inside of the radio (deltaR)
      // This could be improved in the future by computing the exact intervals instead o trying random values
      eta = seed.eta() + (generator()->random_flat() - m_deltaR);
      phi = seed.phi() + (generator()->random_flat() - m_deltaR);
    } while (dR(eta, phi, seed.eta(), seed.phi()) > m_deltaR);

    // Generate the energy fraction desired to the new seed based on the original seed
    float energy = seed.etot() * (m_relEnergyMax + m_relEnergyMax * generator()->random_flat());
    const auto main_event_t = sample_t();
    const auto main_event_z = sample_z();

    int iparticle = fill(pythia, m_pdgid, energy, eta, phi, m_atRest, m_hasLifetime);
    auto *p = &pythia->event[iparticle];
    pythia->event.popBack();
    auto new_seed = generator::Seed(eta, phi);
    new_seed.emplace_back(1, 0, p->id(), p->px(), p->py(), p->pz(), p->eta(), p->phi(),
                      p->xProd(), p->yProd(), p->zProd() + main_event_z, p->tProd() + main_event_t,
                      p->e(), p->eT());
    ctx.push_back(new_seed);
  }


  // // Generate main event. Quit if too many failures.
  // if (!gun->next())
  // {
  //   if (m_iAbort++ > m_nAbort)
  //   {
  //     MSG_ERROR("Event generation aborted prematurely, owing to error in main event!");
  //     throw AbortPrematurely();
  //   }
  // }

  // double weight = gun->info.mergingWeight();
  // double evtweight = gun->info.weight();
  // weight *= evtweight;

  // // Do not print zero-weight events.
  // if (weight == 0.)
  // {
  //   MSG_WARNING("Pythia generation return weight zero.");
  //   return StatusCode::FAILURE;
  // }

  // for (int i = 0; i < gun->event.size(); ++i)
  // {

  //   auto *p = &gun->event[i];
  //   ParticleData *pdt = &gun->particleData;

  //   // Find any unrecognized particle codes.
  //   int id = p->id();

  //   if (id == 0 || !pdt->isParticle(id))
  //   {
  //     MSG_WARNING("Unknown code id = " << id);
  //     continue;
  //   }

  //   const auto main_event_t = sample_t();
  //   const auto main_event_z = sample_z();

  //   // Study final-state particles.
  //   if (p->isFinal())
  //   {
  //     // Search for the associated seed into the event record
  //     for (auto &seed : *ctx)
  //     {
  //       // Get the associated seed for this generated particle
  //       // Add the particle into the seed particle list
  //       if (dR(p->eta(), p->phi(), seed.eta(), seed.phi()) <= m_deltaR)
  //       {
  //         seed.emplace_back(1, 0, p->id(), p->px(), p->py(), p->pz(), p->eta(), p->phi(),
  //                           p->xProd(), p->yProd(), p->zProd() + main_event_z, p->tProd() + main_event_t,
  //                           p->e(), p->eT());
  //         MSG_INFO("Adding particle: id = " << p->id() << " eta = " << p->eta() << " phi = " << p->phi() << " Et = " << p->eT());
  //       } // delta R inside the limit
  //     } // Loop over seeds
  //   } // Is final
  // } // Loop over particles

  return StatusCode::SUCCESS;
}

float BoostedEvents::dR(float eta1, float phi1, float eta2, float phi2) const
{
  float deta = fabs(eta1 - eta2);
  float dphi = fabs(phi1 - phi2);
  if (dphi >= TMath::Pi())
  {
    dphi = 2 * TMath::Pi() - dphi;
  }
  return sqrt(deta * deta + dphi * dphi);
}

StatusCode BoostedEvents::finalize()
{
  return StatusCode::SUCCESS;
}
