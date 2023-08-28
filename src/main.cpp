#include <Eigen/Core>
#include <Eigen/Dense>
#include <iostream>
#include <limits>
#include <vector>

#include "model.h"
#include "renderer.h"
#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

int main(int argc, char **argv) {
  const int width = 800;
  const int height = 800;
  Renderer *renderer = new Renderer(width, height);
  Model *model = nullptr;

  if (argc == 2) {
    model = new Model(argv[1]);
  } else {
    model = new Model("obj/african_head/african_head.obj");
  }

  float *zbuffer = new float[width * height];
  for (int i = width * height; i--;) {
    zbuffer[i] = -std::numeric_limits<float>::max();
  }

  TGAImage image(width, height, TGAImage::RGB);
  TGAImage texture;
  texture.read_tga_file("obj/african_head/african_head_diffuse.tga");
  texture.flip_vertically();

  Eigen::Vector3f light_dir(0, 0, -1);  // Define light_dir

  for (int i = 0; i < model->nfaces(); i++) {
    std::vector<int> face = model->face(i);
    std::vector<int> face_texture = model->face_texture(i);
    Eigen::Vector3f pts[3];
    Eigen::Vector2f uvs[3];
    Eigen::Vector3f world_coords[3];
    for (int j = 0; j < 3; j++) {
      pts[j] = renderer->world2screen(model->vert(face[j]));
      uvs[j] = model->texture_vert(face_texture[j]);
      world_coords[j] = model->vert(face[j]);
    }
    Eigen::Vector3f n = (world_coords[2] - world_coords[0]).cross(world_coords[1] - world_coords[0]);
    n.normalize();
    float intensity = n.dot(light_dir);
    if (intensity > 0) {
      renderer->triangle(pts, uvs, zbuffer, image, texture, intensity);
    }
  }

  image.flip_vertically();
  image.write_tga_file("output.tga");

  delete model;
  delete renderer;
  delete[] zbuffer;

  return 0;
}
