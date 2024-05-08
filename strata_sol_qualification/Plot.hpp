//
// Created by dym on 07.05.24.
//

#pragma once

#include <QPointer>

#include <qcustomplot.h>

#include <strata_sol_qualification/RunnerMessage.hpp>


class QCPGraph;


namespace ssq {


class Plot final : public QCustomPlot {
public:
  ~Plot() override;
  Plot(QWidget* parent = nullptr);

public slots:
  void changes(RunnerMessageVariant msg);

private:
  QPointer<QCPGraph> graph_;
};


}// namespace ssq
