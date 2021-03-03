#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

#include "task.h"

class ThreadPool {
public:
   ThreadPool(unsigned thread_num=4, unsigned connection_buffer_len=100);
	bool connection_buffer_full() {
		std::lock_guard<std::mutex> lk(mu_);
		return max_connection_buffer_len_ == connection_buffer_.size();
	}

	void insert_conn(int conn_fd) {
		std::unique_lock<std::mutex> lk(mu_);
		while (!connection_buffer_full()) {
			cv_.wait(lk);
		}
		std::cout << "Inserting" << std::endl;
		connection_buffer_.push(conn_fd);
		cv_.notify_one();
	}
private:
   unsigned int max_connection_buffer_len_;
   std::queue<int> connection_buffer_;
	std::mutex mu_;
	std::condition_variable cv_;
   std::vector<std::thread> thread_buffer_;
};
