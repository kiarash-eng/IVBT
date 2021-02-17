#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"


DetectorMessenger::DetectorMessenger( DetectorConstruction* Det): detector(Det) {
  detectorDir = new G4UIdirectory("/world/");
  detectorDir -> SetGuidance(" world control.");

  sourceCmd = new G4UIcmdWithAString("/source-model/switch", this);
  sourceCmd -> SetGuidance("Choose among the selected source geometries.");
  sourceCmd -> SetParameterName("choice", true);
  sourceCmd -> SetDefaultValue(" ");
  sourceCmd -> SetCandidates("NovosteBetaCath40mmguidewire NovosteBetaCath40mm GuidantGalileoGuidewire GuidantGalileo sryexperimental");
  sourceCmd -> AvailableForStates(G4State_PreInit, G4State_Idle);

  coreCmd = new G4UIcmdWithAString("/source/phantom", this);
  coreCmd -> SetGuidance("Choose among the following available Phantoms.");
  coreCmd -> SetParameterName("choice", true);
  coreCmd -> SetDefaultValue(" ");
  coreCmd -> SetCandidates("Artery_Stent Water");
  coreCmd -> AvailableForStates(G4State_PreInit, G4State_Idle);

  stentnumberCmd = new G4UIcmdWithAString("/detector-stent/stent-number", this);
  stentnumberCmd -> SetGuidance("Choose among the selected number of stents.");
  stentnumberCmd -> SetParameterName("choice", true);
  stentnumberCmd -> SetDefaultValue(" ");
  stentnumberCmd -> SetCandidates("One Three");
  stentnumberCmd -> AvailableForStates(G4State_PreInit, G4State_Idle);

  stentpositionCmd = new G4UIcmdWithAString("/detector-stent/stent-position", this);
  stentpositionCmd -> SetGuidance("Choose among the selected position of stents.");
  stentpositionCmd -> SetParameterName("choice", true);
  stentpositionCmd -> SetDefaultValue(" ");
  stentpositionCmd -> SetCandidates("Overlapped NotOverlapped");
  stentpositionCmd -> AvailableForStates(G4State_PreInit, G4State_Idle);
}

DetectorMessenger::~DetectorMessenger()
{
  delete sourceCmd;
  delete stentnumberCmd;
  delete coreCmd;
  delete detectorDir;
  delete stentpositionCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  // Messenger function to set the source type
  if (command == sourceCmd) {
       detector->SelectBrachySource(newValue);
  }
  if (command == coreCmd) {
       detector->SelectBrachyCore(newValue);
  }
  if (command == stentnumberCmd) {
   detector->SelectStentNumber(newValue);
 }
  if (command == stentpositionCmd) {
    detector->SelectStentOverlapping(newValue);
  }
}
