//
// Created by dym on 06.05.24.
//

#include <strata_sol_qualification/Runner.hpp>


namespace ssq {


struct Runner::Private final {
};


Runner::~Runner() = default;


Runner::Runner(QObject* parent)
    : QObject{parent}
    , p{new Private} {
}


void Runner::changes(std::shared_ptr<ControlBlockMessageVariant> control) {
}


}// namespace ssq
