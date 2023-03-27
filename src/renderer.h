#ifndef RENDERER_H
#define RENDERER_H

#include "geometry.h"
#include "tgaimage.h"

class Renderer {
 public:
  Renderer(int w, int h); // constructor

  void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color);
  void triangle(Vec3f* pts, float* zbuffer, TGAImage& image, TGAColor color);
  Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P);
  Vec3f world2screen(Vec3f v);

 private:
  int width;
  int height;
};

#endif  // RENDERER_H
