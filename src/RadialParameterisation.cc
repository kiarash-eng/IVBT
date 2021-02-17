#include "RadialParameterisation.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"

RadialParameterisation::RadialParameterisation(const G4double startRadius, const G4double stepRadius, const G4double incrementRadius, const G4double deltaAngle, const G4double zhalfLength)
:G4VPVParameterisation(),startradius(startRadius),stepradius(stepRadius),incrementradius(incrementRadius), deltaangle(deltaAngle), zhalflength(zhalfLength)
{

}

RadialParameterisation::~RadialParameterisation() {}

void RadialParameterisation::ComputeDimensions(G4Tubs& cylinder, const G4int copyNo, const G4VPhysicalVolume*) const
{

 // Goes out radially and then loops over at starting radius and different rho
 
 //angleno starts at 1, once stepradius copies have been made it increments
 G4int angleno = (G4int)((copyNo)/(incrementradius)+1);
 //radno increments with copyno, once angleno passes the threshold it reverts back to zero and starts again
 G4int radno = copyNo - (angleno-1)*(incrementradius);

 cylinder.SetDeltaPhiAngle(deltaangle);
 cylinder.SetStartPhiAngle((deltaangle*angleno - deltaangle));
 //cylinder.SetDeltaPhiAngle(180*deg);
 cylinder.SetOuterRadius(radno*stepradius + startradius -(0.05*mm));
 cylinder.SetInnerRadius((radno-1)*stepradius + startradius);
 cylinder.SetZHalfLength(zhalflength);
 //cylinder.SetDeltaPhiAngle(180*deg);
 //cylinder.SetStartPhiAngle((G4double)((copyNo)/10)*deg, false);
 
 
 
 

 //cylinder.SetInnerRadius(copyNo*0.1*mm);
 //cylinder.SetOuterRadius(copyNo*0.1*mm+0.1*mm);

}

void RadialParameterisation::ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physical) const
{
 G4ThreeVector origin(0, 0, 0);
 physical->SetTranslation(origin);
 physical->SetRotation(0);

}



