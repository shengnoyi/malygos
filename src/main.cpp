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
  TGAImage texture;
  if (argc == 3) {
    model = new Model(argv[1]);
    texture.read_tga_file(argv[2]);
  } else {
    model = new Model("obj/african_head/african_head.obj");
    texture.read_tga_file("obj/african_head/african_head_diffuse.tga");
  }
  texture.flip_vertically();

  Eigen::Vector3f eye(0.0f, 0.0f, 1.5f);
  Eigen::Vector3f center(0.0f, 0.0f, 0.0f);
  Eigen::Vector3f up(0.0f, 1.0f, 0.0f);

  renderer->setModel(Eigen::Matrix4f::Identity());
  renderer->setView(renderer->lookAt(eye, center, up));
  renderer->setProjection(renderer->perspective(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -10.0f));
  renderer->updateMvp();

  TGAImage image(width, height, TGAImage::RGB);
  TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);

  Eigen::Vector3f light_dir(0, 0, -1);  // Define light_dir
  light_dir.normalize();
  for (int i = 0; i < model->nfaces(); i++) {
    std::vector<int> face = model->face(i);
    std::vector<int> face_texture = model->face_texture(i);
    std::vector<int> face_normal = model->face_normal(i);
    Eigen::Vector3f pts[3];
    Eigen::Vector2f uvs[3];
    Eigen::Vector3f world_coords[3];
    float intensities[3];
    for (int j = 0; j < 3; j++) {
      pts[j] = renderer->world2screen(model->vert(face[j]));
      uvs[j] = model->texture_vert(face_texture[j]);
      world_coords[j] = model->vert(face[j]);
      Eigen::Vector3f ity = model->normal_vert(face_normal[j]);
      ity.normalize();
      intensities[j] = std::min(std::max(0.0f, ity.dot(-light_dir)), 1.0f);
    }
    Eigen::Vector3f n = (world_coords[2] - world_coords[0]).cross(world_coords[1] - world_coords[0]);
    n.normalize();
    float intensity = n.dot(light_dir);
    if (intensity > 0) {
      renderer->triangle(pts, uvs, zbuffer, image, texture, intensities);
    }
  }

  image.flip_vertically();
  zbuffer.flip_vertically();
  image.write_tga_file("output.tga");
  zbuffer.write_tga_file("zbuffer.tga");

  delete model;
  delete renderer;

  return 0;
}
