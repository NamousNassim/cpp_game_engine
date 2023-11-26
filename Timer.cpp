#include "Timer.hpp"
#include <stdexcept>

Timer::Timer()
    :GameObject(), _status(Status::OFF), _callback(nullptr), _duration(0), _timeCounter(0), _isAbsoluteTime(false)
{
    setActive(true);
}

Timer::Timer(double duration, std::function<void ()> callback)
    :GameObject(), _status(Status::OFF), _callback(callback), _duration(duration), _timeCounter(0), _isAbsoluteTime(false) { }

void Timer::setDuration(double duration)
{
    if (duration <= 0)
        throw std::invalid_argument("duration have to be positive");

    _duration = duration;
}

void Timer::setCallback(std::function<void ()> callback)
{
    _callback = callback;
}

void Timer::setAbsoluteTime(bool absolute)
{
    _isAbsoluteTime = absolute;
}

void Timer::start()
{
    _timeCounter = 0;
    _status = Status::RUNNING;
}

void Timer::startSingleAction()
{
    _timeCounter = 0;
    _status = Status::RUNNGING_SINGLE_ACTION;
}

void Timer::stop()
{
    if (_status == Status::RUNNING)
        _status = Status::STOPPED;
    else if (_status == Status::RUNNGING_SINGLE_ACTION)
        _status = Status::STOPPED_SINGLE_ACTION;
}

void Timer::restart()
{
    if (_status == Status::STOPPED)
        _status = Status::RUNNING;
    else if (_status == Status::STOPPED_SINGLE_ACTION)
        _status = Status::RUNNGING_SINGLE_ACTION;
}

void Timer::reset()
{
    _timeCounter = 0;
    _status = Status::OFF;
}

Timer::Status Timer::getStatus() const
{
    return _status;
}

double Timer::duration() const
{
    return _duration;
}

std::function<void ()> Timer::callback() const
{
    return _callback;
}

void Timer::onFrame()
{
    if (_status != Status::RUNNING && _status != Status::RUNNGING_SINGLE_ACTION)
        return;

    if (_isAbsoluteTime)
        _timeCounter += GameInfo::absoluteDeltaTime();
    else
        _timeCounter += GameInfo::deltaTime();

    while(_timeCounter >= _duration)
    {
        _callback();
        _timeCounter -= _duration;
        if (_status == Status::RUNNGING_SINGLE_ACTION)
            _status = Status::STOPPED;
    }
}

