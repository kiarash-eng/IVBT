#ifndef TrackLengthEstimator_h
#define TrackLengthEstimator_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

class TrackLengthMessenger;
class TrackLengthEstimator : public G4VPrimitiveScorer
{
 public: // with description
      TrackLengthEstimator(G4String name, G4int depth=0);
      TrackLengthEstimator(G4String name, const G4String& unit, G4int depth=0);
      virtual ~TrackLengthEstimator();
      void loadMus(std::vector<G4String> matNames);
      static G4int FindCounts(G4int n);
      static G4int FindNumEnergyBins();
      void SetNumberCol(G4int);
      void SetNumberRow(G4int);

  protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
      virtual G4int GetIndex(G4Step*);
      virtual G4double ComputeVolume(G4Step*, G4int idx);

  public:
      virtual void Initialize(G4HCofThisEvent*);
      virtual void EndOfEvent(G4HCofThisEvent*);
      virtual void clear();
      virtual void DrawAll();
      virtual void PrintAll();

      virtual void SetUnit(const G4String& unit);

  private:

      G4int FindBinLocation(G4double energy, std::vector<G4double>&);

      G4double linLogLogInterpolate(G4double x, G4int bin,
                                std::vector<G4double>& points,
                                std::vector<G4double>& data);
      G4int numMaterials;
      G4int firstevent;
      std::vector<G4String> matList;
      std::map<G4String, std::vector<G4double> > muEnergies;
      std::map<G4String, std::vector<G4double> > muValues;
      std::map<G4String, std::vector<G4double> > muenValues;

      G4int HCID;
      G4THitsMap<G4double>* EvtMap;
      TrackLengthMessenger* trackLengthMessenger;
      G4int fNcol, fNrow; //ncol = #of cols, nrow = # of rows


};
#endif
