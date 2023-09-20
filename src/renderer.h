#ifndef RENDERER_H
#define RENDERER_H

#include <Eigen/Core>
#include <Eigen/Geometry>  // Include for transformations

#include "tgaimage.h"

class Renderer {
 public:
  Renderer(int w, int h);

  void setModel(const Eigen::Matrix4f& model);
  void setView(const Eigen::Matrix4f& model);
  void updateMvp();
  void setProjection(const Eigen::Matrix4f& projection);
  void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color);
  void triangle(Eigen::Vector3f* pts, Eigen::Vector2f* uvs, float* zbuffer, TGAImage& image, TGAImage& texture, float intensity);

  Eigen::Matrix4f lookAt(const Eigen::Vector3f& eye, const Eigen::Vector3f& center, const Eigen::Vector3f& up);
  Eigen::Vector3f barycentric(Eigen::Vector3f A, Eigen::Vector3f B, Eigen::Vector3f C, Eigen::Vector3f P);
  Eigen::Vector3f world2screen(Eigen::Vector3f v);
  Eigen::Matrix4f orthographic(float left, float right, float bottom, float top, float near, float far);
  Eigen::Matrix4f perspective(float left, float right, float bottom, float top, float near, float far);

 private:
  int width;
  int height;

  Eigen::Matrix4f _model;
  Eigen::Matrix4f _view;
  Eigen::Matrix4f _projection;
  Eigen::Matrix4f _mvp;
  Eigen::Matrix4f _viewport;
};

#endif  // RENDERER_H
