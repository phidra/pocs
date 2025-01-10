#ifndef ROUTING_KIT_PROTOBUF_H
#define ROUTING_KIT_PROTOBUF_H

#include <cstdint>
#include <stdexcept>
#include <string>

namespace RoutingKit {

uint64_t decode_varint_as_uint64_and_advance_first_parameter(char const*& begin, char const* end);
int64_t zigzag_convert_uint64_to_int64(uint64_t x);

template <class VarIntCallback, class DoubleCallback, class StringCallback>
void decode_protobuf_message_with_callbacks(char const* begin,
                                            char const* end,
                                            VarIntCallback const& varint_callback,
                                            DoubleCallback const& double_callback,
                                            StringCallback const& string_callback) {
    while (begin != end) {
        uint64_t header = decode_varint_as_uint64_and_advance_first_parameter(begin, end);
        uint64_t field_id = header >> 3;
        uint64_t wire_type = header & ((1 << 3) - 1);
        switch (wire_type) {
            case 0:
                varint_callback(field_id, decode_varint_as_uint64_and_advance_first_parameter(begin, end));
                break;
            case 1:
                if (end - begin < 8)
                    throw std::runtime_error(
                        "Protobuf message is corrupt, the end of message was reached while parsing a 64-bit floating "
                        "point.");
                double_callback(field_id, *reinterpret_cast<double const*>(begin));
                begin += 8;
                break;
            case 5:
                if (end - begin < 4)
                    throw std::runtime_error(
                        "Protobuf message is corrupt, the end of message was reached while parsing a 32-bit floating "
                        "point.");
                double_callback(field_id, *reinterpret_cast<float const*>(begin));
                begin += 4;
                break;
            case 2: {
                uint64_t len = decode_varint_as_uint64_and_advance_first_parameter(begin, end);
                if ((uint64_t)(end - begin) < len)
                    throw std::runtime_error(
                        "Protobuf message is corrupt, the end of message was reached while parsing a string or "
                        "embedded message.");
                string_callback(field_id, begin, begin + len);
                begin += len;
                break;
            }
            default:
                throw std::runtime_error("Protobuf message contains unknown wire type " + std::to_string(wire_type) +
                                         ".");
        }
    }
}

}  // namespace RoutingKit

#endif
