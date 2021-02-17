#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "StackingAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
//#include "TrackingAction.hh"

ActionInitialization::ActionInitialization(G4String fileN, long int tid, ParallelWorldConstructionScoring *scoringWorld ,DetectorConstruction * detector)
    : G4VUserActionInitialization(), fileName(fileN), fTid(tid), fScoringWorld(scoringWorld), fDetector(detector)
{
}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::Build() const
{
    EventAction* theEventAction = new EventAction();
    PrimaryGeneratorAction *thePrimGenAction = new PrimaryGeneratorAction();
    RunAction *theRunAction = new RunAction(fScoringWorld, fileName, fTid, thePrimGenAction);
    StackingAction *theStackingAction = new StackingAction(theRunAction); 

    SetUserAction(thePrimGenAction);
    SetUserAction(theRunAction);
    SetUserAction(theEventAction);
    SetUserAction(theStackingAction);
    //SetUserAction(new TrackingAction());
}
/*
void ActionInitialization::BuildForMaster() const {
    RunAction* theRunAction = new RunAction(fScoringWorld, fileName, fTid);
    SetUserAction(theRunAction);
}*/
