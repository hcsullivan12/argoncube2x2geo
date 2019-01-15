//
// File: Voxel.cxx
//
// Author: Hunter Sullivan
//
// Description: Voxel structure.
//

#include "Voxel.h"

namespace majorana
{

Voxel::Voxel(const unsigned& voxelID,
             const float&    x, 
             const float&    y, 
             const float&    r, 
             const float&    theta)
 : m_id(voxelID),
   m_x(x), 
   m_y(y), 
   m_r(r), 
   m_theta(theta)
{
  m_referenceTable.clear();
}

Voxel::~Voxel()
{}

}