//////////////////////////////////////////////////////////////////////////
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

// USER //
#include "SourceMaterial.hh"

// GEANT4 //
#include "globals.hh"
#include "Randomize.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4MaterialTable.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4RunManager.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4NistManager.hh"

SourceMaterial::SourceMaterial():
  Air(0),H2O(0),ceramic(0),Vacuum(0),plexiglass(0),steel(0),
  gold(0),Pb(0),W(0),Sn(0),Se(0),I(0),Ir(0),Co(0),Gd(0),Pd(0)
{}

SourceMaterial::~SourceMaterial() {}

void SourceMaterial::DefineMaterials()
{
  G4NistManager* nist_manager = G4NistManager::Instance();
  G4Element*  H  = nist_manager->FindOrBuildElement("H");
  G4Element*  Fe = nist_manager->FindOrBuildElement("Fe");
  G4Element*  Cr =nist_manager->FindOrBuildElement("Cr");
  G4Element*  Ni = nist_manager->FindOrBuildElement("Ni");
  G4Element*  Mn = nist_manager->FindOrBuildElement("Mn");
  G4Element*  Si = nist_manager->FindOrBuildElement("Si");
  G4Element*  C  = nist_manager->FindOrBuildElement("C");
  G4Element*  O  = nist_manager->FindOrBuildElement("O");
  G4Element*  Al  = nist_manager->FindOrBuildElement("Al");


  Ir = nist_manager->FindOrBuildMaterial("G4_Ir");
  Se = nist_manager->FindOrBuildMaterial("G4_Se");
  Gd = nist_manager->FindOrBuildMaterial("G4_Gd");
  I = nist_manager->FindOrBuildMaterial("G4_I");
  W = nist_manager->FindOrBuildMaterial("G4_W");
  Pb = nist_manager->FindOrBuildMaterial("G4_Pb");
  Sn = nist_manager->FindOrBuildMaterial("G4_Sn");
  Air  = nist_manager->FindOrBuildMaterial("G4_AIR");
  H2O =nist_manager->FindOrBuildMaterial("G4_WATER");

  // Perspex, plexiglass, lucite

  plexiglass = new G4Material("Plexiglass",1.19*g/cm3,3);
  plexiglass->AddElement(H,0.08);
  plexiglass->AddElement(C,0.60);
  plexiglass->AddElement(O,0.32);

  // Stainless steel
  steel = new G4Material( "Steel", 8.0*g/cm3, 6 );
  steel->AddElement( Fe, 0.6792 );
  steel->AddElement( Cr, 0.1900 );
  steel->AddElement( Ni, 0.1000 );
  steel->AddElement( Mn, 0.0200 );
  steel->AddElement( Si, 0.0100 );
  steel->AddElement( C , 0.0008 );


  gold = new G4Material("gold",79.,196.97*g/mole,19.32*g/cm3);



  //ceramic(Medical Physics, May 2000)

  ceramic = new G4Material("allumina",2.88*g/cm3,2);
  ceramic->AddElement(Al,2);
  ceramic->AddElement(O,3);

  G4double density = universe_mean_density;
  G4double pressure = 3.e-18*pascal;
  G4double temperature = 2.73*kelvin;

  Vacuum = new G4Material("Galactic", 1.,1.01*g/mole,density,kStateGas,temperature,pressure);


}

G4Material* SourceMaterial::GetMat(G4String material)
{
  // Returns a material
  G4Material* pttoMaterial = G4Material::GetMaterial(material);
  return pttoMaterial;
}

