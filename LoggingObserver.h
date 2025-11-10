#ifndef LOGGINGOBSERVER_H
#define LOGGINGOBSERVER_H

#include <string>
#include <vector>
#include <iosfwd>

class Subject; // fwd

// 1) ILoggable: any class that wants to be logged implements this
class ILoggable {
public:
    virtual ~ILoggable() = default;
    virtual std::string stringToLog() const = 0;
};

// 2) Observer: base observer
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(Subject* s) = 0;
};

// 3) Subject: keeps a list of observers and notifies them
class Subject {
public:
    Subject();
    Subject(const Subject& other);
    Subject& operator=(const Subject& other);
    virtual ~Subject();

    void attach(Observer* o);
    void detach(Observer* o);
protected:
    void notify();   // subclasses call this at the exact log points
private:
    std::vector<Observer*>* observers_; // pointer per A2 rule
};

// 4) LogObserver: writes to gamelog.txt whenever update() is called
class LogObserver : public Observer {
public:
    LogObserver();
    ~LogObserver() override;
    void update(Subject* s) override; // appends s->stringToLog()
};

#endif // LOGGINGOBSERVER_H
