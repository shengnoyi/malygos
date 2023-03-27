#include "model.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

Model::Model(const char *filename) : verts_(),
                                     texture_verts_(),
                                     faces_(),
                                     face_textures_() {
  std::ifstream in;
  in.open(filename, std::ifstream::in);
  if (in.fail()) return;
  std::string line;
  while (!in.eof()) {
    std::getline(in, line);
    std::istringstream iss(line.c_str());
    char trash;
    if (!line.compare(0, 2, "v ")) {
      // 到空格会停止；
      iss >> trash;
      Vec3f v;
      for (int i = 0; i < 3; i++) iss >> v[i];
      verts_.push_back(v);
    } else if (!line.compare(0, 4, "vt  ")) {
      iss >> trash;
      iss >> trash;
      Vec2f v;
      for (int i = 0; i < 2; i++) iss >> v[i];
      texture_verts_.push_back(v);
    } else if (!line.compare(0, 2, "f ")) {
      std::vector<int> f;
      std::vector<int> texture;
      int itrash, idx, texture_idx;
      iss >> trash;
      while (iss >> idx >> trash >> texture_idx >> trash >> itrash) {
        // in wavefront obj all indices start at 1, not zero
        f.push_back(idx - 1);
        texture.push_back(texture_idx - 1);
      }
      faces_.push_back(f);
      face_textures_.push_back(texture);
    } else {
      // skip
    }
  }
  std::cerr << "# v# " << verts_.size()
            << " vt# " << texture_verts_.size()
            << " f# " << faces_.size()
            << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
  return (int)verts_.size();
}

int Model::ntextureverts() {
  return (int)texture_verts_.size();
}

int Model::nfaces() {
  return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
  return faces_[idx];
}
std::vector<int> Model::face_texture(int idx) {
  return face_textures_[idx];
}

Vec3f Model::vert(int i) {
  return verts_[i];
}

Vec2f Model::texture_vert(int i) {
  return texture_verts_[i];
}
