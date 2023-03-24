#include <gtest/gtest.h>

#include "model.h"

Model *model = NULL;

// Demonstrate some basic assertions.
TEST(TestModel, BasicAssertions) {
  model = new Model("obj/african_head/african_head.obj");
  EXPECT_EQ(model->nfaces(), 2492);
}