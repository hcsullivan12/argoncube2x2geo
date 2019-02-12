// 
// File: PixelPlane.h
//
// Author: Hunter Sullivan
//

#ifndef PIXELPLANE_H
#define PIXELPLANE_H

#include "G4LogicalVolume.hh"

namespace geo
{

class PixelPlane 
{
  public:
    PixelPlane();
    ~PixelPlane();

    G4LogicalVolume* ConstructVolume(const std::string&   name,
                                     const G4double&      halfThickness,
                                     const G4double&      xHalfL,
                                     const G4double&      yHalfL,
                                     const G4double&      pixelPitch,
                                     const G4double&      pixelRMax);
};
}

#endif
