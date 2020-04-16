#ifndef DataVector_h
#define DataVector_h


#include "GaugiKernel/DataHandle.h"
#include <vector>
#include <memory>
#include <iostream>

namespace SG
{

  template<class T>
  class DataVector : public DataHandle{  
    public:
      
      /*! Constructor */
      DataVector() = default;

      /*! Destructor **/
      ~DataVector();
      
      /*! Return the number of objects into this container */
      size_t size() const;
      
      /*! Add a new object into the container */
      void push_back( const T* obj );


      const std::vector<const T*>& operator*() const;


    private:

      /*! Hold all object pointers */
      //std::vector< std::unique_ptr<T> > m_data;
      std::vector< const T* > m_data;
  };

  
  
  template<class T>
  DataVector<T>::~DataVector<T>()
  {
    for(auto obj : m_data )
    {
      if(obj) delete obj;
    }
  }

  
  template<class T>
  inline size_t DataVector<T>::size() const
  {
    return m_data.size();
  }
  
  template<class T>
  inline void DataVector<T>::push_back( const T* obj )
  {
    m_data.push_back( obj );
  }

  template<class T>
  inline const std::vector<const T*>& DataVector<T>::operator*() const
  {
    return m_data;
  }
}
#endif






