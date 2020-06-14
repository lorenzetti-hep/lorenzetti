

#include <iostream>
#include "P8Kernel/ParticleFilter.h"

using namespace std;
using namespace generator;


void ParticleFilter::filter(Pythia8::Event& event) {

  // Reset arrays in preparation for new event.
  m_keptIndx.resize(0);
  m_keptPtrs.resize(0);
  // Loop over all particles in the event record.
  for (int i = 0; i < event.size(); ++i) {
    // Skip if particle kind selection criteria not fulfilled.
    if (!event[i].isFinal()) continue;
    if (m_select == 2 && !event[i].isVisible()) continue;
    bool isCharged = event[i].isCharged();
    if (m_select == 3 && !isCharged) continue;
    // Skip if too large pseudorapidity.
    if (abs(event[i].eta()) > m_etaMax) continue;
    // Skip if too small pT.
    if       (isCharged && event[i].pT() < m_pTminCharged) continue;
    else if (!isCharged && event[i].pT() < m_pTminNeutral) continue;
    // Add particle to vectors of indices and pointers.
    m_keptIndx.push_back( i );
    m_keptPtrs.push_back( &event[i] );
  // End of particle loop. Done.
  }
}


void ParticleFilter::list(ostream& os) {

  // Header.
  os << "\n --------  PYTHIA Event Listing  (filtered)  ------------------"
     << "-----------------------------------------------------------------"
     << "----\n \n    no        id   name            status     mothers  "
     << " daughters     colours      p_x        p_y        p_z         e  "
     << "        m \n";

  // At high energy switch to scientific format for momenta.
  double eSum = 0.;
  for (int iKept = 0; iKept < size(); ++iKept) eSum += m_keptPtrs[iKept]->e();
  bool useFixed = (eSum < 1e5);

  // Listing of kept particles in event.
  for (int iKept = 0; iKept < size(); ++iKept) {
    int i = m_keptIndx[iKept];
    Pythia8::Particle& pt = *m_keptPtrs[iKept];

    // Basic line for a particle, always printed.
    os << setw(6) << i << setw(10) << pt.id() << "   " << left
       << setw(18) << pt.nameWithStatus(18) << right << setw(4)
       << pt.status() << setw(6) << pt.mother1() << setw(6)
       << pt.mother2() << setw(6) << pt.daughter1() << setw(6)
       << pt.daughter2() << setw(6) << pt.col() << setw(6) << pt.acol()
       << ( (useFixed) ? fixed : scientific ) << setprecision(3)
       << setw(11) << pt.px() << setw(11) << pt.py() << setw(11)
       << pt.pz() << setw(11) << pt.e() << setw(11) << pt.m() << "\n";
  }

  // Listing finished.
  os << "\n --------  End PYTHIA Event Listing  ----------------------------"
     << "-------------------------------------------------------------------"
     << endl;
}



