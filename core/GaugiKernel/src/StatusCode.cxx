
// System include(s):
#include <iostream>
#include <cstdlib>

// Local include(s):
#include "GaugiKernel/StatusCode.h"

namespace {

   /// Class used behind the scenes to keep track of the unchecked status codes
   ///
   /// One object of this type is created, just in case there are unchecked
   /// status codes encountered during the job. This single object is finally
   /// deleted at the end of the job, printing some complaints for the user.
   ///
   class UncheckedCounter {

   public:
      /// Default constructor
      UncheckedCounter() : m_uncheckedSuccess( 0 ), m_uncheckedFailure( 0 ) {}
      /// Destructor
      ~UncheckedCounter() {
         std::cerr << "Warning in <StatusCode>:" << std::endl;
         std::cerr << "Warning in <StatusCode>: "
                   << "Unchecked status codes encountered during the job"
                   << std::endl;
         if( m_uncheckedSuccess ) {
            std::cerr << "Warning in <StatusCode>: "
                      << "Number of unchecked successes: "
                      << m_uncheckedSuccess << std::endl;
         }
         if( m_uncheckedFailure ) {
            std::cerr << "Error in   <StatusCode>: "
                      << "Number of unchecked failures: "
                      << m_uncheckedFailure << std::endl;
         }
         // Let him/her know how to look up the unchecked codes in the easiest
         // way:
         std::cerr << "Warning in <StatusCode>: "
                   << "To fail on an unchecked code, call "
                   << "StatusCode::enableFailure() at the job's start"
                   << std::endl;
         std::cerr << "Warning in <StatusCode>:" << std::endl;
      }

      /// Number of unchecked successful status codes
      int m_uncheckedSuccess;
      /// Number of unchecked failure status codes
      int m_uncheckedFailure;

   }; // class UncheckedCounter

} // private namespace



static bool s_failure = false;

StatusCode::StatusCode( unsigned long rstat )
   : m_code( rstat ), m_checked( false ) {

}

StatusCode::StatusCode( const StatusCode& parent )
   : m_code( parent.m_code ), m_checked( false ) {

   // Mark the parent's code checked:
   parent.m_checked = true;
}

StatusCode::~StatusCode() {

   if( ! m_checked ) {
      // If we are supposed to fail, let's fail right away:
      if( s_failure ) {
         std::cerr << "Fatal in <StatusCode::~StatusCode> "
                   << "Unchecked status code encountered" << std::endl;
         std::abort();
      }
      // Global variable for keeping track of unchecked return codes.
      // It gets deleted only at the end of the process.
      static ::UncheckedCounter s_counter;
      if( m_code == SUCCESS ) {
         s_counter.m_uncheckedSuccess += 1;
      } else if( m_code == FAILURE ) {
         s_counter.m_uncheckedFailure += 1;
      } else {
         std::cerr << "Fatal in <StatusCode::~StatusCode> "
                   << "Unknown status code encountered" << std::endl;
         std::abort();
      }
   }
}

StatusCode& StatusCode::operator= ( const StatusCode& rhs ) {

   // Check if we need to do anything:
   if( this == &rhs ) {
      return *this;
   }

   // Get the code from the other object:
   m_code = rhs.m_code;

   // Set the checked status flags correctly:
   m_checked = false;
   rhs.m_checked = true;

   // Return this object:
   return *this;
}

StatusCode& StatusCode::operator= ( unsigned long code ) {

   // Set the code:
   m_code = code;

   // Set the object unchecked:
   m_checked = false;

   // Return this object:
   return *this;
}

bool StatusCode::isSuccess() const {

   setChecked();
   return ( m_code == SUCCESS );
}

bool StatusCode::isFailure() const {

   setChecked();
   return ( ! isSuccess() );
}

StatusCode::operator unsigned long() const {

   setChecked();
   return m_code;
}

void StatusCode::enableFailure() {

   s_failure = true;
   return;
}

void StatusCode::disableFailure() {

   s_failure = false;
   return;
}
