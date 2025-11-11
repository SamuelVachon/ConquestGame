#include "LoggingObserver.h"
#include <fstream>
#include <algorithm>

Subject::Subject()
: observers_(new std::vector<Observer*>()) {}

Subject::Subject(const Subject& other)
: observers_(new std::vector<Observer*>(*other.observers_)) {}

Subject& Subject::operator=(const Subject& other) {
    if (this != &other) {
        *observers_ = *other.observers_;
    }
    return *this;
}

Subject::~Subject() {
    delete observers_;
    observers_ = nullptr;
}

void Subject::attach(Observer* o) {
    if (!o) return;
    observers_->push_back(o);
}

void Subject::detach(Observer* o) {
    if (!o) return;
    auto& v = *observers_;
    v.erase(std::remove(v.begin(), v.end(), o), v.end());
}

void Subject::notify() {
    // Copy to avoid issues if observers detach inside update
    auto copy = *observers_;
    for (Observer* o : copy) {
        if (o) o->update(this);
    }
}

LogObserver::LogObserver() {}
LogObserver::~LogObserver() {}

void LogObserver::update(Subject* s) {
    const auto* loggable = dynamic_cast<const ILoggable*>(s);
    if (!loggable) return;

    std::ofstream out("gamelog.txt", std::ios::app);
    if (!out.is_open()) return;

    out << loggable->stringToLog() << '\n';
}


