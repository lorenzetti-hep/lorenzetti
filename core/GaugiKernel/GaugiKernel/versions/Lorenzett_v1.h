#ifndef LORENZETT_V1_H
#define LORENZETT_V1_H

#include <TROOT.h>
#include "Rtypes.h"
// Header file for the classes stored in the TTree if any.
#include "vector"


namespace edm{


struct Lorenzett_v1 {

  // Create all local variables since this must be a const method
  int EventNumber         ;
  float avgmu             ;
  float seed_eta          ;
  float seed_phi          ;
  float seed_et           ;
  bool  mc_cl_match       ;
  float mc_cl_eta         ;
  float mc_cl_phi         ;
  float mc_cl_et          ;
  float mc_cl_e1          ;
  float mc_cl_e2          ;
  float mc_cl_e3          ;
  float mc_cl_ehad1       ;
  float mc_cl_ehad2       ;
  float mc_cl_ehad3       ;
  float mc_cl_etot        ;
  float mc_cl_reta        ;
  float mc_cl_rphi        ;
  float mc_cl_rhad        ;
  float mc_cl_eratio      ;
  float mc_cl_f0          ;
  float mc_cl_f1          ;
  float mc_cl_f2          ;
  float mc_cl_f3          ;
  float mc_cl_weta2       ;
  float mc_cl_e233        ;
  float mc_cl_e237        ;
  float mc_cl_e277        ;
  float mc_cl_emaxs1      ;
  float mc_cl_e2tsts1     ;
  bool  mc_cl_ringer_match;
  bool  cl_match          ;
  float cl_eta            ;
  float cl_phi            ;
  float cl_et             ;
  float cl_e1             ;
  float cl_e2             ;
  float cl_e3             ;
  float cl_ehad1          ;
  float cl_ehad2          ;
  float cl_ehad3          ;
  float cl_etot           ;
  float cl_reta           ;
  float cl_rphi           ;
  float cl_rhad           ;
  float cl_eratio         ;
  float cl_f0             ;
  float cl_f1             ;
  float cl_f2             ;
  float cl_f3             ;
  float cl_weta2          ;
  float cl_e233           ;
  float cl_e237           ;
  float cl_e277           ;
  float cl_emaxs1         ;
  float cl_e2tsts1        ;
  bool  cl_ringer_match   ;

  std::vector<float> *mc_cl_rings       ;
  std::vector<float> *cl_rings          ;


  std::vector<float> *mc_cl_cell_et           ;
  std::vector<float> *mc_cl_cell_eta          ;
  std::vector<float> *mc_cl_cell_phi          ;
  std::vector<float> *mc_cl_cell_deta         ;
  std::vector<float> *mc_cl_cell_dphi         ;
  std::vector<float> *mc_cl_cell_energy       ;
  std::vector<int>   *mc_cl_cell_channel_eta  ;
  std::vector<int>   *mc_cl_cell_channel_phi  ;
  std::vector<int>   *mc_cl_cell_sampling     ;
  
  
  std::vector<float> *cl_cell_et              ;
  std::vector<float> *cl_cell_eta             ;
  std::vector<float> *cl_cell_phi             ;
  std::vector<float> *cl_cell_deta            ;
  std::vector<float> *cl_cell_dphi            ;
  std::vector<float> *cl_cell_energy          ;
  std::vector<int>   *cl_cell_channel_eta     ;
  std::vector<int>   *cl_cell_channel_phi     ;
  std::vector<int>   *cl_cell_sampling        ;


};

}
#endif //
