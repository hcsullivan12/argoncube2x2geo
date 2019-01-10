//
// File: VoxelTable.h
//
// Author: Hunter Sullivan
//
// Discription:
//

#ifndef VOXELTABLE_H
#define VOXELTABLE_H

#include "Voxel.h"

#include <vector>
#include <list>
#include <string>
#include <algorithm>    

namespace majorana 
{

class VoxelTable
{

public:
  static VoxelTable* Instance();
  ~VoxelTable();

  void Initialize(const std::string& voxelizationPath);

  const std::list<Voxel>& GetVoxels() const { return m_voxelList; };
  const Voxel&            GetVoxel(const unsigned& id); 
  
private:
  VoxelTable();
  static VoxelTable* instance;

  std::list<Voxel> m_voxelList;

};
}
#endif
