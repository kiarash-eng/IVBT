
////////////////////////////////////////////////////////////////////////////
//
// Author: Shirin A. Enger <shirin@enger.se>
//
// License & Copyright
// ===================
// 
// Copyright 2015 Shirin A Enger <shirin@enger.se>
// 
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//////////////////////////////////////////////////////////////////////////

#ifndef SourceMaterial_H
#define SourceMaterial_H 1

#include "globals.hh"
class G4Material;

class SourceMaterial
{ 
public:
  SourceMaterial();
  ~ SourceMaterial();

public:
  void  DefineMaterials();
  G4Material* GetMat(G4String); //returns the material

private:
  
  G4Material* Air;
  G4Material* H2O;
  G4Material* ceramic;
  G4Material* Vacuum; 
  G4Material* plexiglass;
  G4Material* steel;
  G4Material* gold;
  G4Material* Pb;
  G4Material* W; 
  G4Material* Sn;
  G4Material* Se;
  G4Material* I; 
  G4Material* Ir; 
  G4Material* Co;
  G4Material* Gd;
  G4Material* Pd;
};
#endif
