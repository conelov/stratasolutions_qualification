//
// Created by dym on 06.05.24.
//

#pragma once

#include <variant>

#include <QMap>


namespace ssq {


struct RunnerMessage final {
  struct Initial final {
  };


  struct DataCargo {
    using Data = QMap<double, double>;
    Data data;
  };


  struct Running final : public DataCargo {
  };


  struct Hold final : public DataCargo {
  };
};


using RunnerMessageVariant = std::variant<
  RunnerMessage::Initial,
  RunnerMessage::Running,
  RunnerMessage::Hold>;


}// namespace ssq
