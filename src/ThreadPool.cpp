#include "ThreadPool.h"

using namespace cct;

void ThreadPool::task()
{
	while (!m_isStopped)
	{
		std::function<void()> theTask;
		{
			std::unique_lock lock(m_mutex);
			m_cv.wait(lock, [&]() { return m_isStopped || !m_tasks.empty(); });

			if (m_isStopped && m_tasks.empty())
				return;

			theTask = std::move(m_tasks.front());
			m_tasks.pop();
		}
		theTask();
	}
}

ThreadPool::ThreadPool(const unsigned int _threadCount)
	: m_threads(new std::thread[_threadCount])
	, m_isStopped(false)
	, m_threadCount(_threadCount)
{
	for (size_t i = 0; i < m_threadCount; ++i)
		m_threads[i] = std::thread([this]() { task(); });
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock lock(m_mutex);
		m_isStopped = true;
	}
	m_cv.notify_all();
	for (size_t i = 0; i < m_threadCount; ++i)
		if (m_threads[i].joinable())
			m_threads[i].join();
	delete[] m_threads;
}
