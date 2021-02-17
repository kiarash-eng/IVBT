//
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "EmDNAPhysics.hh"
#include "PhysicsList.hh"
#include "G4EmProcessOptions.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4ParallelWorldProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProductionCutsTable.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ios.hh"
#include "G4StepLimiter.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ParallelWorldPhysics.hh"
PhysicsList::PhysicsList(): G4VModularPhysicsList() {
  SetVerboseLevel(1);

  // EM physics: 3 alternatives

  //emPhysicsList = new G4EmStandardPhysics_option4(1);

  // Alternatively you can substitute this physics list
  // with the LowEnergy Livermore or LowEnergy Penelope:
  //emPhysicsList = new G4EmLivermorePhysics();
  // Low Energy based on Livermore Evaluated Data Libraries
  //
  // Penelope physics
  emPhysicsList = new G4EmPenelopePhysics();

  // Geant4DNA physics
  //emPhysicsList = new EmDNAPhysics();
  // emPhysicsList = new EmDNAPhysics_option1();

  // Add Decay
  decPhysicsList = new G4DecayPhysics();
  radDecayPhysicsList = new G4RadioactiveDecayPhysics();

}

PhysicsList::~PhysicsList() {
  delete decPhysicsList;
  delete radDecayPhysicsList;
  delete emPhysicsList;
}

void PhysicsList::ConstructParticle() {
  decPhysicsList->ConstructParticle();
}

void PhysicsList::ConstructProcess() {
  AddTransportation();
  AddParallelWorldProcess();
  emPhysicsList->ConstructProcess();

   G4EmProcessOptions emOptions;
   emOptions.SetFluo(true); // To activate deexcitation processes and fluorescence
   emOptions.SetAuger(true); // To activate Auger effect if deexcitation is activated
   emOptions.SetPIXE(true); // To activate Particle Induced X-Ray Emission (PIXE)

  // decay physics list
  decPhysicsList->ConstructProcess();
  radDecayPhysicsList->ConstructProcess();

  G4cout << "### PhysicsList::ConstructProcess is done" << G4endl;
}

void PhysicsList::AddParallelWorldProcess() {

  //For explanation of what's going on here refer to Geant4 documentation, Parallel Worlds Application Developer's Guide 4.7
  G4ParallelWorldProcess* ScoringWorldProcess = new G4ParallelWorldProcess("scoringWorldProc");
  ScoringWorldProcess->SetParallelWorld("ScoringWorld");

  G4ParallelWorldProcess* ScoringWorld2Process = new G4ParallelWorldProcess("scoringWorld2Proc");
  ScoringWorld2Process->SetParallelWorld("ScoringWorld2");

  //auto theParticleIterator=GetParticleIterator();

  theParticleIterator->reset();

  while ((*theParticleIterator)())
  {
   G4ParticleDefinition* particle = theParticleIterator->value();
   G4ProcessManager* pmanager = particle->GetProcessManager();
   /*
   pmanager->AddProcess(ScoringWorldProcess);
   pmanager->SetProcessOrderingToLast(ScoringWorldProcess, idxAtRest);
   pmanager->SetProcessOrdering(ScoringWorldProcess, idxAlongStep, 1);
   pmanager->SetProcessOrderingToLast(ScoringWorldProcess, idxPostStep);


   pmanager->AddProcess(ScoringWorld2Process);
   pmanager->SetProcessOrderingToLast(ScoringWorld2Process, idxAtRest);
   pmanager->SetProcessOrdering(ScoringWorld2Process, idxAlongStep, 1);
   pmanager->SetProcessOrderingToLast(ScoringWorld2Process, idxPostStep);
*/


        pmanager->AddProcess(ScoringWorldProcess);
        if(ScoringWorldProcess->IsAtRestRequired(particle)) {
            pmanager->SetProcessOrdering(ScoringWorldProcess, idxAtRest, 9998);
        }
        pmanager->SetProcessOrdering(ScoringWorldProcess, idxAlongStep, 1);
        pmanager->SetProcessOrdering(ScoringWorldProcess, idxPostStep, 9998);

        pmanager->AddProcess(ScoringWorld2Process);
        if(ScoringWorld2Process->IsAtRestRequired(particle)) {
            pmanager->SetProcessOrdering(ScoringWorld2Process, idxAtRest, 9999);
        }
        //This used to be priority 1, now I'm setting in way back as well
        pmanager->SetProcessOrdering(ScoringWorld2Process, idxAlongStep, 1);
        pmanager->SetProcessOrdering(ScoringWorld2Process, idxPostStep, 9999);

    }

}

void PhysicsList::SetCuts() {
  // Definition of  threshold of production
  // of secondary particles
  // This is defined in range.
  //defaultCutValue = 0.001 * mm;
  defaultCutValue = 0.01 * mm;
  //SetCutValue(defaultCutValue, "gamma");
  //SetCutValue(defaultCutValue, "e-");
  //SetCutValue(defaultCutValue, "e+");

  // By default the low energy limit to produce
  // secondary particles is 990 eV.
  // This value is correct when using the EM Standard Physics.
  // When using the Low Energy Livermore this value can be
  // changed to 250 eV corresponding to the limit
  // of validity of the physics models.
  // Comment out following three lines if the
  // Standard electromagnetic Package is adopted.
  //Low originally set to 100eV
  G4double lowLimit = 5000. * eV;
  G4double highLimit = 100. * GeV;

  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(lowLimit,
                                                                  highLimit);

  // Print the cuts
  if (verboseLevel>0) DumpCutValuesTable();
}
