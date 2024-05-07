//
// Created by dym on 06.05.24.
//

#include <strata_sol_qualification/Runner.hpp>

#include <functional>

#include <QBasicTimer>
#include <QPointer>
#include <QThread>
#include <QTimerEvent>

#include <range/v3/all.hpp>

#include <strata_sol_qualification/Application.hpp>
#include <strata_sol_qualification/utils.hpp>


namespace ssq {
namespace {
namespace _ {


class Worker final : public QObject {
  Q_OBJECT
public:
  Worker(QObject* parent = nullptr)
      : QObject{parent} {
  }


  Q_INVOKABLE void generate(ControlBlockMessage::Run msg, int interval = 100) {
    if (std::exchange(config_.mode, msg.mode) != msg.mode || !generator_) {
      switch (config_.mode) {
        case ControlBlockMessage::RunMode::Uniform:
          generator_ = [dist = std::uniform_int_distribution{0, 100}]() mutable -> double {
            return dist(Application::randSource());
          };
          break;
        default:
          assert(false);
      }
    }

    updater.start(interval, this);
  }

public slots:
  void stop() {
    updater.stop();
    data_.clear();
    generator_ = nullptr;
  }

signals:
  void dataStep(RunnerMessage::DataCargo::Data, QPrivateSignal) const;

protected:
  void timerEvent(QTimerEvent* event) override {
    if (event->timerId() == updater.timerId()) {
      generate_impl();
    } else {
      QObject::timerEvent(event);
    }
  }

private:
  void generate_impl() {
    constexpr std::size_t samples = 100;
    namespace rv                  = ranges::views;
    for (auto const pair : rv::generate_n([this] {
           return QPair{generator_(), generator_()};
         },
           samples)) {
      data_.push_back(pair);
    }
    emit dataStep(data_, {});
  }

private:
  QBasicTimer                    updater;
  RunnerMessage::DataCargo::Data data_;
  std::function<double()>        generator_;
  ControlBlockMessage::Run       config_;
};


class ThreadOwner final {
public:
  ~ThreadOwner() {
    if (running()) {
      stop();
    }
  }


  ThreadOwner(QObject& parent)
      : parent_{parent} {
  }


  void start() {
    assert(!running());
    th_ = new QThread{&parent_};
    w_ = new Worker;
    w_->moveToThread(th_);
    th_->start();
  }


  void stop() {
    assert(running());
    w_->deleteLater();
    th_->quit();
    th_->wait();
    th_->deleteLater();
  }


  Worker& worker() {
    assert(running());
    return *w_;
  }


  bool running() const {
    assert(!th_ == !w_);
    return th_;
  }

private:
  QObject&            parent_;
  QPointer<QThread>   th_;
  QPointer<_::Worker> w_;
};


}// namespace _
}// namespace


struct Runner::Private final {
  _::ThreadOwner th;
};


Runner::~Runner() = default;


Runner::Runner(QObject* parent)
    : QObject{parent}
    , p{new Private{
        .th{*this},
      }} {
}


void Runner::changes(ControlBlockMessageVariant control) {
  std::visit(
    overloaded{
      [this](ControlBlockMessage::Run const& msg) {
        p->th.start();
        auto& worker = p->th.worker();
        QObject::connect(&worker, &_::Worker::dataStep, this, [this](RunnerMessage::DataCargo::Data data) {
          emit kick(RunnerMessage::Running{std::move(data)}, {});
        });
        QMetaObject::invokeMethod(&worker, [&worker, msg] {
          worker.generate(msg);
        });
      },

      [](ControlBlockMessage::Pause const& msg) {

      },

      [this](ControlBlockMessage::Stop const& msg) {
        p->th.stop();
      },
    },
    control);
}


}// namespace ssq


#include "Runner.moc"
