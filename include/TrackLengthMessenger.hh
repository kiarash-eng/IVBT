#ifndef TrackLengthMessenger_h
#define TrackLengthMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "TrackLengthEstimator.hh"

class TrackLengthEstimator;
class G4UIdirectory;
class G4UIcmdWithAString;

class TrackLengthMessenger: public G4UImessenger {
  public:
    explicit TrackLengthMessenger(TrackLengthEstimator*);
    virtual ~TrackLengthMessenger();

   virtual void SetNewValue(G4UIcommand*, G4String);

  private:

  TrackLengthEstimator *track;
  G4UIdirectory *TrackDir;
  G4UIcmdWithAString *fmuenCmd;

};
#endif
