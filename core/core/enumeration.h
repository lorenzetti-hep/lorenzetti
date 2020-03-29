
#ifndef enumeration_h
#define enumeration_h


// Event Object Data
namespace CaloSampling{

  enum CaloSample{
    PS = 0,
    EM1= 1,
    EM2= 2, 
    EM3= 3,
    HAD1=4, 
    HAD2=5, 
    HAD3=6,
    HAD1_Extended=7,
    HAD2_Extended=8,
    HAD3_Extended=9
  };


  enum CaloLayer{
    ECal = 1,
    HCal = 2 
  };

} 

#endif
