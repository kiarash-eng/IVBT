#include "SphericalParameterisation.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"

// SphericalParameterisation::SphericalParameterisation(const G4double minRadius, const G4double maxRadius, const G4double deltaTheta)
//   :G4VPVParameterisation()
//   ,fMinRadius(minRadius)
//   ,fMaxRadius(maxRadius)
//   ,fDeltaTheta(deltaTheta)
// {}

SphericalParameterisation::SphericalParameterisation(const G4double startRadius, const G4double stepRadius, const G4double thetaSpread)
:G4VPVParameterisation(),startradius(startRadius),stepradius(stepRadius),thetaspread(thetaSpread)
{}

SphericalParameterisation::~SphericalParameterisation() {}

// void SphericalParameterisation::ComputeDimensions(G4Sphere& sphere, const G4int copyNo, const G4VPhysicalVolume*) const
// {
//   G4double startTheta;
//   if (copyNo == 0)
//     {
//       startTheta = 0.*deg;
//       sphere.SetStartThetaAngle(startTheta);
//       sphere.SetDeltaThetaAngle(fDeltaTheta/2);
//     }
//   else if (copyNo == 36)
//     {
//       startTheta = 180.*deg-fDeltaTheta/2;
//       sphere.SetStartThetaAngle(startTheta);
//       sphere.SetDeltaThetaAngle(fDeltaTheta/2);
//     }
//   else
//     {
//       startTheta = copyNo*fDeltaTheta-fDeltaTheta/2;
//       sphere.SetStartThetaAngle(startTheta);
//       sphere.SetDeltaThetaAngle(fDeltaTheta);
//     }
//   sphere.SetInsideRadius(fMinRadius);
//   sphere.SetOuterRadius(fMaxRadius);
//   sphere.SetStartPhiAngle(0.*deg);
//   sphere.SetDeltaPhiAngle(360.*deg);
// }

void SphericalParameterisation::ComputeDimensions(G4Sphere& sphere, const G4int copyNo, const G4VPhysicalVolume*) const
{

 sphere.SetInnerRadius((copyNo)*stepradius+startradius);
 sphere.SetOuterRadius((copyNo+1)*stepradius+startradius);
 sphere.SetStartPhiAngle(0*deg);
 sphere.SetDeltaPhiAngle(360*deg);
 sphere.SetStartThetaAngle(90.*deg-thetaspread/2);
 sphere.SetDeltaThetaAngle(thetaspread);


	// Pretty sure this code is broken,
	// Fixing it 
	/*
    G4double startRadius = (copyNo+1)*fStepRadius-fDeltaRadius/2;
    sphere.SetInnerRadius(startRadius);
    sphere.SetOuterRadius(startRadius+fDeltaRadius);
    sphere.SetStartPhiAngle(0*deg);
    sphere.SetDeltaPhiAngle(360*deg);
    //This works so that the spherical shells are always centered about the 90 degree point
    sphere.SetStartThetaAngle(90.*deg-fDeltaTheta/2);
    sphere.SetDeltaThetaAngle(fDeltaTheta);
*/
}





