#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;


class DetectorMessenger: public G4UImessenger
{
public:
  DetectorMessenger(DetectorConstruction* );
  ~DetectorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

private:

  DetectorConstruction*  detector;
  G4UIdirectory*         detectorDir;
  G4UIcmdWithAString*    sourceCmd;
  G4UIcmdWithAString*    coreCmd;
  G4UIcmdWithAString*    lumenCmd;
  G4UIcmdWithAString*    stentmodelCmd;
  G4UIcmdWithAString*    stentnumberCmd;
  G4UIcmdWithAString*    stentpositionCmd;
  G4UIcmdWithAString*    neointimaCmd;
  G4UIcmdWithAString*    calcifiedCmd;

};
#endif
