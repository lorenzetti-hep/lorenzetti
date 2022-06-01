#ifndef DataHandle_h
#define DataHandle_h

#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/MsgStream.h"
#include <string>
#include <map>
#include <memory>


namespace SG{


  // Base class for downcast 
  class DataHandle{
    public:
      DataHandle()=default;
      virtual ~DataHandle(){};

  };



  class EventContext : public MsgService
  {
    public:
      
      /*! Contructor */
      EventContext( std::string name );
      
      /*! Destructor */
      ~EventContext();

      /*! Record only Containers of type DataVector<OBJECT> */
      template<class T> void record( std::string &sgkey, std::unique_ptr<T> &container );

      /*! get the pointer given a key */
      template<class T> const T* get( std::string &sgkey );

      /*! Clear all data handlers map */
      void clear();
        
      /*! Set the store gate service */
      void setStoreGateSvc( StoreGate *store ){ m_store=store; }

      /*! Get the store gate service */
      StoreGate* getStoreGateSvc(){ return m_store; };


      void setThreadId( int id ){ m_threadId = id; };
      int getThreadId(){return m_threadId;};

      void setNumberOfThreads(int numberOfThreads) { m_numberOfThreads=numberOfThreads;};
      int getNumberOfThreads(){return m_numberOfThreads;};


      /*! Get all keys into the storage */
      std::vector<std::string> keys() const
      { 
        std::vector<std::string> keys;
        for( auto &it : m_storable_ptr)
        {
          keys.push_back(it.first);
        }
        return keys;
      };


    private:

      std::map< std::string, std::unique_ptr<const DataHandle > > m_storable_ptr;
      
      StoreGate *m_store;

      int m_threadId;
      int m_numberOfThreads;
  };





  template< class T >
  class ReadHandle{

    public:

      ReadHandle( std::string sgkey, EventContext &);
      
      ~ReadHandle();

      /*
       * @brief Dereference the pointer.
       */
      const T* operator->();

      /*
       * @brief Dereference the pointer.
       */
      const T* operator*();


      const T* ptr();

      std::string key();


      bool isValid();


    private:

      // Key used into the storage
      std::string m_sgkey;

      // container pointer
      const T* m_ptr;
      
      // Used to check if pointer was read
      bool m_isValid; 
  };


  


  template< class T >
  class WriteHandle{

    public:

      WriteHandle( std::string sgkey, EventContext & );

      WriteHandle();

      ~WriteHandle();

      /*
       * @brief Dereference the pointer.
       */
      T* operator->();

      /*
       * @brief Dereference the pointer.
       */
      T& operator*();

      /*
       * @brief tranfer the pointer to write handle
       */ 
      void record( std::unique_ptr<T> ptr );

      /*
       * @brief Get the storable key
       */ 
      std::string key();

    private:

      /*! Storage key */
      std::string m_sgkey;
      /*! Hold the container until this object will to out of scope */
      std::unique_ptr<T> m_ptr;
      /*! Hold the event context. The destructor will pass the pointer to the event context */
      EventContext *m_ctx;
  };




}

#include "DataHandle.icc"

#endif

