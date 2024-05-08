//
// Created by dym on 06.05.24.
//

#include <strata_sol_qualification/MainWindow.hpp>

#include <strata_sol_qualification/Runner.hpp>

#include "ui_MainWindow.h"


namespace ssq {


struct MainWindow::Private final : public Ui::MainWindow {
  Runner* const runner;
};


MainWindow::~MainWindow() = default;


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}
    , p{new Private{
        .runner = new Runner{this}}} {
  p->setupUi(this);

  setWindowTitle(QString{PROJECT_NAME} + " v" + PROJECT_VERSION);

  QObject::connect(p->controlBlock, &ControlBlock::kick, p->runner, &Runner::changes);
  QObject::connect(p->runner, &Runner::kick, p->plot, &Plot::changes);
}


}// namespace ssq
