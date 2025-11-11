#ifndef LOGGINGOBSERVER_H
#define LOGGINGOBSERVER_H

#include <string>
#include <vector>
#include <iosfwd>
#include <fstream>    // for std::ofstream
#include <algorithm>  // for std::remove in detach

class Subject;   // forward declare

// ILoggable: classes that can produce a log line
class ILoggable {
public:
    virtual ~ILoggable() = default;
    virtual std::string stringToLog() const = 0;
};

// Observer: base observer
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(Subject* s) = 0;
};

// Subject: owns a list of observers and notifies them
class Subject {
public:
    Subject();
    Subject(const Subject& other);
    Subject& operator=(const Subject& other);
    virtual ~Subject();

    void attach(Observer* o);
    void detach(Observer* o);
protected:
    void notify();   // subclasses call this at log points
private:
    std::vector<Observer*>* observers_; // pointer per A2 rule
};

// LogObserver: appends stringToLog() to gamelog.txt on update
class LogObserver : public Observer {
public:
     LogObserver();
    ~LogObserver() override;
    void update(Subject* s) override; // appends s->stringToLog()


};

#endif
