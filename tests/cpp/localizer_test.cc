#include <gtest/gtest.h>
#include "./utils.h"
#include "data/batch_iter.h"
#include "difacto/base.h"
#include <common/localizer.h>

using namespace difacto;

TEST(Localizer, Base) {
  BatchIter iter("../tests/data", "libsvm", 0, 1, 100);
  CHECK(iter.Next());
  dmlc::data::RowBlockContainer<unsigned> compact;
  std::vector<feaid_t> uidx;
  std::vector<real_t> freq;

  Localizer lc;
  lc.Compact(iter.Value(), &compact, &uidx, &freq);
  auto batch = compact.GetBlock();
  int size = batch.size;

  for (auto& i : uidx) i = ReverseBytes(i);

  EXPECT_EQ(norm1(uidx.data(), uidx.size()), 65111856);
  EXPECT_EQ(norm1(freq.data(), freq.size()), 9648);
  EXPECT_EQ(norm1(iter.Value().offset, size+1),
            norm1(batch.offset, size+1));
  EXPECT_EQ(norm2(iter.Value().value, batch.offset[size]),
            norm2(batch.value, batch.offset[size]));
}

TEST(Localizer, BaseHash) {
  BatchIter iter("../tests/data", "libsvm", 0, 1, 100);
  CHECK(iter.Next());
  dmlc::data::RowBlockContainer<unsigned> compact;
  std::vector<feaid_t> uidx;
  std::vector<real_t> freq;

  Localizer lc(1000);
  lc.Compact(iter.Value(), &compact, &uidx, &freq);
  auto batch = compact.GetBlock();
  int size = batch.size;

  for (auto& i : uidx) i = ReverseBytes(i);

  EXPECT_EQ(norm1(uidx.data(), uidx.size()), 478817);
  EXPECT_EQ(norm1(freq.data(), freq.size()), 9648);
  EXPECT_EQ(norm1(iter.Value().offset, size+1),
            norm1(batch.offset, size+1));
  EXPECT_EQ(norm2(iter.Value().value, batch.offset[size]),
            norm2(batch.value, batch.offset[size]));
}
