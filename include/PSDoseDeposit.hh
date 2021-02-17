
#ifndef PSDoseDeposit_h
#define PSDoseDeposit_h 1


#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"


class PSDoseDeposit : public G4VPrimitiveScorer
{
 public: // with description
      PSDoseDeposit(G4int Nvox, G4double SphereRad, G4String name, G4int depth=0);
      virtual ~PSDoseDeposit();

  protected: // with description
      virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* ROhist);


  public: 
      virtual void Initialize(G4HCofThisEvent*);
      virtual void EndOfEvent(G4HCofThisEvent*);
      virtual void clear();
      virtual void DrawAll();
      virtual void PrintAll();
     
	
  private:
      G4int HCID;
      G4THitsMap<G4double>* EvtMap;
      G4int Nvoxels;
      G4double SphereRadius;
      G4double PI;
   
};
#endif
