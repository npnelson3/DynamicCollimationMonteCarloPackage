# GenericPencilBeamScanningSourceTOPAS
By Nick Nelson <npnelson3@gmail.com>

# Features
This is a custom particle generator source extension for the TOPAS Monte Carlo platform. This source was specifically written to model a pencil beam scanning proton therapy beamline. It constists of a divergent point source placed at a user-specified SAD that samples a spot size function and back projects the sampled points to the SAD to deduce the angular source distribution of the source. For most beamlines, magnets will deflect the beam in X and Y at different locations. This source model includes magnet-specific deflections given distances to the X and Y magnets from isocenter. If the magnet deflections are not invoked in the TOPAS parameter file, the divergent point source will rotate to produce the user-specified deflections.

# Installation
Please see the TOPAS documentation for installing extensions. The .cc and .hh files must be placed in your TOPAS extension folder and cmake must be called to compile the new, extended TOPAS application. 

# TOPAS Usage
