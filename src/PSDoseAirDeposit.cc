#include "PSDoseAirDeposit.hh"
#include "G4VSolid.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VPVParameterisation.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;



PSDoseAirDeposit::PSDoseAirDeposit(G4int Nvox, G4double SphereRad, G4String name, G4int depth)
  :G4VPrimitiveScorer(name,depth),HCID(-1),Nvoxels(Nvox),SphereRadius(SphereRad),PI(3.141592653589793)
{
  G4String muenFile="new_muen.dat";
  ifstream fin(muenFile);

  if (!fin) {
    G4cerr << "Cannot open " << muenFile << "." << G4endl;
    return;
  }

  fin >> numBins;
  G4cout << "Muen file: " <<  muenFile << G4endl;

  muenEnergy.resize(numBins);
  muenValue.resize(numBins);

  for (G4int i=0; i<numBins; i++) {
	  fin >> muenEnergy[i] >> muenValue[i];
	  muenEnergy[i] *= MeV;
	  muenValue[i] *= cm2/g;
	  G4cout<< muenValue[i] / (cm2/g) << G4endl;
  }

  fin.close();
  fin.clear();
}

PSDoseAirDeposit::~PSDoseAirDeposit() {}

G4bool PSDoseAirDeposit::ProcessHits(G4Step* aStep, G4TouchableHistory*) {

  if (aStep->GetPreStepPoint()->GetCharge()!=0.) return false;

  G4double stepLength=aStep->GetStepLength();
  if (stepLength==0.) return false;

  G4double energy = aStep->GetPreStepPoint()->GetKineticEnergy();
  if (energy < 5.*keV) return false;
  G4VPhysicalVolume* physVol = aStep->GetPreStepPoint()->GetPhysicalVolume();
  G4VSolid *solid = physVol->GetLogicalVolume()->GetSolid();
  G4double volume = solid->GetCubicVolume();
  G4int binNumber = FindBinLocation(energy);
  G4double muen = linLogLogInterpolate(energy, binNumber, muenEnergy, muenValue);

  G4double dose = stepLength * muen * energy / volume;
  EvtMap->add(0,dose);
  return TRUE;

}

void PSDoseAirDeposit::Initialize(G4HCofThisEvent* HCE) {
  EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(),
				    GetName());
  if(HCID < 0) {HCID = GetCollectionID(0);}
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PSDoseAirDeposit::EndOfEvent(G4HCofThisEvent*) {}

void PSDoseAirDeposit::clear() {
  EvtMap->clear();
}

void PSDoseAirDeposit::DrawAll() {}

void PSDoseAirDeposit::PrintAll() {
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

// This function is taken from the example program xray_fluorescence
G4int PSDoseAirDeposit::FindBinLocation(G4double energy) {
  // Protection against call outside allowed range
  G4double e0 = muenEnergy[0];
  if (energy < e0) {
    energy = e0;
  }

  G4int lowerBound = 0;
  G4int upperBound = numBins - 1;

  // Binary search
  while (lowerBound <= upperBound) {
    G4int midBin = (lowerBound + upperBound) / 2;
    if (energy < muenEnergy[midBin]) {
      upperBound = midBin-1;
    }
    else {
      lowerBound = midBin+1;
    }
  }

  return upperBound;
}

// This function is taken from G4LinLogLogInterpolation
G4double PSDoseAirDeposit::linLogLogInterpolate(G4double x, G4int bin,
                                      std::vector<G4double>& points,
                                      std::vector<G4double>& data)
{
  G4int nBins = data.size() - 1;
  G4double value = 0.;
  if (x < points[0]) {
    value = 0.;
  }
  else if (bin < nBins) {
    G4double e1 = points[bin];
    G4double e2 = points[bin+1];
    G4double d1 = data[bin];
    G4double d2 = data[bin+1];
    if(d1 > 0.0 && d2 > 0.0) {
      value = (std::log10(d1)*std::log10(e2/x) + std::log10(d2)*std::log10(x/e1)) / std::log10(e2/e1);
      value = std::pow(10.,value);
    } else {
      value = (d1*std::log10(e2/x) + d2*std::log10(x/e1)) / std::log10(e2/e1);
    }
  }
  else {
    value = data[nBins];
  }

  return value;
}
