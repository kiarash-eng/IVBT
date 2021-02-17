
#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

class DetectorMessenger;
class G4LogicalVolume;
class G4Material;
class G4Box;
class G4Orb;
class G4Sphere;
class G4Colour;
class G4VPhysicalVolume;
class G4VPhysicalVolume;
class SourceMaterial;
class SourceModel;

#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"


class DetectorConstruction : public G4VUserDetectorConstruction
{
	public:

  	//DetectorConstruction(G4int NumberVoxels=100);
		DetectorConstruction();
  	~DetectorConstruction();

    void SelectBrachySource(G4String);
		void SelectBrachyCore(G4String);
    void SelectLumenDiameter(G4String);
		void SelectStentModel(G4String);
		void SelectStentNumber(G4String);
		void SelectNeointimalFibrosis(G4String);
		void SelectCalcifiedThickness(G4String);
		void SelectStentOverlapping(G4String);
		void ConstructWorld();


  	G4VPhysicalVolume* Construct();
    G4Material* GetMaterial() {return theMaterial;};
    private:
    G4Material* theMaterial;
		G4Material* Media;
		G4Material* FibroticPlaque;
		G4Material* CalcifiedPlaque;
		G4Material* stent;



   // Logical volumes
    //
    G4LogicalVolume* World_log;
    G4LogicalVolume* Phantom_log;
    G4LogicalVolume* Afterloader_log;
    G4LogicalVolume* Scoring_log;

    // Physical volumes
    //
    G4VPhysicalVolume* World_phys;
    G4VPhysicalVolume* Phantom_phys;
    G4VPhysicalVolume* Afterloader_phys;

    G4double ScoringSphereRadius;
    G4double WorldSize;
    G4double ArteryShift;
		G4double Neointima;
		G4double Source_misPosition;

    SourceMaterial* pMaterial;
    DetectorMessenger* detectorMessenger;
    G4String fBrachySource;
		G4String fCore;
		G4String StentModel;
    G4String fLumenDiameter;
		G4String fStentOverlapping;
		G4String fStentModel;
		G4String fStentNumber;
		G4String fNeofibrosis;
		G4String fCalcifiedPlaque;
		G4double Vessel_thickness;
    SourceModel* Source;

  G4int Nvoxels;
	G4int Ring_number;
	G4int Rod_number;
	G4double StentNumber;
	G4double LumenDiameter;
	G4double Stent_thickness;
	G4double Ring_thickness;
	G4double Ring_Longitudinal_length;
	G4double Ring_spacing;
	G4double RVR;
	G4double RVD;
	G4double mediaThickness;
	G4double adventitiaThickness;
	G4double LesionLength;
	G4double ArteryLength;
	G4double Plaque_thickness;
	G4double Temperature;
	G4double CalcifiedThickness;
};

#endif
