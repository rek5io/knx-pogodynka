#pragma once

#include <vector>
#include <span>
#include "result.hpp"
using namespace result;

namespace baos {
    inline static uint16_t be_u16_from_bytes(uint8_t b1, uint8_t b2) {
        return (uint16_t)b2 | ((uint16_t)b1 << 8);
    }

    class DecodeError {

    };

    class Frame {
        private:
            std::vector<uint8_t> data;
            Frame(std::vector<uint8_t> d) : data(d) {}

        public:
            static auto from_bytes(std::span<const uint8_t> raw) -> Result<Frame, DecodeError> {
                if (raw.size() < 7) {
                    return Result<Frame, DecodeError>::Err(DecodeError());
                }

                if (raw[0] != 0x68) {
                    return Result<Frame, DecodeError>::Err(DecodeError());
                }

                if (raw[1] != raw[2]) {
                    return Result<Frame, DecodeError>::Err(DecodeError());
                }

                size_t frame_len = (size_t)raw[1];

                if (raw.size() < (frame_len + 5)) {
                    return Result<Frame, DecodeError>::Err(DecodeError());
                }

                std::vector<uint8_t> data(raw.begin() + 5, raw.begin() + 5 + frame_len);

                return Result<Frame, DecodeError>::Ok(Frame(data));
            }

            auto get_data() -> std::span<const uint8_t> {
                return std::span<const uint8_t>(data);
            }
    };

    class Request {
        public:
            uint16_t start_id = 0;
            uint16_t length = 0;

            static auto decode(Frame frame) -> Result<Request, DecodeError> {
                std::span<const uint8_t> data = frame.get_data();

                if (data.size() < 6) {
                    return Result<Request, DecodeError>::Err(DecodeError());
                }

                if (data[0] != 0xf0) {
                    return Result<Request, DecodeError>::Err(DecodeError());
                }

                if (data[1] != 0x05) {
                    return Result<Request, DecodeError>::Err(DecodeError());
                }

                Request req;
                
                req.start_id = be_u16_from_bytes(data[2], data[3]);
                req.length = be_u16_from_bytes(data[4], data[5]);

                return Result<Request, DecodeError>::Ok(req);
            }

            static auto test() -> void {
                const uint8_t ft12_frame[] = {
                    0x68,
                    0x06, 0x06,
                    0x68,
                    0x40,
                    0xF0, 0x05, 0x00, 0x01, 0x00, 0x02,
                    0x38,
                    0x16
                };

                Frame frame = Frame::from_bytes(ft12_frame).unwrap();
                Request req = Request::decode(frame).unwrap();

                std::println("req: start: {} len: {}", req.start_id, req.length);
            }
    };

}
