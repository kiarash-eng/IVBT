#ifndef PositionList_h
#define PositionList_h 1

#include <vector>
#include "globals.hh"
#include "G4ThreeVector.hh"

class PositionList {
  public:
    PositionList() { }
    ~PositionList() { }

    G4bool ReadDataFile(G4String);
    G4int GetNumberOfSources() {return totalNumberOfSources;}
    G4ThreeVector GetPosition(G4int SourceNumber);
    std::vector<G4ThreeVector> GetListOfPositions();

  private:
    G4int totalNumberOfSources;
    std::vector<G4ThreeVector> listOfPositions;
};

#endif
