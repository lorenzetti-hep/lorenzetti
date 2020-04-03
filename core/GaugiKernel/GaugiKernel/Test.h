#ifndef test_h
#define test_h

#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/MsgStream.h"
#include "GaugiKernel/defines.h"
#include "GaugiKernel/Property.h"
#include <iostream>
#include <string>


class Object{
  public:
    Object( int val): m_value(val){};
    ~Object(){};

    int get(){ return m_value;};
  private:
    int m_value;
};


typedef SG::DataVector<Object> Container;



class Test: public MsgService, public Gaugi::PropertyService
{
  public:

    Test ();
    ~Test();

    void run();
  
  private:

    void testDataVector();
};


#endif
