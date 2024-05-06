#include <QApplication>

#include <strata_sol_qualification/MainWindow.hpp>


int main(int argc, char* argv[]) {
  QApplication app{argc, argv};

  ssq::MainWindow root;
  root.show();

  return QApplication::exec();
}
