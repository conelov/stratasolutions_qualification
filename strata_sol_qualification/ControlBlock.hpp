//
// Created by dym on 06.05.24.
//

#pragma once

#include <QFrame>

#include <strata_sol_qualification/ControlBlockMessage.hpp>
#include <strata_sol_qualification/RunnerMessage.hpp>


namespace ssq {


class ControlBlock final : public QFrame {
  Q_OBJECT
public:
  ~ControlBlock() override;
  ControlBlock(QWidget* parent = nullptr);

public slots:
  void changes(RunnerMessageVariant msg);

signals:
  void kick(ControlBlockMessageVariant, QPrivateSignal) const;

private:
  struct Private;
  QScopedPointer<Private> const p;
};


}// namespace ssq
