//
// Created by dym on 06.05.24.
//

#include <strata_sol_qualification/MainWindow.hpp>

#include <boost/hana/for_each.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/unpack.hpp>

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
  namespace hana = boost::hana;
  hana::for_each(hana::make_tuple(
                   hana::make_pair(p->controlBlock, &ControlBlock::changes),
                   hana::make_pair(p->plot, &Plot::changes)),
    [this](auto&& pair) { hana::unpack(
                            std::forward<decltype(pair)>(pair), [this](auto rec, auto sig) {
                              QObject::connect(p->runner, &Runner::kick, rec, sig);
                            }); });
}


}// namespace ssq
