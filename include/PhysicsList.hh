//
//    **********************************
//    *                                *
//    *   PhysicsList.hh         *
//    *                                *
//    **********************************
/*

*/
#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "G4EmConfigurator.hh"
#include "globals.hh"

// Modular physics used in this application
class PhysicsList: public G4VModularPhysicsList
{
public:
  PhysicsList();
  ~PhysicsList();
  void ConstructParticle();
 // void AddPhysicsList(const G4String& name);
  void ConstructProcess();
  void AddParallelWorldProcess();
protected:
  void SetCuts();

private:
  G4VPhysicsConstructor* emPhysicsList;
  G4VPhysicsConstructor* decPhysicsList;
  G4VPhysicsConstructor* radDecayPhysicsList;
};
#endif
