#include "ThreadPool.h"

void ThreadPool::start(std::size_t num_threads)
{
	for (auto i = 0u; i < num_threads; ++i) {
		_threads.emplace_back(
				[=] {
					while (true) {
						Task task;
						{
							std::unique_lock<std::mutex> lock(_event_mutex);

							_event_var.wait(lock, [=] { return _stopping || !_tasks.empty(); });

							if (_stopping && _tasks.empty())
								break;

							task = std::move(_tasks.front());
							_tasks.pop();
						}

						task();
					}
				}
		);
	}
}

void ThreadPool::stop()
{
	{
		std::unique_lock<std::mutex> lock(_event_mutex);
		_stopping = true;
	}

	_event_var.notify_all();

	for (auto& thread: _threads)
		thread.join();
}