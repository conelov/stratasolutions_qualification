//
// Created by dym on 06.05.24.
//

#pragma once

#include <variant>
#include <utility>

#include <QVector>


namespace ssq {


struct RunnerMessage final {
  struct Initial final {
  };


  struct DataCargo {
    using Data = QVector<std::pair<double, double>>;
    Data data;

    explicit DataCargo(auto&& data)
        : data{std::forward<decltype(data)>(data)} {
    }
  };


  struct Running final : public DataCargo {
    using DataCargo::DataCargo;
  };


  struct Hold final : public DataCargo {
    using DataCargo::DataCargo;
  };
};


using RunnerMessageVariant = std::variant<
  RunnerMessage::Initial const,
  RunnerMessage::Running const,
  RunnerMessage::Hold const>;


}// namespace ssq
