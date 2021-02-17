
#ifndef SourceModel_h
#define SourceModel_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4VPhysicalVolume;

class SourceModel {
public:
  SourceModel();
  virtual ~SourceModel();

  // this function manages the creation of the source in terms of
  // geometry ...
  virtual void CreateSource(G4VPhysicalVolume*) {};

  // ...
  // This function deletes the source in the geometry (phantom); to be
  // used when the user needs to substitute one source with another one ...
  virtual void CleanSource() {};
};
#endif
