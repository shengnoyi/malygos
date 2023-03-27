#include <gtest/gtest.h>

#include "geometry.h"

TEST(TestGeometry, CrossProductAssertions) {
  Vec3f a(2, 3, 4);
  EXPECT_EQ(a.x, 2);
  EXPECT_EQ(a.y, 3);
  EXPECT_EQ(a.z, 4);

  Vec3f b(5, 6, 7);
  EXPECT_EQ(b.x, 5);
  EXPECT_EQ(b.y, 6);
  EXPECT_EQ(b.z, 7);

  Vec3f p = a ^ b;
  EXPECT_EQ(p.x, -3);
  EXPECT_EQ(p.y, 6);
  EXPECT_EQ(p.z, -3);

  Vec3f i1(1, 0, 0);
  Vec3f i2(0, 1, 0);
  Vec3f i3 = i1 ^ i2;
  EXPECT_EQ(i3.x, 0);
  EXPECT_EQ(i3.y, 0);
  EXPECT_EQ(i3.z, 1);
}