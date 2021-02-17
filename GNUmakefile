name := TG60
G4TARGET := $(name)
G4EXLIB = true

ifndef G4INSTALL
				#G4INSTALL= /home/Kia/geant4_10_06_source/geant4.10.06.p01
        G4INSTALL= ~/Documents/geant4_10.06.p01_source/geant4.10.06.p01
        endif

#EXTRALIBS += $(shell $(ROOTSYS)/bin/root-config --glibs)
#CPPFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags)




.PHONY: all
all: lib bin



include $(G4INSTALL)/config/binmake.gmk
