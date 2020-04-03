#ifndef DataVector_h
#define DataVector_h

#include <vector>
#include <memory>

namespace SG
{

  // Base class for downcast 
  class DataVectorBase{
    public:
      DataVectorBase(){};
      ~DataVectorBase(){};
  };


  template<class T>
  class DataVector : public DataVectorBase{  
    public:
      
      /*! Constructor */
      DataVector() = default;

      /*! Destructor **/
      ~DataVector();
      
      /*! Return the number of objects into this container */
      size_t size() const;
      
      /*! Add a new object into the container */
      void push_back( const T* obj );

    private:

      /*! Hold all object pointers */
      //std::vector< std::unique_ptr<T> > m_data;
      std::vector< const T* > m_data;
  };


  template<class T>
  inline DataVector<T>::~DataVector(){}
  
  
  
  template<class T>
  inline size_t DataVector<T>::size() const
  {
    return m_data.size();
  }
  
  template<class T>
  inline void DataVector<T>::push_back( const T* obj )
  {
    m_data.push_back( obj );
    //m_data.push_back( std::unique_ptr<T>(obj) );
  }


}
#endif






