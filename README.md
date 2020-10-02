# Dynamic Collimation Monte Carlo package
By Nick Nelson <npnelson3@gmail.com>

# Features
The Dynamic Collimation Monte Carlo (DCMC) was developed to help facilitate the simulation of pencil beam scanning proton beams and collimation devices. The DCMC package utilized the the TOPAS Monte Carlo platform and consists of a generalized PBS source model and collimator component extensions.

The source model is particle generator extension for TOPAS and was specifically written to model a pencil beam scanning proton therapy beamline. It constists of a divergent point source placed at a user-specified SAD that samples a spot size function and back projects the sampled points to the SAD to deduce the angular distribution of the source. For most beamlines, magnets will deflect the beam in X and Y at different locations. This source model includes magnet-specific deflections given distances to the X and Y magnets from isocenter. If the magnet deflections are not invoked in the TOPAS parameter file, the divergent point source will rotate to produce the user-specified deflections.

The two collimation components include in the package are trimmers and apertures. The trimmers are modeled after the Dynamic Collimation System and linearly translate while rotating about the magnet planes to match the divergence of the scanned beam. Aperture components were developed to model an aperture-based collimator, such as Mevion's Adaptive Aperture.

# Installation
Please see the TOPAS documentation for installing extensions. The .cc and .hh files must be placed in your TOPAS extension folder and cmake must be called to compile the new, extended TOPAS application. 

# TOPAS Usage
