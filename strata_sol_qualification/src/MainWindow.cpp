//
// Created by dym on 06.05.24.
//

#include <strata_sol_qualification/MainWindow.hpp>

#include "ui_MainWindow.h"


namespace ssq {


struct MainWindow::Private final : public Ui::MainWindow {
};


MainWindow::~MainWindow() = default;


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}
    , p{new Private} {
  p->setupUi(this);

  setWindowTitle(QString{PROJECT_NAME} + " v" + PROJECT_VERSION);


}


}// namespace ssq
