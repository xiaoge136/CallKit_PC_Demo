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
			while (loop--) {
				std::unique_lock<std::mutex> lock(mutex);
				std::cv_status status = cond.wait_for(lock, ms);
				if (std::cv_status::timeout == status) {
					task();
					loop = 0;
					break;
				}
				else {
					std::cout << "timer[" << name << "] wait break" << std::endl;
					loop = 0;
					break;
				}
			}
			expired = true;
			std::cout << "timer[" << name << "] thread finish" << std::endl;
		});
	}

	void stop() {
		if (expired == true) {
			std::cout << "timer[" << name << "] already stopped" << std::endl;
			return;
		}
		{
			std::cout << "timer[" << name << "] stop notify" << std::endl;
			std::unique_lock<std::mutex> lock{ mutex };
			cond.notify_one();
		}
		std::cout << "timer[" << name << "] stop finish" << std::endl;
	}
};
}

#endif // !_TIMER_H_