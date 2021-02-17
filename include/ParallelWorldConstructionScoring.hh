#ifndef ParallelWorldConstructionScoring_h
#define ParallelWorldConstructionScoring_h 1

#include <string>
#include <vector>
//#include <iostream>

#include "globals.hh"


#include "G4VUserParallelWorld.hh"
//#include "PatientMaterial.hh"
//#include "SourceWorldConstruction.hh"
//#include "ParallelWorldMessenger.hh"
#include "DetectorConstruction.hh"

//class ParallelWorldMessenger;
class G4LogicalVolume;


class ParallelWorldConstructionScoring : public G4VUserParallelWorld {
  public:
  explicit ParallelWorldConstructionScoring(G4String&);
  ~ParallelWorldConstructionScoring() {; };

  virtual void Construct();
  virtual void ConstructSD();

  G4int GetNumVoxelsRow();
  G4int GetNumVoxelsCol();
  G4int GetNumVoxels();
  std::vector<G4String> GetColumnLabels();
  std::vector<G4String> GetRowLabels();
  //void InitScoring(SourceWorldConstruction *);
  G4bool UseTrackLength() {
    return fUseTrackLength;
  }

  void setPhantFilename(G4String);

  void GetMinPatient(G4double& x, G4double& y, G4double& z) const {
    x = XMin;
    y = YMin;
    z = ZMin;
  }

  void GetNVoxelPatient(G4int& Nx, G4int& Ny, G4int& Nz) const {
    Nx = NumBinX;
    Ny = NumBinY;
    Nz = NumBinZ;
  }

  void GetHDPatient(G4double& hDx, G4double& hDy, G4double& hDz) const {
    hDx = halfVoxelDimensionX;
    hDy = halfVoxelDimensionY;
    hDz = halfVoxelDimensionZ;
  }

  void GetVTPatient(G4double& vTx, G4double& vTy, G4double& vTz) const {
    vTx = pixel_spacing_X;
    vTy = pixel_spacing_Y;
    vTz = pixel_spacing_Z;
  }

  void setRefAk(G4double);
  void setAkPerHistory(G4double);
  void setTotalTime(G4double);
  void setTrackLength(G4bool);

  G4double GetTotalTime() {
    return fTotalTime;
  }

  G4double GetRefAk() {
    return fRefAk;
  };

  G4double GetAkPerHistory() {
    return fAkPerHistory;
  };

  G4double GetXMin() {
    return XMin;
  };

  G4double GetXMax() {
    return XMax;
  };

  G4double GetYMin() {
    return YMin;
  };

  G4double GetYMax() {
    return YMax;
  };

  G4double GetZMin() {
    return ZMin;
  };

  G4double GetZMax() {
    return ZMax;
  };

  G4int GetNumBinX() {
    return NumBinX;
  };

  G4int GetNumBinY() {
    return NumBinY;
  };

  G4int GetNumBinZ() {
    return NumBinZ;
  };

  G4double GetPixelSpacingX() {
    return pixel_spacing_X;
  };

  G4double GetPixelSpacingY() {
    return pixel_spacing_Y;
  };

  G4double GetPixelSpacingZ() {
    return pixel_spacing_Z;
  };

  G4double* GetDensities() {
    return densities;
  }

  size_t* GetMatNumbers() {
    return materialNumbers;
  }

  std::vector<G4String> GetMatNames() {
    return matNames;
  }

  private:
    G4String phantomFilename;
    G4double fAkPerHistory;
    G4double fRefAk;
    G4double fTotalTime;
    G4double ThetaMax;
    G4double ThetaMin;
    G4double RhoMin;
    G4double RhoMax;
    G4int thetaInc;
    G4int rhoInc;
    G4double RadialResolution;
    G4double AngularResolution;


    G4bool fUseTrackLength;

    G4double pixel_spacing_X;
    G4double pixel_spacing_Y;
    G4double pixel_spacing_Z;
    //PatientMaterial *mat;
    G4int NumBinX, NumBinY, NumBinZ;
    G4int nVoxels;
    G4double halfVoxelDimensionX, halfVoxelDimensionY, halfVoxelDimensionZ;
    G4double *densities;
    size_t *materialNumbers;
    std::vector<G4String> matNames;

    G4double XMax, XMin;
    G4double YMax, YMin;
    G4double ZMin, ZMax;
    G4LogicalVolume* Scoring_log;
    //DetectorConstruction *egsphant;
};

#endif
