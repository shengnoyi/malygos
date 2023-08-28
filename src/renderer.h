#ifndef RENDERER_H
#define RENDERER_H

#include <Eigen/Core>
#include "tgaimage.h"

class Renderer {
public:
    Renderer(int w, int h);

    void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color);
    void triangle(Eigen::Vector3f* pts, Eigen::Vector2f* uvs, float* zbuffer, TGAImage& image, TGAImage& texture, float intensity);
    Eigen::Vector3f barycentric(Eigen::Vector3f A, Eigen::Vector3f B, Eigen::Vector3f C, Eigen::Vector3f P);
    Eigen::Vector3f world2screen(Eigen::Vector3f v);

private:
    int width;
    int height;
};

#endif  // RENDERER_H
