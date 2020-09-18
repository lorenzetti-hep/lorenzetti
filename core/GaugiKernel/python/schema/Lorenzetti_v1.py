
__all__ = ["Lorenzetti_v1"]

def Lorenzetti_v1():

  code = """
  namespace edm{
  struct Lorenzetti_v1 {
  
    int EventNumber         ;
    float avgmu             ;
    float seed_eta          ;
    float seed_phi          ;
    float seed_et           ;
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
    std::vector<float> *cl_rings          ;
    std::vector<float> *cl_cell_et              ;
    std::vector<float> *cl_cell_eta             ;
    std::vector<float> *cl_cell_phi             ;
    std::vector<float> *cl_cell_deta            ;
    std::vector<float> *cl_cell_dphi            ;
    std::vector<float> *cl_cell_energy          ;
    std::vector<int>   *cl_cell_layer           ;
  };
  }"""

  import ROOT
  ROOT.gInterpreter.ProcessLine(code)

  from ROOT import edm
  return edm.Lorenzetti_v1()



