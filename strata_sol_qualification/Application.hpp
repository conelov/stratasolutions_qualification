//
// Created by dym on 07.05.24.
//

#pragma once

#include <random>

#include <QApplication>


namespace ssq {


class Application final : public QApplication {
  Q_OBJECT
public:
  template<typename T = std::mt19937>
  static T& randSource() {
    thread_local T rng = [] {
      std::random_device rd{};
      T                  gen{rd()};
      return std::move(gen);
    }();
    return rng;
  }

public:
  using QApplication::QApplication;
};


}// namespace ssq
