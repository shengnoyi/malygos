#include "renderer.h"

#include <Eigen/Core>
#include <Eigen/Dense>
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "tgaimage.h"

Renderer::Renderer(int w, int h) {
  width = w;
  height = h;
  // Initialize other member variables here if needed
  _view = Eigen::Matrix4f::Identity();  // Set the view matrix to the identity matrix
  _viewport << w / 2, 0, 0, (w - 1) / 2,
      0, h / 2, 0, (h - 1) / 2,
      0, 0, 1, 0,
      0, 0, 0, 1;
}

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

void Renderer::triangle(Eigen::Vector3f *pts, Eigen::Vector2f *uvs, TGAImage &zbuffer,
                        TGAImage &image, TGAImage &texture, float *intensities) {
  Eigen::Vector2i bboxmin(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
  Eigen::Vector2i bboxmax(-std::numeric_limits<int>::max(), -std::numeric_limits<int>::max());
  Eigen::Vector2i clamp(width - 1, height - 1);

  for (int i = 0; i < 3; i++) {
    bboxmin.x() = std::max(0, std::min(bboxmin.x(), static_cast<int>(pts[i].x())));
    bboxmin.y() = std::max(0, std::min(bboxmin.y(), static_cast<int>(pts[i].y())));

    bboxmax.x() = std::min(clamp.x(), std::max(bboxmax.x(), static_cast<int>(pts[i].x())));
    bboxmax.y() = std::min(clamp.y(), std::max(bboxmax.y(), static_cast<int>(pts[i].y())));
  }
  Eigen::Vector3f P;
  for (P.x() = bboxmin.x(); P.x() <= bboxmax.x(); P.x()++) {
    for (P.y() = bboxmin.y(); P.y() <= bboxmax.y(); P.y()++) {
      int x = P.x();
      int y = P.y();
      Eigen::Vector3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
      if (bc_screen.x() >= 0 && bc_screen.y() >= 0 && bc_screen.z() >= 0) {
        Eigen::Vector2f uv = uvs[0] * bc_screen.x() + uvs[1] * bc_screen.y() + uvs[2] * bc_screen.z();
        float intensity = intensities[0] * bc_screen.x() + intensities[1] * bc_screen.y() + intensities[2] * bc_screen.z();
        float z = pts[0].z() * bc_screen.x() + pts[1].z() * bc_screen.y() + pts[2].z() * bc_screen.z();

        // normalize `z` to [0, 2] -> cast to int -> clamp
        int frag_deepth = static_cast<int>(std::max(0.0f, std::min(1.0f, (z + 1.0f) * 0.5f)) * 255);
        if (zbuffer.get(x, y)[0] < frag_deepth) {
          TGAColor color = texture.get(int(uv.x() * texture.get_width()),
                                       int(uv.y() * texture.get_height()));
          zbuffer.set(x, y, TGAColor(frag_deepth));
          image.set(x, y, color * intensity);
        }
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
  Eigen::Vector4f m = (_viewport * _mvp * Eigen::Vector4f(v.x(), v.y(), v.z(), 1.0));

  return (m / m[3]).head(3);
}
Eigen::Matrix4f Renderer::lookAt(const Eigen::Vector3f &eye, const Eigen::Vector3f &center, const Eigen::Vector3f &up) {
  Eigen::Vector3f zAxis = (eye - center).normalized();
  Eigen::Vector3f xAxis = up.cross(zAxis).normalized();
  Eigen::Vector3f yAxis = zAxis.cross(xAxis);

  Eigen::Matrix4f translation;
  translation << 1, 0, 0, -eye.x(),
      0, 1, 0, -eye.y(),
      0, 0, 1, -eye.z(),
      0, 0, 0, 1;

  Eigen::Matrix4f rotation;
  rotation << xAxis.x(), xAxis.y(), xAxis.z(), 0,
      yAxis.x(), yAxis.y(), yAxis.z(), 0,
      zAxis.x(), zAxis.y(), zAxis.z(), 0,
      0, 0, 0, 1;

  return rotation * translation;
}

Eigen::Matrix4f Renderer::orthographic(float l, float r, float b, float t, float n, float f) {
  // 我们使用右手坐标系，所以 f 应该小于 n
  if (l < r && b < t && f < n) {
    Eigen::Matrix4f projectionMatrix = Eigen::Matrix4f::Identity();
    projectionMatrix << 2.0f / (r - l), 0.0f, 0.0f, -(r + l) / (r - l),
        0.0f, 2.0f / (t - b), 0.0f, -(t + b) / (t - b),
        0.0f, 0.0f, 2.0f / (n - f), -(n + f) / (n - f),
        0.0f, 0.0f, 0.0f, 1.0f;

    return projectionMatrix;
  } else {
    throw std::invalid_argument("Invalid orthographic projection parameters.");
  }
}
Eigen::Matrix4f Renderer::perspective(float l, float r, float b, float t, float n, float f) {
  Eigen::Matrix4f MOrth = orthographic(l, r, b, t, n, f);
  Eigen::Matrix4f MP = Eigen::Matrix4f::Identity();
  MP << n, 0, 0, 0,
      0, n, 0, 0,
      0, 0, n + f, -f * n,
      0, 0, 1, 0;
  return MOrth * MP;
}

void Renderer::setModel(const Eigen::Matrix4f &model) {
  _model = model;
}
void Renderer::setView(const Eigen::Matrix4f &view) {
  _view = view;
}
void Renderer::setProjection(const Eigen::Matrix4f &projection) {
  _projection = projection;
}
void Renderer::updateMvp() {
  _mvp = _projection * _view * _model;
}
