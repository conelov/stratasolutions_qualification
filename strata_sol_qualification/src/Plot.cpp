//
// Created by dym on 07.05.24.
//

#include <strata_sol_qualification/Plot.hpp>

#include <strata_sol_qualification/utils.hpp>


namespace ssq {


Plot::~Plot() = default;


Plot::Plot(QWidget* parent)
    : QCustomPlot{parent} {
}


void Plot::changes(RunnerMessageVariant msg) {
  std::visit(
    overloaded{
      [this](RunnerMessage::DataCargo const& msg) {
        if (!graph_) {
          graph_ = addGraph();
          graph_->setLineStyle(QCPGraph::lsNone);
          graph_->setScatterStyle(QCPScatterStyle{QCPScatterStyle::ssCircle, 5});
        }
        for (auto const [x, y] : msg.data) {
          graph_->addData(x, y);
        }
        graph_->rescaleAxes();
        replot();
      },

      [](RunnerMessage::Initial const& msg) {

      },
    },
    msg);
}


}// namespace ssq
