#ifndef GENERATOR_THREADPOOL_H
#define GENERATOR_THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

/**
 * Пул потоков
 * В нем запускаются процессы по работе .gmi файлами
 */
class ThreadPool
{
public:
	using Task = std::function<void()>;

	explicit ThreadPool(std::size_t num_threads)
	{
		start(num_threads);
	}

	~ThreadPool()
	{
		stop();
	}

	template<typename T>
	auto enqueue(T task) -> std::future<decltype(task())>;

private:
	std::vector<std::thread> _threads;

	std::condition_variable _event_var;

	std::mutex _event_mutex;
	bool _stopping = false;

	std::queue<Task> _tasks;

	void start(std::size_t num_threads);

	void stop();
};

template<typename T>
auto ThreadPool::enqueue(T task) -> std::future<decltype(task())>
{
	auto wrapper = std::make_shared<std::packaged_task<decltype(task())() >>(std::move(task));
	{
		std::unique_lock<std::mutex> lock{ _event_mutex };
		_tasks.emplace([=]
		{
			(*wrapper)();
		});
	}

	_event_var.notify_one();
	return wrapper->get_future();
}

#endif //GENERATOR_THREADPOOL_H
