#ifndef MsgStream_h
#define MsgStream_h


#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include "GaugiKernel/defines.h"
#include "G4Threading.hh"

/**
 * Macro to be used within MsgService inherited classes.
 *
 * It will check if message is above level before requesting to enter it into
 * MsgStreamMirror to check it.
 **/
#define MSG_LVL_CHK(xmsg, lvl)  do { \
  if ( msgLevel( lvl ) ) { \
    if (G4Threading::G4GetThreadId() < 0 ) \
      msg() << lvl << xmsg << endreq; \
    else\
      msg() << lvl <<  " (G4WT"<< G4Threading::G4GetThreadId() << ") "  <<xmsg << endreq; \
  } \
} while (0);

/**
 * Macro for check and displaying DEBUG messages.
 *
 **/
# define MSG_DEBUG(xmsg) MSG_LVL_CHK( xmsg, ::MSG::DEBUG )

/**
 * Macro for displaying INFO messages
 **/
#define MSG_INFO(xmsg)    MSG_LVL_CHK( xmsg, ::MSG::INFO    )

/**
 * Macro for displaying WARNING messages
 **/
#define MSG_WARNING(xmsg) MSG_LVL_CHK( xmsg, ::MSG::WARNING )

/**
 * Macro for displaying VERBOSE messages
 **/
#define MSG_ERROR(xmsg)   MSG_LVL_CHK( xmsg, ::MSG::ERROR   )

/**
 * Macro for displaying FATAL messages
 *
 * It will also raise a std::runtime_error with same message.
 **/
#define MSG_FATAL(xmsg)                                 \
  msg() << MSG::FATAL << xmsg;                          \
  auto e = std::runtime_error( msg().stream().str() );  \
  msg().doOutput();                                     \
  throw e;

namespace MSG
{

/**
 * The multiple message levels available
 **/
enum Level {
  NIL = 0,
  VERBOSE = 1,
  DEBUG   = 2,
  INFO    = 3,
  WARNING = 4,
  ERROR   = 5,
  FATAL   = 6
};

} // namespace MSG

/**
 * @brief Get C string from enum
 **/
//==============================================================================
inline
const char* to_str(const MSG::Level lvl)
{
  switch (lvl){
    case MSG::VERBOSE:
      return "VERBOSE";
    case MSG::DEBUG:
      return "DEBUG";
    case MSG::INFO:
      return "INFO";
    case MSG::WARNING:
      return "WARNING";
    case MSG::ERROR:
      return "ERROR";
    case MSG::FATAL:
      return "FATAL";
    default:
      throw std::runtime_error("Couldn't determine LEVEL enumeration.");
  }
}

/**
 * @brief Overload ostringstream to accept MSG::Level
 *
 * In fact, we only need this prototype for the compile to accept the macros
 * declared above. It is implemented only for convinience, as it should be the
 * wanted behavior in case anyone decides to enter the MSG::Level accidently to
 * an ostringstream.
 **/
inline
std::ostringstream& operator<<(std::ostringstream &s, 
                               MSG::Level /*level*/)
{
  return s;
}

/**
 * @brief Overload operator << to accept display vectors
 **/
template <typename T>                                                      
std::ostream& operator<< ( std::ostream& out, const std::vector< T >& vec )
{                                                                          
  // A little prefix:                                                      
  out << "[";                                                              
  // Print the contents:                                                   
  for( size_t i = 0; i < vec.size(); ++i ) {                               
     out << vec[ i ];                                                      
     if( i < vec.size() - 1 ) {                                            
        out << ", ";                                                       
     }                                                                     
  }                                                                        
  // A little postfix:                                                     
  out << "]";                                                              
  // Return the stream:                                                    
  return out;                                                              
}                                                                          

/**
 * @brief Adds logging capability to inherited classes
 *
 * Based on Athena framework's MsgStream (almost a copy, but without using
 * Gaudi infrastructure). It cannot have the same name as the Asg class,
 * otherwise we'll have conflicts on PyROOT usage.
 **/
class MsgStreamMirror
{

  public:

    /**
     * @brief Helper class for displaying MsgStreamMirror messages
     **/
    class Message {
      public: 
        /// Number of charatecters before displaying message
        static constexpr unsigned space_between_log_and_msg = 45;

        ///
        static constexpr const char* color[] = { 
                                                GAUGI_WHITE,
                                                GAUGI_GRAY,
                                                GAUGI_CYAN,
                                                GAUGI_GREEN,
                                                GAUGI_YELLOW,
                                                GAUGI_RED,
                                                GAUGI_BOLDRED,
                                               };

        /// Ctor
        explicit Message( const std::string &logName, 
                 const MSG::Level level,
                 const bool useColor,
                 const std::string &message ) 
          : m_formatted_msg("")
        {
          if (useColor){ m_formatted_msg += color[level]; }
          const char* lvl_str = to_str(level);
          const size_t end_log_name = space_between_log_and_msg 
                                      - (std::strlen(lvl_str) + 2 );
          m_formatted_msg += "Cxx.";
          m_formatted_msg += logName.substr(0, end_log_name - 4 );
          m_formatted_msg.append( end_log_name 
                                  - m_formatted_msg.length() + ((useColor)?5:0) + 1
                                  , 0x20);
          m_formatted_msg += lvl_str;
          m_formatted_msg += 0x20;
          m_formatted_msg += message;
          if (useColor){ m_formatted_msg += GAUGI_RESET; }
        }

        /// Overloads std::cout printing capabilities
        friend std::ostream &operator<<( 
            std::ostream &stream, 
            Message &msg );

      private:
        /// The formated message
        std::string m_formatted_msg;
    };

    /// Ctor using standard configuration
    /// @{
    MsgStreamMirror() 
      : m_streamName("RingerCore_Log"),
        m_level(MSG::INFO),
        m_currentLevel(MSG::INFO),
        m_useColor(false),
        m_active(true){;}

    /// Ctor using integer for setting logname and msgLevel
    explicit MsgStreamMirror(const std::string &logname, const int msgLevel)
      : m_streamName(logname),
        m_level( ( msgLevel > MSG::FATAL ) ? ( MSG::FATAL )
                                           : ( ( msgLevel < MSG::VERBOSE ) ? ( MSG::VERBOSE )
                                                                           : ( static_cast<MSG::Level>( msgLevel ) ) 
                                             )
               ),
        m_currentLevel(MSG::INFO),
        m_useColor(false),
        m_active( m_level <= m_currentLevel )
        {;}

    /// Ctor setting logname and msgLevel
    explicit MsgStreamMirror(const std::string &logname, const MSG::Level msgLevel)
      : m_streamName(logname),
        m_level(msgLevel),
        m_currentLevel(MSG::INFO),
        m_useColor(false),
        m_active(m_level <= m_currentLevel)
        {;}

    /// Ctor using integer for setting logname and msgLevel and color
    explicit MsgStreamMirror(const std::string &logname, const int msgLevel, const bool useColor)
      : m_streamName(logname),
        m_level( ( msgLevel > MSG::FATAL ) ? ( MSG::FATAL )
                                           : ( ( msgLevel < MSG::VERBOSE ) ? ( MSG::VERBOSE )
                                                                           : ( static_cast<MSG::Level>( msgLevel ) ) 
                                             )
               ),
        m_currentLevel(MSG::INFO),
        m_useColor(useColor),
        m_active( m_level <= m_currentLevel )
        {;}

    /// Ctor setting logname and msgLevel
    explicit MsgStreamMirror(const std::string &logname, const MSG::Level msgLevel, const bool useColor)
      : m_streamName(logname),
        m_level(msgLevel),
        m_currentLevel(MSG::INFO),
        m_useColor(useColor),
        m_active(m_level <= m_currentLevel)
        {;}

    /// Copy constructor
    MsgStreamMirror(const MsgStreamMirror& msg)
      : m_streamName(msg.m_streamName),
        m_level(msg.m_level),
        m_currentLevel(msg.m_currentLevel),
        m_useColor(msg.m_useColor),
        m_active(msg.m_active)
    {
      try { // ignore exception if we cannot copy the string
        m_stream.flags( msg.m_stream.flags() );
      } catch (...) {;}
    }
    /// @}

    /// Inline methods
    /// @{
    /// Retrieve current message level
    MSG::Level level() const
    { 
      return m_level;
    }

    /// Retrieve a reference to current logname
    const std::string& logName() const 
    {
      return m_streamName;
    }

    /// Change logger level
    bool useColor(){
      return m_useColor;
    }
    
    /// Change logger level
    void setLevel(MSG::Level msgLevel){
      if ( msgLevel != m_level ){
        m_level = msgLevel;
        // Report change of level:
        report(m_currentLevel);
      }
    }
    /// Change logger level
    void setUseColor(const bool useColor){
      m_useColor = useColor;
    }

    /// Change this logger name
    void setLogName(const std::string &logName)
    {
      m_streamName = logName;
    }

    /// oMsgStreamMirror flush emulation
    MsgStreamMirror& flush()    {
      if ( isActive() ) m_stream.flush();
      return *this;
    }

    /// Operators overload:
    /// @{
    /// Accept MsgStreamMirror modifiers (this will call endreq/endmsg)
    MsgStreamMirror& operator<<(MsgStreamMirror& (*_f)(MsgStreamMirror&)) {
      if ( isActive() ) _f(*this);
      return *this;
    }
    /// Accept oMsgStreamMirror modifiers
    MsgStreamMirror& operator<<(std::ostream& (*_f)(std::ostream&)) {
      if ( isActive() ) _f(m_stream);
      return *this;
    }

    /// Accept ios modifiers
    MsgStreamMirror& operator<<(std::ios& (*_f)(std::ios&)) {
      if ( isActive() ) _f(m_stream);
      return *this;
    }

    /// Accept MsgStreamMirror activation using MsgStreamMirrorer operator
    MsgStreamMirror& operator<< (MSG::Level level) {
      return report(level);
    }

    /// Accept MsgStreamMirror activation using MsgStreamMirrorer operator
    MsgStreamMirror& operator<<(long long arg) {
      try {
        // this may throw, and we cannot afford it if the stream is used in a catch block
        m_stream << arg;
      } catch (...) {}
      return *this;
    }
    /// @}

    /// Initialize report of new message: activate if print level is sufficient.
    MsgStreamMirror& report(MSG::Level lvl) {
      if ( ( m_currentLevel = MSG::Level(lvl) ) >= level() ) {
        activate();
      } else {
        deactivate();
      }
      return *this;
    }

    /// Activate MsgStreamMirror
    void activate() {
      m_active = true;
    }
    /// Deactivate MsgStreamMirror
    void deactivate() {
      m_active = false;
    }

    /// Accessor: is MsgStreamMirror active
    bool isActive() const {
      return m_active;
    }

    /// Check if logger is active at message level
    bool msgLevel(const MSG::Level lvl) const {
      return this->level() <= lvl;
    }

    /// IOS emulation
    /// @{
    long flags() const {
      return isActive() ? m_stream.flags()    : 0;
    }
    long flags(std::ios_base::fmtflags v) {
      return isActive() ? m_stream.flags(v)  :  0;
    }
    long setf(std::ios_base::fmtflags v) {
      return isActive() ? m_stream.setf(v)  :  0;
    }
    int width() const {
      return isActive() ? m_stream.width()    : 0;
    }
    int width(int v) {
      return isActive() ? m_stream.width(v)    : 0;
    }
    char fill() const {
      return isActive() ? m_stream.fill()     : (char)-1;
    }
    char fill(char v) {
      return isActive() ? m_stream.fill(v)     : (char)-1;
    }
    int precision() const  {
      return isActive() ? m_stream.precision(): 0;
    }
    int precision(int v) {
      return isActive() ? m_stream.precision(v): 0;
    }
    int rdstate() const  {
      return isActive() ? m_stream.rdstate () : std::ios_base::failbit;
    }
    int good() const  {
      return isActive() ? m_stream.good ()    : 0;
    }
    int eof() const  {
      return isActive() ? m_stream.eof ()     : 0;
    }
    int bad() const  {
      return isActive() ? m_stream.bad()      : 0;
    }
    long setf(std::ios_base::fmtflags _f, std::ios_base::fmtflags _m) {
      return isActive() ? m_stream.setf(_f, _m)   : 0;
    }
    void unsetf(std::ios_base::fmtflags _l)    {
      if ( isActive() ) m_stream.unsetf(_l);
    }
    void clear(std::ios_base::iostate _i = std::ios_base::failbit)  {
      if ( isActive() ) m_stream.clear(_i);
    }
    /// @}
    /// @}

    /// Print information about the log
    void print( const MSG::Level lvl );

    /// Output method
    MsgStreamMirror& doOutput();

    /// Access string MsgStreamMirror
    std::ostringstream& stream(){
      return m_stream;
    }

  private:

    /// Stream name used for identifying the origin from the message display
    std::string m_streamName;
    /// The message level where the stream should accept the input
    MSG::Level m_level;
    /// Message level for the current input string
    MSG::Level m_currentLevel;
    /// Whether stream uses color
    bool m_useColor;

    /// The auxiliary caching string:
    std::ostringstream m_stream;

    /// Whether we are active
    bool m_active;
};

/// MsgStreamMirror Modifier: endmsg. Calls the output method of the MsgStreamMirror
inline
MsgStreamMirror& endmsg(MsgStreamMirror& s) 
{
  return s.doOutput();
}
#define endreq endmsg


#if defined (__GNUC__) && ! defined(__clang__)
inline MsgStreamMirror& operator << (MsgStreamMirror& s,
                               const std::_Setiosflags &manip) {
  try {
    // this may throw, and we cannot afford it if the stream is used in a catch block
    if ( s.isActive() ) s.stream() << manip;
  } catch(...) {}
  return s;
}
inline MsgStreamMirror& operator << (MsgStreamMirror& s,
                               const std::_Resetiosflags &manip)      {
  try {
    // this may throw, and we cannot afford it if the stream is used in a catch block
    if ( s.isActive() ) s.stream() << manip;
  } catch (...) {}
  return s;
}
inline MsgStreamMirror& operator << (MsgStreamMirror& s,
                               const std::_Setbase &manip)    {
  try {
    // this may throw, and we cannot afford it if the stream is used in a catch block
    if ( s.isActive() ) s.stream() << manip;
  } catch (...) {}
  return s;
}
inline MsgStreamMirror& operator << (MsgStreamMirror& s,
                               const std::_Setprecision &manip)       {
  try {
    // this may throw, and we cannot afford it if the stream is used in a catch block
    if ( s.isActive() ) s.stream() << manip;
  } catch (...) {}
  return s;
}
inline MsgStreamMirror& operator << (MsgStreamMirror& s,
                               const std::_Setw &manip)       {
  try {
    // this may throw, and we cannot afford it if the stream is used in a catch block
    if ( s.isActive() ) s.stream() << manip;
  } catch (...) {}
  return s;
}

namespace MSG {
  inline
  MsgStreamMirror& dec(MsgStreamMirror& log) {
    log.setf(std::ios_base::dec, std::ios_base::basefield);
    return log;
  }
  inline
  MsgStreamMirror& hex(MsgStreamMirror& log) {
    log.setf(std::ios_base::hex, std::ios_base::basefield);
    return log;
  }
}
#endif    // WIN32 or (__GNUC__)

/// Specialization to avoid the generation of implementations for char[].
/// \see {<a href="https://savannah.cern.ch/bugs/?87340">bug #87340</a>}
inline MsgStreamMirror& operator<< (MsgStreamMirror& s, const char *arg){
  try {
    // this may throw, and we cannot afford it if the stream is used in a catch block
    if ( s.isActive() ) s.stream() << arg;
  } catch (...) {}
  return s;
}

/// General templated stream operator
template <typename T>
MsgStreamMirror& operator<< (MsgStreamMirror& lhs, const T& arg)  {
  if(lhs.isActive())
    try {
      // this may throw, and we cannot afford it if the stream is used in a catch block
      lhs.stream() << arg;
    }
    catch (...) {}
  return lhs;
}

#if defined(__GNUC__) && ! defined(__clang__)
/// compiler is stupid. Must specialize
template<typename T>
MsgStreamMirror& operator << (MsgStreamMirror& lhs, const std::_Setfill<T> &manip) {
  if ( lhs.isActive() )
    try {
      // this may throw, and we cannot afford it if the stream is used in a catch block
      lhs.stream() << manip;
    } catch(...) {}
  return lhs;
}
#endif

//==============================================================================
inline
std::ostream &operator<<( 
    std::ostream &stream, 
    MsgStreamMirror::Message &msg )
{
  return stream << msg.m_formatted_msg;
}

/**
 * @brief Adds messaging capability to interface
 **/
class IMsgService 
{

  protected:

    /// Stores the default stream name for this interface
    std::string m_defName;

    /// Default level for this interface
    MSG::Level m_defLevel;

    /// Default useColor for this interface
    bool m_defUseColor;

  public:
    /**
     * @brief Builds default interface
     **/
    IMsgService()
      : m_defName("MsgStreamMirror"),
        m_defLevel( MSG::INFO ),
        m_defUseColor( false ){;}

    /**
     * @brief Defines default log name and default level for Messaging Service
     **/
    IMsgService( const std::string& defName, 
                 const MSG::Level defLevel = MSG::INFO ,
                 const bool defUseColor = true )
      : m_defName(defName),
        m_defLevel(defLevel),
        m_defUseColor( defUseColor ){;}

    virtual ~IMsgService(){;}

    /// Check if stream will display at message level
    bool msgLevel(const MSG::Level lvl) const
    {
      return msg().msgLevel(lvl);
    }

    /// Change stream message level display
    void setMsgLevel(const MSG::Level lvl)
    {
      msg().setLevel(lvl);
    }


    void setMsgLevel( const int lvl )
    {
      setMsgLevel( (MSG::Level)lvl );
    }


    /// Get Level of output from MsgStreamMirror Manager
    MSG::Level getMsgLevel() const 
    {
      return msg().level();
    }

    /// Change whether stream use colored messages
    void setUseColor(const bool useColor)
    {
      msg().setUseColor(useColor);
    }

    /// Get whether stream use colored messages
    bool getUseColor() const 
    {
      return msg().useColor();
    }

    /// Change stream display name
    void setLogName(const std::string &name)
    {
      msg().setLogName(name);
    }

    /// Get stream name
    const std::string& getLogName() const 
    {
      return msg().logName();
    }

    /// Get streamer
    virtual MsgStreamMirror& msg() = 0;
    /// cv get streamer
    virtual MsgStreamMirror& msg() const = 0;
};


/**
 * @brief Consolidate message service capabilitiy
 **/
class MsgService : virtual public IMsgService
{
  public:

    /**
     * @brief Builds Message service using default configuration set by
     *        interface
     **/
    MsgService() :
      m_log( m_defName,  m_defLevel, m_defUseColor ){;}

    explicit MsgService(const int lvl)
      : m_log( m_defName, lvl, m_defUseColor ){;}

    /**
     * @brief Builds Message service changing level
     **/
    explicit MsgService(const MSG::Level lvl)
      : m_log( m_defName, lvl, m_defUseColor ){;}

    explicit MsgService(const int lvl, const bool useColor)
      : m_log( m_defName, lvl, useColor ){;}

    explicit MsgService(const MSG::Level lvl, const bool useColor)
      : m_log( m_defName, lvl, useColor ){;}

    /// Retrieve log
    MsgStreamMirror& msg() override final {
      return m_log;
    }

    /// Const Retrieve log
    MsgStreamMirror& msg() const override final {
      return m_log;
    }

  private:
    /// Message Stream:
    mutable MsgStreamMirror m_log;
};

#endif
