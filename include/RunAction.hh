
#ifndef RunAction_h
#define RunAction_h 1

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "ParallelWorldConstructionScoring.hh"
#include "G4UserRunAction.hh"
#include "globals.hh"
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;
//class Run;
class EventAction;
class RunAction : public G4UserRunAction
{
  public:
    //RunAction(G4int NumVoxels, G4String Name,long int Tid);
    RunAction(ParallelWorldConstructionScoring* scoringWorld, G4String Name,long int Tid);
    RunAction(ParallelWorldConstructionScoring* scoringWorld, G4String Name,long int Tid, PrimaryGeneratorAction* thePrimGen);
   ~RunAction();

  public:
    virtual G4Run* GenerateRun();
    void    BeginOfRunAction(const G4Run* aRun);
    void    EndOfRunAction(const G4Run*);
    static G4int GetNumEvents() {return numEvents;}
    long int fNGammasCreated;
    long int tid;
  private:
    static G4int numEvents;
  	G4int Nvoxels;
    G4String RunName;
    ParallelWorldConstructionScoring* fScoringWorld;
    PrimaryGeneratorAction* fPrimaryGenerator;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
