#include <string>
#include <fstream>
#include <vector>

#include "globals.hh"
#include "string_utils.hh"
#include "PositionList.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

G4bool PositionList::ReadDataFile(G4String filename) {
  std::ifstream dataFile(filename);
  G4String buffer;

  G4ThreeVector positionVector;
  std::vector<G4int> weightVector;

  if (dataFile.good() != 1) {
    std::cerr << "Could not find file: " << filename << std::endl;
    return 1;
  }

  // Parsing .mac file to get source positions, assumes GPS is being used.
  /* Current one source position looks like:
    /gps/source/add 0.5788
    /gps/particle ion
    /gps/ion 77 192 0 0
    /gps/pos/type Volume
    /gps/pos/shape Cylinder
    /gps/pos/radius 0.03 cm
    /gps/pos/halfz 0.175 cm
    /gps/energy 0 keV
    /gps/position 10.6077 68.2232 -1174.9870 mm
    /gps/pos/confine source
  */
  while (std::getline(dataFile, buffer)) {
    if (buffer.find("/gps/source/add") != std::string::npos) {
      std::vector<std::string> split_vector = split(buffer, ' ');
      weightVector.push_back(std::atof(split_vector[1].c_str()));
    }
    else if (buffer.find("/gps/position") != std::string::npos) {
      std::vector<std::string> split_vector = split(buffer, ' ');
      G4ThreeVector pos(std::atof(split_vector[1].c_str()), std::atof(split_vector[2].c_str()), std::atof(split_vector[3].c_str()));

      if (split_vector.size() > 4) {
        if (split_vector[4] == "mm") pos *= mm;
        if (split_vector[4] == "cm") pos *= cm;
      }

      listOfPositions.push_back(pos);
    }
  }

  totalNumberOfSources = listOfPositions.size();
  dataFile.close();
  return 0;
}

std::vector<G4ThreeVector> PositionList::GetListOfPositions() {
  return listOfPositions;
}


G4ThreeVector PositionList::GetPosition(G4int SourceNumber) {
  G4ThreeVector v = listOfPositions[SourceNumber];
  return v;
}



