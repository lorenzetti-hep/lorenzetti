#ifndef Property_h
#define Property_h

#include <string>
#include <map>


namespace Gaugi{

  // Base property class to be used as any type into the generic type of properties
  class PropertyBase{
    public:
      virtual ~PropertyBase() = 0;
  };
  
  
  // The property class will hold the pointer of the variable
  template <typename T>
  class Property : public PropertyBase
  {
    public:
      Property( T& val ) : m_ptr(&val) {};
      ~Property() = default;
      void setValue( T& val ){ *m_ptr=val; };
      T getValue() { return *m_ptr; };
  
    private:
      T *m_ptr;
  };
  
  
  // Base class for proeprty service 
  class PropertyService
  {
    public:
 
      /* Contructor */
      PropertyService(){};
      ~PropertyService()=default;
  
      /* Declare */
      void declareProperty( std::string name, int                       &value );
      void declareProperty( std::string name, float                     &value );
      void declareProperty( std::string name, double                    &value );
      void declareProperty( std::string name, bool                      &value );
      void declareProperty( std::string name, std::string               &value );
      void declareProperty( std::string name, std::vector<int>          &value );
      void declareProperty( std::string name, std::vector<bool>         &value );
      void declareProperty( std::string name, std::vector<float>        &value );
      void declareProperty( std::string name, std::vector<std::string>  &value );
      void declareProperty( std::string name, std::vector<std::vector<int>>          &value );
      
      /* Setter */
      void setProperty( std::string name, int                       value );
      void setProperty( std::string name, float                     value );
      void setProperty( std::string name, double                    value );
      void setProperty( std::string name, bool                      value );
      void setProperty( std::string name, std::string               value );
      void setProperty( std::string name, std::vector<int>          value );
      void setProperty( std::string name, std::vector<bool>         value );
      void setProperty( std::string name, std::vector<float>        value );
      void setProperty( std::string name, std::vector<std::string>  value );
      void setProperty( std::string name, std::vector<std::vector<int>>  value );

      /* Getter */
      void getProperty( std::string name, int                       &value );
      void getProperty( std::string name, float                     &value );
      void getProperty( std::string name, double                    &value );
      void getProperty( std::string name, bool                      &value );
      void getProperty( std::string name, std::string               &value );
      void getProperty( std::string name, std::vector<int>          &value );
      void getProperty( std::string name, std::vector<bool>         &value );
      void getProperty( std::string name, std::vector<float>        &value );
      void getProperty( std::string name, std::vector<std::string>  &value );
      void getProperty( std::string name, std::vector<std::vector<int>>          &value );
  
      bool hasProperty( std::string name);


    private:

      /* Template specification for any type of declare property */
      template<typename T> void __declareProperty( std::string name, T &value );
      /* Template specification for any type of set property */
      template<typename T> void __setProperty( std::string name, T &value );
      /* Template specification for any type of get property */
      template<typename T> void __getProperty( std::string name, T &value );
      // Dictionary of properties
      std::map<std::string, std::unique_ptr<PropertyBase>> m_properties;
  };
  
  
   
  inline PropertyBase::~PropertyBase(){}
  
  /** Declare property **/

  template<class T> inline void PropertyService::__declareProperty( std::string name, T &value ) 
  {
    auto property = new Property<T>(value); 
    m_properties.insert( std::pair<std::string, std::unique_ptr<PropertyBase>>(name,property) );
  }
   
  inline void PropertyService::declareProperty( std::string name, int          &value              ){__declareProperty<int>(name,value);}
  inline void PropertyService::declareProperty( std::string name, float        &value              ){__declareProperty<float>(name,value);}
  inline void PropertyService::declareProperty( std::string name, double       &value              ){__declareProperty<double>(name,value);}
  inline void PropertyService::declareProperty( std::string name, bool         &value              ){__declareProperty<bool>(name,value);}
  inline void PropertyService::declareProperty( std::string name, std::string  &value              ){__declareProperty<std::string>(name,value);}
  inline void PropertyService::declareProperty( std::string name, std::vector<int>          &value ){__declareProperty<std::vector<int>>(name,value);}
  inline void PropertyService::declareProperty( std::string name, std::vector<bool>         &value ){__declareProperty<std::vector<bool>>(name,value);}
  inline void PropertyService::declareProperty( std::string name, std::vector<float>        &value ){__declareProperty<std::vector<float>>(name,value);}
  inline void PropertyService::declareProperty( std::string name, std::vector<std::string>  &value ){__declareProperty<std::vector<std::string>>(name,value);}
  inline void PropertyService::declareProperty( std::string name, std::vector<std::vector<int>>  &value ){__declareProperty<std::vector<std::vector<int>>>(name,value);}



  
  /** Setter **/

  template<typename T> inline void PropertyService::__setProperty( std::string name, T &value )
  {
    if (m_properties.find(name) != m_properties.end()) {
      dynamic_cast<Property<T>&>(*m_properties[name]).setValue(value);
    }else{
      throw std::runtime_error("set property with name " + name + " failed. the property does not exist."); 
    }
  }
  
  inline void PropertyService::setProperty( std::string name, int                       value ){__setProperty<int>(name,value);}
  inline void PropertyService::setProperty( std::string name, float                     value ){__setProperty<float>(name,value);}
  inline void PropertyService::setProperty( std::string name, double                    value ){__setProperty<double>(name,value);}
  inline void PropertyService::setProperty( std::string name, bool                      value ){__setProperty<bool>(name,value);}
  inline void PropertyService::setProperty( std::string name, std::string               value ){__setProperty<std::string>(name,value);}
  inline void PropertyService::setProperty( std::string name, std::vector<int>          value ){__setProperty<std::vector<int>>(name,value);}
  inline void PropertyService::setProperty( std::string name, std::vector<bool>         value ){__setProperty<std::vector<bool>>(name,value);}
  inline void PropertyService::setProperty( std::string name, std::vector<float>        value ){__setProperty<std::vector<float>>(name,value);}
  inline void PropertyService::setProperty( std::string name, std::vector<std::string>  value ){__setProperty<std::vector<std::string>>(name,value);}
  inline void PropertyService::setProperty( std::string name, std::vector<std::vector<int>>  value ){__setProperty<std::vector<std::vector<int>>>(name,value);}

  
  /** Getter **/

  template<typename T> inline void PropertyService::__getProperty( std::string name, T &value )
  {
    if (m_properties.find(name) != m_properties.end()) {
      value = dynamic_cast<Property<T>&>(*m_properties[name]).getValue(); 
    }else{
      throw std::runtime_error("get property with name " + name + " failed. the property does not exist."); 
    }

  }
   
  inline void PropertyService::getProperty( std::string name, int                       &value ){__getProperty<int>(name,value);}
  inline void PropertyService::getProperty( std::string name, float                     &value ){__getProperty<float>(name,value);}
  inline void PropertyService::getProperty( std::string name, double                    &value ){__getProperty<double>(name,value);}
  inline void PropertyService::getProperty( std::string name, bool                      &value ){__getProperty<bool>(name,value);}
  inline void PropertyService::getProperty( std::string name, std::string               &value ){__getProperty<std::string>(name,value);}
  inline void PropertyService::getProperty( std::string name, std::vector<int>          &value ){__getProperty<std::vector<int>>(name,value);}
  inline void PropertyService::getProperty( std::string name, std::vector<bool>         &value ){__getProperty<std::vector<bool>>(name,value);}
  inline void PropertyService::getProperty( std::string name, std::vector<float>        &value ){__getProperty<std::vector<float>>(name,value);}
  inline void PropertyService::getProperty( std::string name, std::vector<std::string>  &value ){__getProperty<std::vector<std::string>>(name,value);}
  inline void PropertyService::getProperty( std::string name, std::vector<std::vector<int>>  &value ){__getProperty<std::vector<std::vector<int>>>(name,value);}



  inline bool PropertyService::hasProperty( std::string name ){
    if (m_properties.find(name) != m_properties.end()) {
      return true; 
    }
    return false;
  }


}//namespace
#endif



