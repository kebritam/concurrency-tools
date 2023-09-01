#pragma once

#include <condition_variable>
#include <queue>
#include <functional>
#include <future>

namespace cct
{
	class ThreadPool
	{
		std::thread* m_threads;
		std::condition_variable m_cv;
		mutable std::mutex m_mutex;
		std::queue<std::function<void()>> m_tasks;
		bool m_isStopped;
		size_t m_threadCount;

		void task();

	public:
		ThreadPool(const ThreadPool& _other) = delete;
		ThreadPool(ThreadPool&& _other) noexcept = delete;
		ThreadPool& operator=(const ThreadPool& _other) = delete;
		ThreadPool& operator=(ThreadPool&& _other) noexcept = delete;

		explicit ThreadPool(unsigned int _threadCount);

		~ThreadPool();

		template<typename F, typename... Args, typename R = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>
		std::future<R> Enqueue(F&& _func, Args... _args)
		{
			auto packagedTask = std::make_shared<std::packaged_task<R()>>(std::bind(std::forward<F>(_func), std::forward<Args>(_args)...));
			auto res = packagedTask->get_future();
			{
				std::unique_lock lock(m_mutex);
				m_tasks.emplace([packagedTask]()
				{
					(*packagedTask)();
				});
			}
			m_cv.notify_one();
			return res;
		}
	};
}
