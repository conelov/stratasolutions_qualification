//
// Created by dym on 06.05.24.
//

#include <strata_sol_qualification/ControlBlock.hpp>

#include <boost/hana/for_each.hpp>
#include <boost/hana/tuple.hpp>

#include <strata_sol_qualification/utils.hpp>

#include "ui_ControlBlock.h"


namespace ssq {


struct ControlBlock::Private final : public Ui::ControlBlock {
};


ControlBlock::~ControlBlock() = default;


ControlBlock::ControlBlock(QWidget* parent)
    : QFrame{parent}
    , p{new Private} {
  p->setupUi(this);

  for (auto const mod : enum_names<ControlBlockMessage::RunMode>()) {
    p->cb_mode->addItem(mod);
  }

  namespace hana = boost::hana;
  hana::for_each(
    hana::make_tuple(
      hana::pair{p->pb_start, [this] {
                   return ControlBlockMessage::Run{
                     .mode = static_cast<ControlBlockMessage::RunMode>(p->cb_mode->currentIndex()),
                   };
                 }},
      hana::pair{p->pb_pause, [] {
                   return ControlBlockMessage::Pause{};
                 }},
      hana::pair{p->pb_stop, [] {
                   return ControlBlockMessage::Stop{};
                 }}),
    [this](auto&& pair) {
      hana::unpack(std::forward<decltype(pair)>(pair), [this](auto pb, auto msg_getter) {
        QObject::connect(pb, &QPushButton::released, this, [this, msg_getter] {
          emit kick(std::make_shared<ControlBlockMessageVariant>(msg_getter()), {});
        });
      });
    });
}


void ControlBlock::changes(std::shared_ptr<RunnerMessageVariant>) {
}


}// namespace ssq
