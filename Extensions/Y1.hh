//
// ********************************************************************
// *                                                                  *
// *  TOPAS Y1 Custom component                                      *
// *                                                                  *
// *  Original Code written by: Nicholas P. Nelson on 9/9/2019        *
// *  Edits and TOPAS compatibility added by: Nicholas P. Nelson      *
// *  University of Wisconsin Medical Radiation Research Center       *	 	                    
// *                                                                  *
// ********************************************************************
//

#ifndef Y1_hh
#define Y1_hh

#include "TsVGeometryComponent.hh"

class Y1 : public TsVGeometryComponent
{    
public:
	Y1(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
				  TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name);
	~Y1();
	
	G4VPhysicalVolume* Construct();
};

#endif
