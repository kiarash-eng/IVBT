#include "TrackLengthMessenger.hh"
#include "TrackLengthEstimator.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"


TrackLengthMessenger::TrackLengthMessenger(TrackLengthEstimator* tr) :G4UImessenger(), track(tr) {
  TrackDir = new G4UIdirectory("/trackLenghtE/");
  TrackDir->SetGuidance("TrackLengthEstimator control.");

  fmuenCmd = new G4UIcmdWithAString("/trackLengthE/fileName", this);
  fmuenCmd->SetGuidance("select mass energy absorption file.");
  fmuenCmd->SetParameterName("choice", true);
  fmuenCmd->SetDefaultValue(" ");
  fmuenCmd->SetCandidates("water.dat air.dat");
  fmuenCmd->AvailableForStates(G4State_PreInit,G4State_Idle);


}

TrackLengthMessenger::~TrackLengthMessenger() {
  delete TrackDir;
  delete fmuenCmd;
}

void TrackLengthMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
  if(command == fmuenCmd) {
  }
}

