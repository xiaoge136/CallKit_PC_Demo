#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>
#include <chrono>
#include <thread>
#include <atomic>
#include <memory>
#include <mutex>
#include <condition_variable>

namespace necall_kit
{

class Timer {
private:
	std::string name;
	std::thread *thread = nullptr;
	std::mutex mutex;
	std::atomic<bool> expired;
	std::atomic<bool> stop_ = false;
	std::condition_variable cond;

public:
	explicit Timer(const std::string &timerName) : expired(true) {
		name = timerName;
	}

	~Timer() {
		stop();
	}

	void startTimer(int interval, int loop, std::function<void()> task) {

		if (expired == false) {
			std::cout << "timer[" << name << "] already started" << std::endl;
			return;
		}
		
		if (thread) {
			cond.notify_all();
			thread->join();
			delete thread;
			thread = nullptr;
		}

		expired = false;
		thread = new std::thread([this, interval, loop, task]() mutable {
			std::cout << "timer[" << name << "] thread start" << std::endl;

			const std::chrono::milliseconds &ms = std::chrono::milliseconds(interval);
			auto endTime = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch()) + ms;
			while (loop--) {
				std::unique_lock<std::mutex> lock(mutex);
				bool res = cond.wait_for(lock, ms, [this, endTime, ms] {
					auto curTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
					if (curTime >= endTime || stop_) {
						return true;
					}
					else {
						return false;
					}
				});

				if (stop_) {
					std::cout << "timer[" << name << "] wait break" << std::endl;
					loop = 0;
					break;
				}
				else {
					task();
					std::cout << "excute task" << std::endl;
					loop = 0;
					break;
				}
			}
			expired = true;
			stop_ = false;
			std::cout << "timer[" << name << "] thread finish" << std::endl;
		});
	}

	void stop() {
		if (expired == true) {
			std::cout << "timer[" << name << "] already stopped" << std::endl;
			return;
		}
		{
			stop_ = true;
			std::cout << "timer[" << name << "] stop notify" << std::endl;
			std::unique_lock<std::mutex> lock{ mutex };
			cond.notify_one();
		}
		std::cout << "timer[" << name << "] stop finish" << std::endl;
	}
};
}

#endif // !_TIMER_H_