// 
// File: PixelPlane.cxx
//
// Author: Hunter Sullivan
//

#include "Geometry/Auxiliary/PixelPlane.h"
#include "MaterialManager.h"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh"

namespace geo
{

PixelPlane::PixelPlane()
{}

PixelPlane::~PixelPlane()
{}

G4LogicalVolume* PixelPlane::ConstructVolume(const std::string&   name,
                                             const G4double&      xHalfL,
                                             const G4double&      yHalfL,
                                             const G4double&      halfThickness,                                             
                                             const G4double&      pixelSpacing,
                                             const G4double&      pixelRMax)
{
  MaterialManager* matMan = MaterialManager::Instance();

  // Plane
  G4Box* solPixelPlane = new G4Box("sol"+name,
                                    xHalfL,
                                    yHalfL,
                                    halfThickness); 
  G4LogicalVolume* volPixelPlane = new G4LogicalVolume(solPixelPlane,
                                                       matMan->FindMaterial("FR4"),
                                                       "vol"+name);                                    
  // Pad 
  // Set the via length to the plane thickness
  //pixelRMax = 2*cm < pixelSpacing/2.0 ? pixelRMax : pixelSpacing/4.0;
  G4Tubs* solPixelPad = new G4Tubs("solPixelPad",
                                    0,
                                    pixelRMax,
                                    halfThickness,
                                    0*deg, 360*deg);
  G4LogicalVolume* volPixelPad = new G4LogicalVolume(solPixelPad,
                                                     matMan->FindMaterial("Copper"),
                                                     "volPixelPad");                                    

  // Placement
  // Reference point is center of plane
  // Assuming pixels fill entire plane and are symmetrically placed
  // Starting point will be top left corner with respect to the active side
  // First compute boundaries (perhaps there is a better way, but this will work for now)
  G4double thisX = pixelSpacing/2.;
  G4double thisY = pixelSpacing/2.;
  G4double startZ = 0; //solPixelPlane->GetZHalfLength() - solPixelPad->GetZHalfLength();
  
  while ((thisX+pixelRMax) < xHalfL) thisX = thisX+pixelSpacing;
  while ((thisY+pixelRMax) < yHalfL) thisY = thisY+pixelSpacing;
  thisX = thisX-pixelSpacing;
  thisY = thisY-pixelSpacing;

  G4double startX = -1*thisX;
  G4double startY = thisY;
  G4ThreeVector pos(startX, startY, startZ);

  unsigned pixelID = 1;
  while (pos.getY() > -1*(yHalfL-pixelRMax))
  {
    while (pos.getX() < (xHalfL-pixelRMax))
    {
      new G4PVPlacement(0, pos, volPixelPad, volPixelPad->GetName()+std::to_string(pixelID), volPixelPlane, false, pixelID-1);
      pixelID++;

      // next position
      pos.setX(pos.getX() + pixelSpacing);
    }
    pos.setX(startX);
    pos.setY(pos.getY() - pixelSpacing);
  }

  std::cout << "Pixel plane has " << pixelID-1 << " pixels." << std::endl;

  return volPixelPlane;
}
}
