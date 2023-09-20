#include <gtest/gtest.h>

#include <Eigen/Core>
#include <Eigen/Dense>

#include "renderer.h"

const int width = 800;
const int height = 800;
Renderer *renderer = new Renderer(width, height);

TEST(RendererTest, BarycentricInsideTriangle) {
  Eigen::Vector3f A(0.0f, 0.0f, 0.0f);
  Eigen::Vector3f B(1.0f, 0.0f, 0.0f);
  Eigen::Vector3f C(0.0f, 1.0f, 0.0f);
  Eigen::Vector3f P(0.2f, 0.2f, 0.0f);

  Eigen::Vector3f bary = renderer->barycentric(A, B, C, P);

  // Assert that the barycentric coordinates sum up to 1
  EXPECT_FLOAT_EQ(bary[0] + bary[1] + bary[2], 1.0f);

  // Assert that the point is inside the triangle
  EXPECT_FLOAT_EQ(bary[0], 0.6f);
  EXPECT_FLOAT_EQ(bary[1], 0.2f);
  EXPECT_FLOAT_EQ(bary[2], 0.2f);
}

TEST(RendererTest, BarycentricOutsideTriangle) {
  Eigen::Vector3f A(0.0f, 0.0f, 0.0f);
  Eigen::Vector3f B(1.0f, 0.0f, 0.0f);
  Eigen::Vector3f C(0.0f, 1.0f, 0.0f);
  Eigen::Vector3f P(0.5f, 0.6f, 0.0f);

  Eigen::Vector3f bary = renderer->barycentric(A, B, C, P);

  // Assert that the barycentric coordinates indicate the point is outside
  EXPECT_FLOAT_EQ(bary[0], -0.1f);
  EXPECT_FLOAT_EQ(bary[1], 0.5f);
  EXPECT_FLOAT_EQ(bary[2], 0.6f);
}