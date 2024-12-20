/* Copyright 2013 Google Inc. All Rights Reserved.

   Distributed under MIT license.
   See file LICENSE for detail or copy at https://opensource.org/licenses/MIT
*/

/* Helpers common across multiple parts of woff2 */

#include <algorithm>
#include <span>

#include "./woff2_common.h"

#include "./port.h"

namespace woff2 {

uint32_t ComputeULongSum(std::span<const uint8_t> buf) {
  uint32_t checksum = 0;
  size_t aligned_size = buf.size() & ~3;
  for (size_t i = 0; i < aligned_size; i += 4) {
    checksum +=
        (buf[i] << 24) | (buf[i + 1] << 16) | (buf[i + 2] << 8) | buf[i + 3];
  }

  // treat size not aligned on 4 as if it were padded to 4 with 0's
  if (buf.size() != aligned_size) {
    uint32_t v = 0;
    for (size_t i = aligned_size; i < buf.size(); ++i) {
      v |= buf[i] << (24 - 8 * (i & 3));
    }
    checksum += v;
  }

  return checksum;
}

size_t CollectionHeaderSize(uint32_t header_version, uint32_t num_fonts) {
  size_t size = 0;
  if (header_version == 0x00020000) {
    size += 12;  // ulDsig{Tag,Length,Offset}
  }
  if (header_version == 0x00010000 || header_version == 0x00020000) {
    size += 12   // TTCTag, Version, numFonts
      + 4 * num_fonts;  // OffsetTable[numFonts]
  }
  return size;
}

} // namespace woff2
