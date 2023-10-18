
#include "CaloCell/CaloCellConverter.h"
#include "GaugiKernel/EDM.h"

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
      cell_t.tau     = cell->tau();
      cell_t.descriptor_link = link;
      return true;
  }
  return false;
}


bool CaloCellConverter::convert( const CaloCell_t &cell_t, CaloCell *&cell )
{
  cell = new xAOD::CaloCell( cell_t.eta, 
                             cell_t.phi, 
                             cell_t.deta, 
                             cell_t.dphi );
  cell->setE(cell_t.e); // estimated energy from OF. Same as descriptor->e()
  cell->setEt(cell_t.et); // estimated transverse energy from OF
  cell->setTau(cell_t.tau);
  return true;
}
