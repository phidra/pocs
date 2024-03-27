#ifndef BIT_SELECT_H
#define BIT_SELECT_H

#include <stdint.h>

namespace RoutingKit {

// returns the 0-based offset of the (n+1)-th bit set in data

uint32_t uint64_bit_select(uint64_t data, uint32_t n);
uint32_t uint512_bit_select(uint64_t const* data, uint32_t n);
uint64_t bit_select(uint64_t uint512_count, uint64_t const* uint512_rank, uint64_t const* data, uint64_t n);
uint64_t skip_empty_uint512(uint64_t& uint512_count, uint64_t const*& uint512_rank, uint64_t const*& data);

}  // namespace RoutingKit

#endif
