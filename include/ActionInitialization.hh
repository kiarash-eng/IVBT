#ifndef ActionInitialization_H
#define ActionInitialization_H 1
#include "G4VUserActionInitialization.hh"
#include "ParallelWorldConstructionScoring.hh"
#include "DetectorConstruction.hh"
class ParallelWorldConstructionScoring;

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization(G4String, long int,  ParallelWorldConstructionScoring *, DetectorConstruction *);
    virtual ~ActionInitialization();

    //virtual void BuildForMaster() const;
    virtual void Build() const;

  private:
    G4String fileName;
    long int fTid;
    ParallelWorldConstructionScoring *fScoringWorld;
    DetectorConstruction *fDetector;
};

#endif
