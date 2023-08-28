#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>

#include "geometry.h"

class Model {
 private:
  std::vector<Eigen::Vector3f> verts_;          // Use Eigen's Vector3f
  std::vector<Eigen::Vector2f> texture_verts_;  // Use Eigen's Vector2f
  std::vector<std::vector<int>> faces_;
  std::vector<std::vector<int>> face_textures_;

 public:
  Model(const char *filename);
  ~Model();
  int nverts();
  int ntextureverts();
  int nfaces();
  Eigen::Vector3f vert(int i);          // Return Eigen's Vector3f
  Eigen::Vector2f texture_vert(int i);  // Return Eigen's Vector2f
  std::vector<int> face(int idx);
  std::vector<int> face_texture(int idx);
};

#endif  //__MODEL_H__
