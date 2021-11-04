
#ifndef Collection_h
#define Collection_h

#include "GaugiKernel/DataHandle.h"
#include <map>
#include <memory>
#include <iostream>


namespace SG{

  template<class T>
  class Collection : public SG::DataHandle
  {  
    public:

      /*! Contructor */
      Collection()=default;
      
      /*! Destructor */
      ~Collection();
      
      /*! Add a new calo cell into the collection */
      bool insert( unsigned long int id, T* );

      bool retrieve( unsigned long int id, T*&obj ) const;
      
      const std::map<unsigned long int, T* >& operator*() const;

    private:

      /*! Hold all object pointers */
      std::map<unsigned long int, T* > m_collection;
  };

  template<class T>
  Collection<T>::~Collection<T>()
  {
    for(auto& p : m_collection)
      if(p.second)  delete p.second;
    m_collection.clear();
  }

  template<class T>
  inline bool Collection<T>::insert( unsigned long int id, T*obj)
  {
    if ( m_collection.count(id) ){
      return false;
    }
    m_collection.insert( std::make_pair( id, obj ) );
    return true;
  }

  template<class T>
  inline bool Collection<T>::retrieve( unsigned long int id, T*&obj ) const
  {
    if (m_collection.count(id)>0){
        obj = m_collection.at(id);
        return true;
    }else{
      return false;
    }
  }

  template<class T>
  inline const std::map<unsigned long int, T* >& Collection<T>::operator*() const
  {
    return m_collection;
  }

}

#endif