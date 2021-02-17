#ifndef RadialParallelWorldConstructionScoring_h
#define RadialParallelWorldConstructionScoring_h 1

#include <string>
#include <vector>
#include <iostream>

#include "globals.hh"


#include "G4VUserParallelWorld.hh"
//#include "PatientMaterial.hh"
//#include "SourceWorldConstruction.hh"
//#include "ParallelWorldMessenger.hh"
#include "DetectorConstruction.hh"

class ParallelWorldMessenger;

class RadialParallelWorldConstructionScoring : public G4VUserParallelWorld {
  public:
  RadialParallelWorldConstructionScoring(G4String&, DetectorConstruction*,int voxels);

  ~RadialParallelWorldConstructionScoring() { }

  virtual void Construct();

  //void InitScoring(SourceWorldConstruction *);

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

  G4bool UseTrackLength() {
    return fUseTrackLength;
  }

  private:
    G4String phantomFilename;
    G4double fAkPerHistory;
    G4double fRefAk;
    G4double fTotalTime;
    G4bool fUseTrackLength;
    ParallelWorldMessenger *fMessenger;

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
    //DetectorConstruction *egsphant;
};

#endif

