#include "renderer.h"

#include <Eigen/Core>
#include <Eigen/Dense>
#include <cmath>
#include <cstdlib>

#include "tgaimage.h"

Renderer::Renderer(int w, int h) : width(w), height(h) {}

void Renderer::line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
  bool steep = false;
  if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
    std::swap(x0, y0);
    std::swap(x1, y1);
    steep = true;
  }
  if (x0 > x1) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }

  for (int x = x0; x <= x1; x++) {
    float t = (x - x0) / static_cast<float>(x1 - x0);
    int y = y0 * (1. - t) + y1 * t;
    if (steep) {
      image.set(y, x, color);
    } else {
      image.set(x, y, color);
    }
  }
}

void Renderer::triangle(Eigen::Vector3f *pts, Eigen::Vector2f *uvs, float *zbuffer,
                        TGAImage &image, TGAImage &texture, float intensity) {
  Eigen::Vector2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
  Eigen::Vector2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
  Eigen::Vector2f clamp(image.get_width() - 1, image.get_height() - 1);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 2; j++) {
      bboxmin[j] = std::max(0.f, std::min(bboxmin[j], pts[i][j]));
      bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
    }
  }

  Eigen::Vector3f P;
  for (P.x() = bboxmin.x(); P.x() <= bboxmax.x(); P.x()++) {
    for (P.y() = bboxmin.y(); P.y() <= bboxmax.y(); P.y()++) {
      Eigen::Vector3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
      if (bc_screen.x() < 0 || bc_screen.y() < 0 || bc_screen.z() < 0) continue;
      Eigen::Vector2f uv(0, 0);
      P.z() = 0;
      for (int i = 0; i < 3; i++) {
        P.z() += pts[i][2] * bc_screen[i];
        uv += uvs[i] * bc_screen[i];
      }
      if (zbuffer[static_cast<int>(P.x() + P.y() * width)] < P.z()) {
        TGAColor color = texture.get(static_cast<int>(uv.x() * texture.get_width()),
                                     static_cast<int>(uv.y() * texture.get_height()));
        zbuffer[static_cast<int>(P.x() + P.y() * width)] = P.z();
        image.set(static_cast<int>(P.x()), static_cast<int>(P.y()), color * intensity);
      }
    }
  }
}

Eigen::Vector3f Renderer::barycentric(Eigen::Vector3f A, Eigen::Vector3f B, Eigen::Vector3f C, Eigen::Vector3f P) {
  Eigen::Vector3f s[2];
  for (int i = 2; i--;) {
    s[i][0] = C[i] - A[i];
    s[i][1] = B[i] - A[i];
    s[i][2] = A[i] - P[i];
  }
  Eigen::Vector3f u = s[0].cross(s[1]);
  if (std::abs(u[2]) > 1e-2) {
    return Eigen::Vector3f(1.f - (u.x() + u.y()) / u.z(), u.y() / u.z(), u.x() / u.z());
  } else {
    return Eigen::Vector3f(-1, 1, 1);
  }
}

Eigen::Vector3f Renderer::world2screen(Eigen::Vector3f v) {
  return Eigen::Vector3f(static_cast<int>((v.x() + 1.) * width / 2. + .5),
                         static_cast<int>((v.y() + 1.) * height / 2. + .5), v.z());
}
