//
// ********************************************************************
// *                                                                  *
// *  TOPAS X2 Custom component                                      *
// *                                                                  *
// *  Original Code written by: Nicholas P. Nelson on 9/9/2019        *
// *  Edits and TOPAS compatibility added by: Nicholas P. Nelson      *
// *  University of Wisconsin Medical Radiation Research Center       *	 	                    
// *                                                                  *
// ********************************************************************
//

#ifndef X2_hh
#define X2_hh

#include "TsVGeometryComponent.hh"

class X2 : public TsVGeometryComponent
{    
public:
	X2(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
				  TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name);
	~X2();
	
	G4VPhysicalVolume* Construct();
};

#endif
