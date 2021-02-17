#include "G4VPVParameterisation.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include <vector>

class G4LogicalVolume;
class G4Sphere;

class SphericalParameterisation : public G4VPVParameterisation {

public:
  
  // Constructor
  SphericalParameterisation(const G4double startRadius, const G4double stepRadius, const G4double thetaSpread);
  //SphericalParameterisation(const G4double minRadius, const G4double maxRadius, const G4double deltaTheta);
  //SphericalParameterisation(const G4double deltaRadius, const G4double deltaTheta);
  
  // Destructor
  virtual ~SphericalParameterisation();
  
  // Method
  virtual void ComputeDimensions(G4Sphere& sphere, 
				 const G4int copyNo, 
				 const G4VPhysicalVolume*) const;
    
private:
    
    void ComputeTransformation(const G4int, G4VPhysicalVolume*) const {};

    
private:
  
  G4double startradius;
  G4double stepradius;
  G4double thetaspread;

};
