#include "MemoryTracker.h"
#include <Windows.h>
#include <iostream>
#include <thread>

MemoryTracker::~MemoryTracker() {
	Stop();
}

void MemoryTracker::Stop() {
	if (m_thread.joinable()) {
		m_running = false;
		m_thread.join();
		if (m_new_output != nullptr) {
			fclose(m_new_output);
			FreeConsole();
		}
	}
}

void MemoryTracker::Update() {
	while (m_running) {
		system("cls");
		for (RawMemory* p_info : m_queue) {
			std::cout << p_info->what();
		}
		std::this_thread::sleep_for(m_interval_milli);
	}
}
