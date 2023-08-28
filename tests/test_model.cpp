#include <gtest/gtest.h>

#include <Eigen/Core>
#include <Eigen/Dense>

#include "model.h"
#include "tgaimage.h"

Model *model = NULL;

// Demonstrate some basic assertions.
TEST(TestModel, BasicAssertions) {
  model = new Model("obj/african_head/african_head.obj");
  EXPECT_EQ(model->nfaces(), 2492);
  EXPECT_EQ(model->ntextureverts(), 1339);
  // f1
  std::vector<int> face = model->face(0);
  EXPECT_EQ(face[0] + 1, 24);
  EXPECT_EQ(face[1] + 1, 25);
  EXPECT_EQ(face[2] + 1, 26);
  EXPECT_FLOAT_EQ(model->vert(0).x(), -0.000581696);
  EXPECT_FLOAT_EQ(model->vert(1).x(), 0.000283538);
  EXPECT_FLOAT_EQ(model->vert(face[0]).x(), 0.134781);
  EXPECT_FLOAT_EQ(model->vert(face[0]).y(), -0.14723);
  EXPECT_FLOAT_EQ(model->vert(face[0]).z(), 0.48805);

  // f2
  std::vector<int> face2 = model->face(1);
  EXPECT_EQ(face2[0] + 1, 24);
  EXPECT_EQ(face2[1] + 1, 26);
  EXPECT_EQ(face2[2] + 1, 23);

  // face texture 1
  std::vector<int> face_texture = model->face_texture(0);
  EXPECT_EQ(face_texture[0] + 1, 1);
  EXPECT_EQ(face_texture[1] + 1, 2);
  EXPECT_EQ(face_texture[2] + 1, 3);
  EXPECT_FLOAT_EQ(model->texture_vert(0).x(), 0.532);
  EXPECT_FLOAT_EQ(model->texture_vert(face_texture[0]).x(), 0.532);
  EXPECT_FLOAT_EQ(model->texture_vert(face_texture[0]).y(), 0.923);

  // face texture 2
  std::vector<int> face2_texture = model->face_texture(1);
  EXPECT_EQ(face2_texture[0] + 1, 1);
  EXPECT_EQ(face2_texture[1] + 1, 3);
  EXPECT_EQ(face2_texture[2] + 1, 4);

  model->~Model();
}

TEST(TestModel, ReadTextureAssertions) {
  TGAImage image;
  image.read_tga_file("obj/african_head/african_head_diffuse.tga");
  model = new Model("obj/african_head/african_head.obj");

  EXPECT_EQ(image.get_width(), 1024);
  EXPECT_EQ(image.get_height(), 1024);

  std::vector<int> face_texture = model->face_texture(0);
  EXPECT_EQ(face_texture[0] + 1, 1);
  EXPECT_EQ(face_texture[1] + 1, 2);
  EXPECT_EQ(face_texture[2] + 1, 3);
  Eigen::Vector2f vt1 = model->texture_vert(face_texture[0]);
  EXPECT_FLOAT_EQ(vt1.x(), 0.532);
  EXPECT_FLOAT_EQ(vt1.y(), 0.923);

  TGAColor color = image.get(image.get_width() * vt1.x(), image.get_height() * vt1.y());
  // 在 TGAColor 中，constructor 的接口是 rgba, 但是内部以 bgra 存储的。
  TGAColor expected_color(90, 70, 59, 0);
  EXPECT_EQ(color[0], expected_color[0]);
  EXPECT_EQ(color[1], expected_color[1]);
  EXPECT_EQ(color[2], expected_color[2]);
  EXPECT_EQ(color[3], expected_color[3]);
}