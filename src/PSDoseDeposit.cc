// G4PSDoseDeposit
#include "PSDoseDeposit.hh"
#include "G4VSolid.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VPVParameterisation.hh"
#include "G4UnitsTable.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring only energy deposit.
//
//
// Created: 2005-11-14  Tsukasa ASO, Akinori Kimura.
//
///////////////////////////////////////////////////////////////////////////////

PSDoseDeposit::PSDoseDeposit(G4int Nvox, G4double SphereRad, G4String name, G4int depth)
  :G4VPrimitiveScorer(name,depth),HCID(-1),Nvoxels(Nvox),SphereRadius(SphereRad),PI(3.141592653589793)
{;}

PSDoseDeposit::~PSDoseDeposit()
{;}

G4bool PSDoseDeposit::ProcessHits(G4Step* aStep, G4TouchableHistory*) {

  if (aStep->GetPreStepPoint()->GetCharge()!=0.) {
    return false;
  }

  G4double edep = 0.0;
  edep = aStep->GetTotalEnergyDeposit();
  G4double en1 = aStep->GetPreStepPoint()->GetKineticEnergy();
  if (en1 < 5.*keV) return false;
  if (edep == 0.) return FALSE;

  G4VPhysicalVolume* physVol = aStep->GetPreStepPoint()->GetPhysicalVolume();
  G4VSolid* solid = physVol->GetLogicalVolume()->GetSolid();

  G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();
  G4double volume = solid->GetCubicVolume();

  G4double dose = edep / volume / density;
  EvtMap->add(0, dose);
  return TRUE;
}

void PSDoseDeposit::Initialize(G4HCofThisEvent* HCE)
{
  EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(),
				    GetName());
  if(HCID < 0) {HCID = GetCollectionID(0);}
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PSDoseDeposit::EndOfEvent(G4HCofThisEvent*) {}

void PSDoseDeposit::clear() {
  EvtMap->clear();
}

void PSDoseDeposit::DrawAll() {}

void PSDoseDeposit::PrintAll() {
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first
    << "  dose deposit: " << G4BestUnit(*(itr->second),"Dose")
    << G4endl;
  }
}
