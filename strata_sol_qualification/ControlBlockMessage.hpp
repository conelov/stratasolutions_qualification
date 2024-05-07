//
// Created by dym on 06.05.24.
//

#pragma once

#include <cstdint>
#include <variant>


namespace ssq {


struct ControlBlockMessage final {
  Q_GADGET
public:
  enum RunMode : std::uint8_t {
    Uniform,
  };
  Q_ENUM(RunMode)


  struct Run final {
    RunMode mode;
  };


  struct Stop final {
  };


  struct Pause final {
  };
};


using ControlBlockMessageVariant = std::variant<
  ControlBlockMessage::Run const,
  ControlBlockMessage::Stop const,
  ControlBlockMessage::Pause const>;


}// namespace ssq