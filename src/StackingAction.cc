using namespace std;

#include "StackingAction.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ios.hh"
#include "G4VTouchable.hh"
#include "G4RadioactiveDecay.hh"

StackingAction::StackingAction(RunAction* theRunAction) {
  runAction = theRunAction;
}
/*
StackingAction*  StackingAction::pinst=NULL;

StackingAction*  StackingAction::GetInstance() {
  return pinst;
}*/

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track * aTrack) {
  G4ClassificationOfNewTrack classification = fUrgent;


   // G4String particleName = aTrack->GetDefinition()->GetParticleName();
   // if (particleName != "e-") {
   //   classification = fKill;
   // }

   // the following line means if the pointer is null, the track is for a
   // primary particle so it has not gone under radioactive decay, yet.

   if (aTrack->GetCreatorProcess() != NULL)
   // you get error if you enter G4cout in this space instead of inside the
   // bracket.
   {
     // G4cout <<aTrack->GetCreatorProcess()->GetProcessName() << G4endl;
     if (aTrack->GetCreatorProcess()->GetProcessName() == "RadioactiveDecayBase")
     //if (aTrack->GetCreatorProcess()->GetProcessName() != "eIoni" )
     // if (aTrack->GetCreatorProcess()->GetProcessName() == "eBrem" )

     {
       //G4cout << "Radioactive Decay Particle Created" << G4endl;
       G4String particleName = aTrack->GetDefinition()->GetParticleName();
        if (particleName == "anti_nu_e" || particleName == "nu_e" || particleName == "alpha+")
        {
          classification = fKill;
        }
        else if(particleName == "e-")
        {
          runAction->fNGammasCreated++;
          //G4cout << "Primary electron created #: " << runAction->fNGammasCreated << G4endl;
        }
     }
   }/*
   if(!aTrack->GetUserInformation()) {
           aTrack->SetUserInformation(info);
         }
    */
  return classification;
}
