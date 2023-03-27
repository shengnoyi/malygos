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
  for (int i = width * height; i--; zbuffer[i] = -std::numeric_limits<float>::max()) {
    // noop
  }

  TGAImage image(width, height, TGAImage::RGB);
  for (int i = 0; i < model->nfaces(); i++) {
    std::vector<int> face = model->face(i);
    Vec3f pts[3];
    for (int i = 0; i < 3; i++) pts[i] = renderer->world2screen(model->vert(face[i]));
    renderer->triangle(pts, zbuffer, image, TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
  }

  image.flip_vertically();  // i want to have the origin at the left bottom corner of the image
  image.write_tga_file("output.tga");
  delete model;
  return 0;
}
