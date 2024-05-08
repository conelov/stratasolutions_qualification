//
// Created by dym on 06.05.24.
//

#include <strata_sol_qualification/Runner.hpp>

#include <functional>

#include <boost/preprocessor/stringize.hpp>

#include <QBasicTimer>
#include <QPointer>
#include <QThread>
#include <QTimerEvent>

#include <range/v3/view/generate_n.hpp>

#include <strata_sol_qualification/Application.hpp>
#include <strata_sol_qualification/utils.hpp>


namespace ssq {
namespace {
namespace _ {


struct Range final {
  int min;
  int max;
} constexpr gen_range{-1000, 1000};


class Worker final : public QObject {
  Q_OBJECT
public:
  Worker(QObject* parent = nullptr)
      : QObject{parent} {
  }


  Q_INVOKABLE void generate(ControlBlockMessage::Run msg, int interval = 150) {
    if (std::exchange(config_.mode, msg.mode) != msg.mode || !generator_) {
      switch (config_.mode) {
        case ControlBlockMessage::RunMode::Uniform:
          generator_ = [dist = std::uniform_int_distribution{gen_range.min, gen_range.max}]() mutable -> double {
            return dist(Application::randSource());
          };
          break;
        default:
          assert(false);
      }
    }

    updater.start(interval, this);
  }


  Q_INVOKABLE void pause() {
    updater.stop();
    generator_ = nullptr;
  }


  Q_INVOKABLE RunnerMessage::DataCargo::Data data() const {
    return data_;
  }

private:
  void stop() {
    pause();
    data_.clear();
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
    constexpr std::size_t samples = 2;
    namespace rv                  = ranges::views;
    for (auto const pair : rv::generate_n([this] {
           return std::pair{generator_(), generator_()};
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
    if (is_running()) {
      stop();
    }
  }


  ThreadOwner(QObject& parent)
      : parent_{parent} {
  }


  void start() {
    assert(!is_running());
    th_ = new QThread{&parent_};
    w_  = new Worker;
    w_->moveToThread(th_);
    th_->start();
  }


  void stop() {
    assert(is_running());
    w_->deleteLater();
    th_->quit();
    th_->wait();
    th_->deleteLater();
  }


  Worker& worker() {
    assert(is_running());
    return *w_;
  }


  bool is_running() const {
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

      [this](ControlBlockMessage::Pause const& msg) {
        auto& worker = p->th.worker();
        QMetaObject::invokeMethod(&worker, std::bind_front(&_::Worker::pause, &worker));
        RunnerMessage::DataCargo::Data data;
        QMetaObject::invokeMethod(&worker, [&data, &worker] { data = worker.data(); }, Qt::BlockingQueuedConnection);
        emit kick(RunnerMessage::Hold{std::move(data)}, {});
      },

      [this](ControlBlockMessage::Stop const& msg) {
        p->th.stop();
        emit kick(RunnerMessage::Initial{}, {});
      },
    },
    control);
}


}// namespace ssq


#include "Runner.moc"
