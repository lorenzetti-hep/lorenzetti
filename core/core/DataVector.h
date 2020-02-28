#ifndef DataVector_h
#define DataVector_h



/** standard includes **/
#include <vector>




namespace xAOD{

  template<class OBJECT>
  class DataVector{
    
    public:
      /** Constructor **/
      DataVector();
      /** Destructor **/
      ~DataVector();
      /** Return the number of objects into this container **/
      size_t size();
      /** Clear and destroy all objects **/
      void clear(); 
      /** Add a new object into the container **/
      void push_back( OBJECT );
      /** release all memory **/
      void release();
      /** Get the standard vector **/
      std::vector<OBJECT> cptr();


    private:

      // Container vector
      std::vector<OBJECT> m_data;

  };
}
#endif
