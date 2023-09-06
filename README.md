# Concurrency Tools
> Simple Concurrency tools written with C++

This project aims to implement multiple concurrency tools such as thread pools and task queues.

## Installation

The simplest way to use is to just transfer the header and source files to your project and start using them. Or you can build them into a library.

## Usage example
```c++
cct::ThreadPool pool(2);
std::future<int> fut1 = pool.Enqueue([]() 
	{
		std::this_thread::sleep_for(std::chrono::seconds(3)); 
		std::cout << "nubmer1\n"; return 1; 
	});
std::future<std::string> fut2 = pool.Enqueue([]() 
	{
		std::this_thread::sleep_for(std::chrono::seconds(3)); 
		std::cout << "nubmer2\n"; return std::string("2"); 
	});
std::future<void> fut3 = pool.Enqueue([]() 
	{
		std::this_thread::sleep_for(std::chrono::seconds(3)); 
		std::cout << "nubmer3\n"; 
	});
pool.Submit([]() 
	{
		std::this_thread::sleep_for(std::chrono::seconds(3)); 
		std::cout << "nubmer4\n"; 
	});

int res1 = fut1.get();
std::string res2 = fut2.get();
fut3.get();
```

## Contributing

1. Fork it (<https://github.com/kebritam/concurrency-tools>)
2. Create your feature branch (`git checkout -b feature/featureName`)
3. Commit your changes (`git commit -am 'Add your commit message'`)
4. Push to the branch (`git push origin feature/featureName`)
5. Create a new Pull Request
