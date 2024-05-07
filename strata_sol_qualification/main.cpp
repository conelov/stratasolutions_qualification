#include <boost/preprocessor/stringize.hpp>

#include <QMetaType>

#include <strata_sol_qualification/Application.hpp>
#include <strata_sol_qualification/MainWindow.hpp>
#include <strata_sol_qualification/RunnerMessage.hpp>


int main(int argc, char* argv[]) {
  qRegisterMetaType<ssq::RunnerMessage::DataCargo::Data>(BOOST_PP_STRINGIZE(RunnerMessage::DataCargo::Data));

  ssq::Application app{argc, argv};

  ssq::MainWindow root;
  root.show();

  return ssq::Application::exec();
}
