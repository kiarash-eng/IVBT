#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>

#include "TrackLengthEstimator.hh"
#include "TrackLengthMessenger.hh"
#include "G4VSolid.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VPVParameterisation.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"

//G4int numEnergyBins = 1400;
//G4int numEnergyBinsLow = 10;
//G4double widthEnergyBinHigh = 1*keV;
//G4double widthEnergyBinLow = 0.1*keV;
//static std::vector<G4double> binCounts (50*numEnergyBins,0);

//G4String filename = "PrimEl.csv";
//std::ofstream out(filename.data());

TrackLengthEstimator::TrackLengthEstimator(G4String name, G4int depth)
  :G4VPrimitiveScorer(name,depth),HCID(-1) {
  SetUnit("Gy");
  fNrow = 0;
  fNcol = 0;
  trackLengthMessenger = new TrackLengthMessenger(this);
}

void TrackLengthEstimator::loadMus(std::vector<G4String> matNames)
{
  this->numMaterials = matNames.size();

  this->matList = matNames;
  for (G4int i = 0; i < this->numMaterials; i++) {

    G4String matName = matList[i];

    //You will get an exception here if you haven't sourced BSOURCE_DIR
    G4String filename = std::string(std::getenv("BSOURCE_DIR")) + "/mu_dats/" + matName + ".dat";
    //The issue was this was making the filename lower case, but my file path wasn't lowercase
    //std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
    std::cout << filename << std::endl;

    std::ifstream muFile(filename);
    if(!muFile) {
        G4cerr << "Cannot open " << filename << "." << G4endl;
        exit(1);
        return;
    }
    int binsForMat;
    muFile >> binsForMat;
    std::vector<G4double> energies;
    std::vector<G4double> mooValues;
    std::vector<G4double> mooenValues;
    for (G4int j = 0; j < binsForMat; j++) {
      G4double energy, moo, mooen;
      muFile >> energy >> moo >> mooen;

      energy *= MeV;
      moo *= cm2/g;
      mooen *= cm2/g;

      energies.push_back(energy);
      mooValues.push_back(moo);
      mooenValues.push_back(mooen);
    }
    muEnergies[matName] = energies;
    muValues[matName] = mooValues;
    muenValues[matName] = mooenValues;

    muFile.close();
  }
}

TrackLengthEstimator::TrackLengthEstimator(G4String name, const G4String& unit,G4int depth):G4VPrimitiveScorer(name,depth),HCID(-1)
{
  SetUnit(unit);
}

TrackLengthEstimator::~TrackLengthEstimator()
{
  delete trackLengthMessenger;
}

G4bool TrackLengthEstimator::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
    G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();

    //Get the replica # aka index number. We keep track of voxels by the order they were constructed in.
    //G4int index = ((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(indexDepth);
    G4int index = GetIndex(aStep);
    G4double cubicVolume = ComputeVolume(aStep, index);
    G4double stepLength = aStep->GetStepLength();
    G4double edep = aStep->GetTotalEnergyDeposit();
    G4double density = aStep->GetPreStepPoint()->GetMaterial()->GetDensity();
    //G4double dose = (edep)/(density*cubicVolume);
    G4double dose = ((edep)/((density)*(cubicVolume)))/gray;
    //G4cout<<"edep:  "<<edep<<" density: "<<density<<" cubicVolume: "<<cubicVolume<<G4endl;
    //G4cout<<"edep in new units:  "<<edep/eV<<" density: "<<density/(kg/m3)<<" cubicVolume: "<<cubicVolume/m3<<G4endl;
    //G4cout<<" dose in default units: "<<dose<<" dose in Gy: "<<dose/gray<<G4endl;


    if (stepLength==0. || edep==0.) {return false;}

    //G4int index = GetIndex(aStep);
    EvtMap->add(index,dose);

    return TRUE;
}

void TrackLengthEstimator::Initialize(G4HCofThisEvent* HCE) {
    EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(),GetName());
    if(HCID < 0) {HCID = GetCollectionID(0);}
    HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void TrackLengthEstimator::EndOfEvent(G4HCofThisEvent*) {;}

void TrackLengthEstimator::clear() {
    EvtMap->clear();
}

void TrackLengthEstimator::DrawAll() {;}

void TrackLengthEstimator::PrintAll() {
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
      G4cout << "  copy no.: " << itr->first
       << "  dose deposit: "
       << *(itr->second)/GetUnitValue()
       << " ["<<GetUnit() <<"]"
       << G4endl;
  }
}

void TrackLengthEstimator::SetUnit(const G4String& unit) {
    CheckAndSetUnit(unit,"Dose");
}

G4double TrackLengthEstimator::ComputeVolume(G4Step* aStep, G4int idx) {
  G4VPhysicalVolume* physVol = aStep->GetPreStepPoint()->GetPhysicalVolume();
  G4VPVParameterisation* physParam = physVol->GetParameterisation();
  G4VSolid* solid = 0;
  if(physParam) { // for parameterized volume
      if(idx<0) {
          G4ExceptionDescription ED;
          ED << "Incorrect replica number --- GetReplicaNumber : " << idx << G4endl;
          G4Exception("TrackLengthEstimator::ComputeVolume","DetPS0004",JustWarning,ED);
	    }

      solid = physParam->ComputeSolid(idx, physVol);
      solid->ComputeDimensions(physParam,idx,physVol);
  } else {
      solid = physVol->GetLogicalVolume()->GetSolid();
  }
  //This returns an estimation of the volume, even though it's "cubic" I believe it's still accurate
  //Returns a cubic estimation
  return solid->GetCubicVolume();
}

G4int TrackLengthEstimator::FindBinLocation(G4double energy, std::vector<G4double>& points) {
  G4double e0 = points[0];
  G4int numBins = points.size();
  if (energy < e0) {
      energy = e0;
  }

  G4int lowerBound = 0;
  G4int upperBound = numBins - 1;

  // Binary search
  while (lowerBound <= upperBound) {
      G4int midBin = (lowerBound + upperBound) / 2;

      if ( energy < points[midBin] ) {
          upperBound = midBin-1;
      } else {
          lowerBound = midBin+1;
      }
    }

  return upperBound;
}



// This function is taken from G4LinLogLogInterpolation
G4double TrackLengthEstimator::linLogLogInterpolate(G4double x, G4int bin,
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
  } else {
      value = data[nBins];
  }

  return value;
}

//////////////added recently
G4int TrackLengthEstimator::GetIndex(G4Step* aStep)
{
  //G4cout << indexDepth << G4endl;
  if(indexDepth != 0) {
  const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();
  G4int irow = touchable->GetReplicaNumber(0);
  G4int icol = touchable->GetReplicaNumber(1);
  return irow*fNcol+icol;

  }
  else {
  return G4VPrimitiveScorer::GetIndex(aStep);
    //return aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(0);
  }
  //G4cout << "position: " << aStep->GetPreStepPoint()->GetPosition() << G4endl << "irow: " <<  irow << " icol: " << icol << " index: " << icol*fNrow+irow << G4endl;


  //return icol*fNrow+irow;
}

void TrackLengthEstimator::SetNumberCol(G4int ncol) {
  this->fNcol=ncol;
}

void TrackLengthEstimator::SetNumberRow(G4int nrow) {
  this->fNrow=nrow;
}
/*
G4int TrackLengthEstimator::FindCounts(G4int n){
  return binCounts[n];
}

G4int TrackLengthEstimator::FindNumEnergyBins(){
  return numEnergyBins;
}
*/
