#include "EventAction.hh"
#include "RunAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"

#include <iostream>
#include <ctime>
using namespace std;

EventAction::EventAction() {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* aEvent) {

  G4int numEvents=RunAction::GetNumEvents();
  G4int evtNb = aEvent->GetEventID();

  if (numEvents>99) {
    if ((evtNb%(numEvents/10)) == 0) {
      time_t mytime;
      time (&mytime);
      G4cout << setw(10) << evtNb << setw(32) << ctime(&mytime) << G4endl;
    }
  }
}

void EventAction::EndOfEventAction(const G4Event*) {}
