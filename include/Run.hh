

#ifndef Run_h
#define Run_h 1

#include "G4Run.hh"
#include "G4Event.hh"

#include "G4THitsMap.hh"
#include <vector>
//
class Run : public G4Run {

public:
  // constructor and destructor.
  //  vector of multifunctionaldetector name has to given to constructor.
  Run(const std::vector<G4String> sdName);
  virtual ~Run();

public:
  // virtual method from G4Run.
  // The method is overriden in this class for scoring.
  virtual void RecordEvent(const G4Event*);

  // Access methods for scoring information.
  // - Number of HitsMap for this RUN.
  //   This is equal to number of collections.
  G4int GetNumberOfHitsMap() const {return theRunMap.size();}
  // - Get HitsMap of this RUN.
  //   by sequential number, by multifucntional name and collection name,
  //   and by collection name with full path.
  G4THitsMap<G4double>* GetHitsMap(G4int i){return theRunMap[i];}
  G4THitsMap<G4double>* GetHitsMap(const G4String& detName,const G4String& colName);
  G4THitsMap<G4double>* GetHitsMap(const G4String& fullName);
  // - Dump All HitsMap of this RUN.
  //   This method calls G4THisMap::PrintAll() for individual HitsMap.
  void DumpAllScorer();
  G4int fNGammasCreated; 

private:
  std::vector<G4String>              theCollName;  // Collection names
  std::vector<G4int>                 theCollID;    // Collection IDs
  std::vector<G4THitsMap<G4double>*> theRunMap;    // HitsMaps
};

//

#endif
