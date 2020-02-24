#ifndef EventReaderMessage_h
#define EventReaderMessege_h

/** geant 4 libs **/
#include "G4UImessenger.hh"

/** standard libs #**/
#include <string>

class EventReader;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

class EventReaderMessenger : public G4UImessenger {
  
  public:
    EventReaderMessenger(EventReader* agen);
    ~EventReaderMessenger();
  
    void SetNewValue(G4UIcommand* command, std::string newValues);
    std::string GetCurrentValue(G4UIcommand* command);
  
  private:
    EventReader           *m_gen;
    G4UIdirectory         *m_dir;
    G4UIcmdWithAnInteger  *m_verbose;
    G4UIcmdWithAString    *m_open;

};

#endif
