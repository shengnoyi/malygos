#include "model.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "geometry.h"


Model::Model(const char *filename)
    : verts_(), texture_verts_(), faces_(), face_textures_() {
  std::ifstream in;
  in.open(filename, std::ifstream::in);
  if (in.fail()) return;

  std::string line;
  while (std::getline(in, line)) {
    std::istringstream iss(line);
    char trash;
    if (line.compare(0, 2, "v ") == 0) {
      iss >> trash;
      Eigen::Vector3f v;
      for (int i = 0; i < 3; i++) iss >> v[i];
      verts_.push_back(v);
    } else if (line.compare(0, 4, "vt  ") == 0) {
      iss >> trash;
      iss >> trash;
      Eigen::Vector2f v;
      for (int i = 0; i < 2; i++) iss >> v[i];
      texture_verts_.push_back(v);
    } else if (line.compare(0, 4, "vn  ") == 0) {
      iss >> trash;
      iss >> trash;
      Eigen::Vector3f v;
      for (int i = 0; i < 3; i++) iss >> v[i];
      normal_verts_.push_back(v);
    } else if (line.compare(0, 2, "f ") == 0) {
      std::vector<int> f;
      std::vector<int> texture;
      std::vector<int> normal;
      int itrash, idx, texture_idx, normal_idx;
      iss >> trash;
      while (iss >> idx >> trash >> texture_idx >> trash >> normal_idx) {
        // in wavefront obj all indices start at 1, not zero
        f.push_back(idx - 1);
        texture.push_back(texture_idx - 1);
        normal.push_back(normal_idx - 1);
      }
      faces_.push_back(f);
      face_textures_.push_back(texture);
      face_normals_.push_back(normal);
    }
  }

  std::cerr << "# v# " << verts_.size()
            << " vt# " << texture_verts_.size()
            << " vn# " << normal_verts_.size()
            << " f# " << faces_.size() << std::endl;
}

int Model::nverts() {
  return static_cast<int>(verts_.size());
}

int Model::ntextureverts() {
  return static_cast<int>(texture_verts_.size());
}

int Model::nfaces() {
  return static_cast<int>(faces_.size());
}

std::vector<int> Model::face(int idx) {
  return faces_[idx];
}

std::vector<int> Model::face_texture(int idx) {
  return face_textures_[idx];
}

std::vector<int> Model::face_normal(int idx) {
  return face_normals_[idx];
}

Eigen::Vector3f Model::vert(int i) {
  return verts_[i];
}

Eigen::Vector2f Model::texture_vert(int i) {
  return texture_verts_[i];
}

Eigen::Vector3f Model::normal_vert(int i) {
  return normal_verts_[i];
}

Model::~Model() {
}
