#include <fstream>
#include <sstream>
#include <ctime>
#include <math.h>
#include <string>

#include "TrackLengthEstimator.hh"
#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4Run.hh"
#include "G4ProcessManager.hh"
#include "G4ProductionCutsTable.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "Run.hh"
#include "G4EmCalculator.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "ParallelWorldConstructionScoring.hh"


using namespace std;

G4int RunAction::numEvents=0;


RunAction::RunAction(ParallelWorldConstructionScoring* scoringWorld, G4String Name, long int Tid)
: fScoringWorld(scoringWorld)
{
    //Nvoxels=NumberVoxels;
    RunName=Name;
    fScoringWorld = scoringWorld;
    //fNGammasCreated = 0;
    fPrimaryGenerator = 0;
    tid = Tid;
}

RunAction::RunAction(ParallelWorldConstructionScoring* scoringWorld, G4String Name, long int Tid, PrimaryGeneratorAction* thePrimGen)
{
    fScoringWorld = scoringWorld;
    RunName=Name;
    tid = Tid;
    fPrimaryGenerator = thePrimGen;
}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run* aRun) {
  numEvents=aRun->GetNumberOfEventToBeProcessed();
  time_t mytime;
  time (&mytime);

  if (numEvents >= 100){
    G4cout << setw(9) << "     " << setw(8) << "Event No" << setw(22) << "Start Time" << G4endl;
    G4cout << " ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ " << G4endl;
  }
}

G4Run* RunAction::GenerateRun() {
  std::vector<G4String>theSDNames;
  //theSDNames.push_back("det3");
  //theSDNames.push_back("det2");
  theSDNames.push_back("det1");
  //theSDNames.push_back("radialdet");
  return new Run(theSDNames);
}

void RunAction::EndOfRunAction(const G4Run* aRun)
 {
  // Print number of events processed
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  G4cout << " Number of Events Processed:" << NbOfEvents << " events. " <<G4endl;

  //Print out results
  Run *theRun=(Run*)aRun;
  G4int nGammasCreated = theRun->fNGammasCreated;
  //If I cared I could send the number of voxels to the DumpAllScorer(), so it would print even for empty voxels
  //theRun->DumpAllScorer();

  // Retreiving Accumulated data and output results.
  // Dump result for all HitsMap collections.
  vector<G4String> colLabels = fScoringWorld->GetColumnLabels();
  vector<G4String> rowLabels = fScoringWorld->GetRowLabels();

  G4int NhitsMap = theRun->GetNumberOfHitsMap();

  for (G4int im=0; im<NhitsMap; im+=2)
  {

    G4THitsMap<G4double>*RunMap1 = theRun->GetHitsMap(im);
    G4THitsMap<G4double>*RunMap2 = theRun->GetHitsMap(im+1);

    if (RunMap1 && RunMap2)
    {
      G4int Ncol = fScoringWorld->GetNumVoxelsCol();

      G4String tids = static_cast<ostringstream*>( &(ostringstream() << tid) )->str();
      G4String filename = "Data/"+tids+"."+RunName+"_"+RunMap1->GetName()+".csv";
      std::ofstream out(filename.data());
      out<<"Number of events processed," << NbOfEvents <<G4endl;
      out<<"Number of electrons created," << nGammasCreated <<G4endl;
      out<<"Air kerma per particle (Gy), Error"<<G4endl;
      out<<"Voxel #, Dose per Particle, Statistical Uncertainty"<<G4endl;

      G4cout<<"Number of elements of RunMap1: "<<RunMap1->GetSize()<<G4endl;
      G4cout<<"Number of elements of RunMap2: "<<RunMap2->GetSize()<<G4endl;
      /*
      for(G4int i=0;i<Nvoxels;i++)
      {
        G4double* dose = (*RunMap1)[i];
        G4double* error = (*RunMap2)[i];
        if (dose || error)
        {
          //Trying to get the units right (so right now this is outputting in internal system units)
          //Which is A-OK, because for the TG-43 dose formalism we just need to be consistent
          //Actual dose units are less relevant
          *dose = *dose / (G4double)NbOfEvents;
          *error = *error / (G4double)NbOfEvents;
          *error = *error - (*dose)*(*dose);
          *error = *error / ((G4double)NbOfEvents-1.0);
          *error = sqrt(*error);
          out << (i+1) << "," << *dose << "," << *error << "," << G4endl;
        }
        else
        {
          out << (i+1) << ", 0, 0" << G4endl;
        }

      }*/


      for(G4String lab: colLabels)
      {
        out<<lab<<",";
      }
      for(G4int i=0;i<fScoringWorld->GetNumVoxels();i++)
      {
        G4double* dose = (*RunMap1)[i];
        G4double* error = (*RunMap2)[i];
        if (dose && error)
        {
          //G4cout << i << G4endl;
          G4double nDose = *dose / (G4double) NbOfEvents;
          if (i%Ncol == Ncol-1) {
            out << nDose/gray; //don't output comma at EOL
          }
          else if (i%Ncol > 0)
          {

            out <<  nDose/gray << ",";
          }
          else
          {
            out << G4endl;
            //out << (floor(i/ScoringWorld->GetNumVoxelsCol())+1)*dr << ",";
            out << rowLabels[floor(i/Ncol)] << ",";
            out << nDose/gray << ",";

          }
        }
        else
        {
          //G4cout << "i=" << i << " dose=" << 0 << G4endl;
          if (i%Ncol == Ncol-1) {
            out << 0;
          }
          else if ((i%Ncol) > 0)
          {
            out << 0 << ",";
          }
          else
          {
            out << G4endl;
            //out << (floor(i/ScoringWorld->GetNumVoxelsCol())+1)*dr << ",";
            out << rowLabels[floor(i/Ncol)] << ",";
            out << 0 << ",";


          }
        }
      }
      out.close();
      out.clear();



      /// try to now write the errors instead
      filename = RunName+"_error.csv";
      out.open(filename.data());
      out<<"Number of events processed," << NbOfEvents <<G4endl;
      out<<"Number of gammas created," << nGammasCreated <<G4endl;
      out<<"Relative Uncertainty" << G4endl;
      //out << "r (cm) | theta (deg),";

      for(G4String lab: colLabels)
      {
        out<<lab<<",";
      }

      for(G4int i=0;i<fScoringWorld->GetNumVoxels();i++)
      {
        G4double* dose = (*RunMap1)[i];
        G4double* error = (*RunMap2)[i];

        if (dose && error)
        {
          G4double dos = *dose;
          G4double err = *error;
          G4double N = (G4double) NbOfEvents;

          dos=dos/N;
          err = err/N;
          err = err - dos*dos;
          err = err / (N-1);
          err = sqrt(err);
          err = err/dos;
          // err = err / N;

          if (i%Ncol == Ncol-1) {
            out << err;
          }
          else if ((i%Ncol) > 0)
          {
            out << err << ",";
          }
          else
          {
            out << G4endl;
            //out << (floor(i/ScoringWorld->GetNumVoxelsCol())+1)*dr << ",";
            out << rowLabels[floor(i/Ncol)] << ",";
            out << err << ",";

          }
        }
        else
        {
          if (i%Ncol == Ncol-1) {
            out << 0;
          }
          else if ((i%Ncol) > 0)
          {
            out << 0 << ",";
          }
          else
          {
            out << G4endl;
            //out << (floor(i/ScoringWorld->GetNumVoxelsCol())+1)*dr << ",";
            out << rowLabels[floor(i/Ncol)] << ",";
            out << 0 << ",";


          }
        }
      }
     out.close();
     out.clear();
    }
  }

}
