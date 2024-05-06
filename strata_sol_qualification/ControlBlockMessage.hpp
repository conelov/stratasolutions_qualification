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
  enum RunMode {
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
  ControlBlockMessage::Run,
  ControlBlockMessage::Stop,
  ControlBlockMessage::Pause>;


}// namespace ssq