#include "TG60Parameterisation.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"

TG60Parameterisation::TG60Parameterisation(const G4double startRadius, const G4double stepRadius, const G4double incrementRadius, const G4double startZ, const G4double stepZ, const G4double incrementZ)
:G4VPVParameterisation(),startradius(startRadius),stepradius(stepRadius),incrementradius(incrementRadius), startz(startZ), stepz(stepZ), incrementz(incrementZ)
{
	radiusNumber = 1;
	zNumber = 0;
}

TG60Parameterisation::~TG60Parameterisation() {}

void TG60Parameterisation::ComputeDimensions(G4Tubs& cylinder, const G4int copyNo, const G4VPhysicalVolume*) const
{
 //radno starts at 1, once incrementz copies have been made, radno increases
 //Castiing a double to (int)radno, allows us to in effect round, which is why radno only increases by whole number values
 G4int radno = (G4int)((copyNo)/(incrementz) + 1);
 //Return if radno is greaterthan increment radius, this means the user entered to many voxels
 if(radno > incrementradius) { G4cout << "Too many voxels entered into TG60Parameterisation, Voxel #:" << copyNo+1 << G4endl; return;}

 cylinder.SetOuterRadius(radno*stepradius + startradius -(0.05*mm));
 cylinder.SetInnerRadius((radno-1)*stepradius + startradius);
 cylinder.SetZHalfLength(stepz/2);
}

void TG60Parameterisation::ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physical) const
{
 //Same definition of radno as above
	G4int radno = (G4int)((copyNo)/(incrementz) + 1);
	if(radno > incrementradius) {return;}
	// Z number is like copyNo that resets every row. 
	// radno-1 is because radno by default starts at 1
	// Each time it subtracts zNumber starts at 0 again
	G4int zNumber = copyNo - (radno-1)*(incrementz);

 G4double zpos = startz - zNumber*stepz - stepz/2;
 G4ThreeVector origin(0, 0, zpos);
 physical->SetTranslation(origin);
 physical->SetRotation(0);

}

//Mmmmm we could do something cool
//Get logicalvolume, then set vis attributes, then setlogical volume
//So we could visualize





