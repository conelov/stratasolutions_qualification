//
// Created by dym on 06.05.24.
//

#pragma once

#include <QObject>
#include <QScopedPointer>

#include <strata_sol_qualification/ControlBlockMessage.hpp>
#include <strata_sol_qualification/RunnerMessage.hpp>


namespace ssq {


class ControlBlock;


class Runner final : public QObject {
  Q_OBJECT
public:
  ~Runner();
  Runner(QObject* parent = nullptr);

public slots:
  void changes(ControlBlockMessageVariant control);

signals:
  void kick(RunnerMessageVariant, QPrivateSignal) const;

private:
  struct Private;
  QScopedPointer<Private> p;
};


}// namespace ssq
