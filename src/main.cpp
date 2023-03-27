#include <cmath>
#include <cstdlib>
#include <limits>
#include <vector>

#include "geometry.h"
#include "model.h"
#include "renderer.h"
#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
Model *model = NULL;
const int width = 800;
const int height = 800;
Renderer *renderer = NULL;
Vec3f light_dir(0, 0, -1);  // define light_dir

int main(int argc, char **argv) {
  renderer = new Renderer(width, height);
  if (2 == argc) {
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
  /**
   * @brief ref: https://github.com/ssloy/tinyrenderer/wiki/Visual-troubleshooting#texturing
   * > The texture needs to be vertically flipped:
   */
  texture.flip_vertically();

  for (int i = 0; i < model->nfaces(); i++) {
    std::vector<int> face = model->face(i);
    std::vector<int> face_texture = model->face_texture(i);
    Vec3f pts[3];
    Vec2f uvs[3];
    Vec3f world_coords[3];
    for (int i = 0; i < 3; i++) {
      pts[i] = renderer->world2screen(model->vert(face[i]));
      uvs[i] = model->texture_vert(face_texture[i]);
      world_coords[i] = model->vert(face[i]);
    }
    Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
    n.normalize();
    float intensity = n * light_dir;
    if (intensity > 0) {
      renderer->triangle(pts, uvs, zbuffer, image, texture, intensity);
    }
  }

  image.flip_vertically();  // i want to have the origin at the left bottom corner of the image
  image.write_tga_file("output.tga");
  delete model;
  return 0;
}
