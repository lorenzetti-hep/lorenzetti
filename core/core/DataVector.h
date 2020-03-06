#ifndef DataVector_h
#define DataVector_h

/** standard includes **/
#include <vector>


template<class OBJECT>
class DataVector{
  
  public:
    /** Constructor **/
    DataVector(){};
    /** Destructor **/
    ~DataVector(){
      for ( auto& obj : m_data )
        if(obj) delete obj;
      m_data.clear();
    };
    
    /** Return the number of objects into this container **/
    size_t size() const {return m_data.size();};
    /** Clear and destroy all objects **/
    void clear() {m_data.clear();}; 
    /** Add a new object into the container **/
    void push_back( OBJECT *obj) {m_data.push_back(obj);};
    /** Get the standard vector **/
    std::vector<OBJECT*> all() const {return m_data;};

  private:

    // Container vector
    std::vector<OBJECT*> m_data;

};
#endif
