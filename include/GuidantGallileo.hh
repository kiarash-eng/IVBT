//////////////////////////////////////////////////////////////////////////
//
// Author: Joseph M. Decunha <joseph.decunha@mail.mcgill.ca>
//
// License & Copyright
// ===================
//
// Copyright 2016 Joseph M. Decunha <joseph.decunha@mail.mcgill.ca>
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
//
//
//-----------------------------------------------------------------------
// Header file that defines the Guidant Galileo 20mm model 
// for Intravascular Brachytherapy
//////////////////////////////////////////////////////////////////////////


#ifndef GuidantGallileo_H
#define GuidantGallileo_H 1

#include "globals.hh"
#include "SourceModel.hh"

class GuidantGallileo : public SourceModel
{
public:
		GuidantGallileo(G4bool);

		void CreateSource(G4VPhysicalVolume*);
		void CleanSource();
		void SetupDimensions();

private:
		G4double sourceradius, tungstenradius, shellradius;
		G4double sourcelength, tungstenlength, shelllength;
		G4bool guidewire;

};
#endif