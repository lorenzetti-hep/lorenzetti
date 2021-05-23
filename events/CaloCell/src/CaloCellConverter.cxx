
#include "CaloCell/CaloCellConverter.h"
#include "GaugiKernel/EDM.h"

//#include "G4Kernel/macros.h"


using namespace xAOD;



bool CaloCellConverter::convert( const CaloCell *cell, CaloCell_t &cell_t, int link)
{
  if(cell){
      cell_t.e       = cell->e();
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