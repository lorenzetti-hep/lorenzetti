

#include "GaugiKernel/Test.h"
#include "GaugiKernel/MsgStream.h"







Test::Test() : IMsgService("Test"), MsgService(MSG::INFO), Gaugi::PropertyService()
{
  MSG_INFO( "Test class" )
}

Test::~Test()
{}



void Test::run()
{
  testDataVector();
}




void Test::testDataVector()
{

  SG::EventContext ctx("TestDatavector");


  {
    SG::WriteHandle< SG::DataVector<Object> >  my_vector( "OBJECTS", ctx );
    my_vector.record( std::unique_ptr<SG::DataVector<Object>> ( new SG::DataVector<Object>() ) );
    my_vector->push_back( new Object(3) );
    my_vector->push_back( new Object(5) );
  }



  {
    SG::ReadHandle< SG::DataVector<Object> > my_vector( "OBJECTS", ctx );

    if (my_vector.isValid())
      MSG_INFO("Read vector with " << my_vector->size());

  }

}


