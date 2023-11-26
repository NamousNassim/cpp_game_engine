#ifndef TIMER_HPP
#define TIMER_HPP

#include "GameObject.hpp"
#include <functional>

/**
 * @brief calls the given callback every given time
 *
 * if it is in RUNNING state, the callback will be called whenever the time set in "duration" has elapsed, then countdown starts again
 * if it is in RUNNING_SINGLE_ACTION state, the callback will be called once after the time set in "duration" has elapsed and then timer will turn off
 * you can stop the timer if it is in the RUNNING or RUNNING_SINGLE_ACTION state and then you can restart the timer, which will restore its state before it was stopped
 */
class Timer: public GameObject
{
public:
    enum class Status: uint8_t
    {
        OFF,
        RUNNING,
        RUNNGING_SINGLE_ACTION,
        STOPPED,
        STOPPED_SINGLE_ACTION
    };

public:
    Timer();
    /**
     * @brief constructs timer with given duration and callback
     * @param time period at which the callback will be called
     * @param callback that will be called
     */

    Timer(double duration, std::function<void()> callback);
    /**
    * @brief sets the time period at which the callback will be called
    * @param time period at which the callback will be called
    */
    void setDuration(double duration);

    /**
     * @brief sets the callback that will be called
     * @param callback that will be called
     */
    void setCallback(std::function<void()> callback);

    /**
     * @brief switch between using absolute and relative time
     */
     void setAbsoluteTime(bool absolute);

    /**
     * @brief starts the timer, the timer turns off only after calling the stop or reset function
     *
     * sets timer status to RUNNING
     */
    void start();

    /**
     * @brief starts the timer, the timer turns off after the first callback
     *
     * sets timer status to RUNNING_SINGLE_ACTION
     */
    void startSingleAction();

    /**
     * @brief stops the timer but remembers previous running timer status
     *
     * in case of RUNNING sets status to STOPPED
     * in case of RUNNING_SINGLE_ACTION sets status to STOPPED_SINGLE_ACTION
     * otherwise does nothing
     */
    void stop();

    /**
     * @brief restarts the timer and returns it to its state before it was stopped
     *
     * in case of STOPPED sets status to RUNNING
     * in case of STOPPED_SINGLE_ACTION sets status to RUNNING_SINGLE_ACTION
     * otherwise does nothing
     */
    void restart();

    /**
     * @brief sets timer state to OFF and reset time counter
     */
    void reset();

    /**
     * @brief getter for timer status
     * @return timer status
     */
    Status getStatus() const;

    /**
     * @brief getter for timer duration
     * @return timer duration
     */
    double duration() const;

    /**
     * @brief getter for timer callback
     * @return timer callback
     */
    std::function<void()> callback() const;

    /**
     * @brief function called every frame counts the time and calls a callback when it is due
     */
    void onFrame() final;

private:
    /// timer running status
    Status _status;

    /// callback that will be called after given time if timer is running
    std::function<void()> _callback;

    /// time period at which the callback will be called
    double _duration;

    /// the internal timer counts how much time has passed since the last callback
    double _timeCounter;

    /// is measuring absolute time
    double _isAbsoluteTime;
};

#endif // TIMER_HPP
