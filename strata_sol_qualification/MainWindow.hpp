//
// Created by dym on 06.05.24.
//

#pragma once

#include <QMainWindow>


namespace ssq {


class MainWindow final : public QMainWindow {
  Q_OBJECT
public:
  ~MainWindow() override;
  MainWindow(QWidget* parent = nullptr);

private:
  struct Private;
  QScopedPointer<Private> const p;
};


}// namespace ssq
