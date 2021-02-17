

#ifndef PSDoseAirDeposit_h
#define PSDoseAirDeposit_h 1


#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

class PSDoseAirDeposit : public G4VPrimitiveScorer
{
 public: // with description
      PSDoseAirDeposit(G4int Nvox, G4double SphereRad, G4String name, G4int depth=0);
      virtual ~PSDoseAirDeposit();

  protected: // with description
      virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* ROhist);


  public:
      virtual void Initialize(G4HCofThisEvent*);
      virtual void EndOfEvent(G4HCofThisEvent*);
      virtual void clear();
      virtual void DrawAll();
      virtual void PrintAll();


  private:

      G4int FindBinLocation(G4double energy);

      G4double linLogLogInterpolate(G4double x, G4int bin,
                                std::vector<G4double>& points,
                                std::vector<G4double>& data);
      G4int HCID;
      G4THitsMap<G4double>* EvtMap;
      G4int Nvoxels;
      G4double SphereRadius;
      G4double PI;
      G4int numBins;
      std::vector<G4double> muenEnergy;
      std::vector<G4double> muenValue;

};
#endif
