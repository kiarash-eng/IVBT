
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIsession.hh"
#include "G4UIterminal.hh"
#include "G4UIcsh.hh"
#include "G4UIExecutive.hh"

#include "G4EmCalculator.hh"
#include "G4Types.hh"
#include "DetectorConstruction.hh"
#include "ParallelWorldConstructionScoring.hh"
#include "RadialParallelWorldConstructionScoring.hh"
#include "PhysicsList.hh"
#include "G4VModularPhysicsList.hh"
#include "globals.hh"
#include "G4ParallelWorldPhysics.hh"
#include "PrimaryGeneratorAction.hh"
#include "StackingAction.hh"
#include "G4VisExecutive.hh"
#include "RunAction.hh"
#include "ActionInitialization.hh"
#include "EventAction.hh"
#include "Randomize.hh"
#include <iostream>

//using namespace std;

int main(int argc, char** argv)
{
    G4String fileName;
      long int Tid = 0;
      if (argc != 1) {
          fileName = argv[1];
          //string timestring = argv[2];
          //G4cout << " timestring: " << timestring << G4endl;
          char * pEnd;

          Tid = strtol(argv[2], &pEnd, 10);

          G4cout << " Tid:  " << Tid << G4endl;


      }

    CLHEP::RanecuEngine *theRanGenerator = new CLHEP::RanecuEngine;
    theRanGenerator->setSeed(Tid);
    CLHEP::HepRandom::setTheEngine(theRanGenerator);
    //G4cout <<Tid << G4endl;

    G4RunManager* runManager = new G4RunManager;

    //G4int radiusincrements = 28;
    //G4int rhoincrements = 40;
    //G4int nVoxels = radiusincrements*rhoincrements;

    //DetectorConstruction* detector = new DetectorConstruction(nVoxels);
    DetectorConstruction* detector = new DetectorConstruction();
    //G4String scoringWorldName = "ScoringWorld";
    G4String scoringWorldName = G4String("ScoringWorld");
    //string scoringWorldName = G4String("ScoringWorld");
    //G4cout<<scoringWorldName<<G4endl;
    ParallelWorldConstructionScoring* scoringWorld = new ParallelWorldConstructionScoring(scoringWorldName);
    //ParallelWorldConstructionScoring* scoringWorld = new ParallelWorldConstructionScoring(scoringWorldName, detector,1);
    //
    // I commented out the creation of scoring world 2 in order to suppress the physics errors we were getting
    //
    //G4String scoringWorld2Name = "ScoringWorld2";
    //ParallelWorldConstructionScoring* scoringWorld2 = new ParallelWorldConstructionScoring(scoringWorld2Name, detector,2);
    //RadialParallelWorldConstructionScoring* scoringWorld2 = new RadialParallelWorldConstructionScoring(scoringWorld2Name, detector,1);
    //G4String scoringWorld3Name = "Scoring3World";
    //ParallelWorldConstructionScoring* scoringWorld3 = new ParallelWorldConstructionScoring(scoringWorld3Name, detector,3);


    detector->RegisterParallelWorld(scoringWorld);
    //detector->RegisterParallelWorld(scoringWorld2);
    //detector->RegisterParallelWorld(scoringWorld3);
    //detector->RegisterParallelWorld(radialscoringworld);

    runManager->SetUserInitialization(detector);

    PhysicsList* physList = new PhysicsList();
    runManager->SetUserInitialization(physList);
    runManager->SetUserInitialization(new ActionInitialization(fileName,Tid, scoringWorld,detector));
    //runManager->SetUserInitialization(new ActionInitialization(scoringWorld, fileName, Tid));

    //runManager->SetUserInitialization(new ActionInitialization(fileName,Tid, scoringWorld,detector));
/*
    PrimaryGeneratorAction *thePrimaryGeneratorAction = new PrimaryGeneratorAction();
    //RunAction *theRunAction = new RunAction(fScoringWorld, fileName, Tid);
    RunAction *theRunAction = new RunAction(scoringWorld, fileName, Tid);
    //RunAction *theRunAction = new RunAction(nVoxels, fileName, Tid);
    runManager->SetUserAction(thePrimaryGeneratorAction);

    //runManager->SetUserAction(theRunAction);
    runManager->SetUserAction(new StackingAction(theRunAction));
    EventAction* pEventAction = new EventAction();
    runManager->SetUserAction(pEventAction);

*/
G4UImanager* UI = G4UImanager::GetUIpointer();

#ifdef G4VIS_USE
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
#endif

    if (argc != 1)
    {
        G4String command = "/control/execute ";
        G4String file_name = argv[1];
        UI->ApplyCommand(command+file_name);
    }
    else
    {
#ifdef G4UI_USE
        G4UIExecutive * ui = new G4UIExecutive(argc, argv);
        ui->SessionStart();
        delete ui;
#endif
    }

#ifdef G4VIS_USE
    delete visManager;
#endif

    delete runManager;
    return 0;
}
