#pragma once
#include <Windows.h>
#include <chrono>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

/// <summary>
/// Virtual base class for polymorphism.
/// </summary>
class RawMemory {
public:
	virtual std::string what() = 0;
protected:
	RawMemory() = default;
};

/// <summary>
/// Container for the memory that you want to monitor. 
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class Memory : public RawMemory {
public:
	Memory() = delete;
	Memory(T& ref, const char* description = "", const char* suffix = "");
	std::string what() override;
private:
	std::string m_description;
	T* m_p_value;
	std::string m_suffix;
};

/// <summary>
/// Class that creates a terminal and updates the monitored memory in a certain interval.
/// </summary>
class MemoryTracker {
public:
	template <typename ...Memory_T>
	MemoryTracker(unsigned int interval_milli, Memory_T& ...Memories);
	~MemoryTracker();
	void Stop();
private:
	void Update();
private:
	bool m_running = true;
	std::chrono::duration<unsigned int, std::milli> m_interval_milli;
	std::vector<RawMemory*> m_queue;
	std::thread m_thread;
	FILE* m_new_output = nullptr;
};

//**********************************************
// Template Implementation
//**********************************************
/// <summary> 
/// Constructor of Memory. It defines how the memory will be displayed in the terminal.
/// </summary>
template<typename T>
inline Memory<T>::Memory(T& ref, const char* description, const char* suffix) {
	m_description = description;
	m_p_value = &ref;
	m_suffix = suffix;
}
/// <summary>
/// Formats the string to be displayed in the terminal. 
/// </summary>
template<typename T>
inline std::string Memory<T>::what() {
	std::stringstream ss;
	ss << m_description << ": " << *m_p_value << m_suffix << "\n";
	return ss.str();
}
/// <summary>
/// Constructor of MemoryTracker. It creates a terminal in a thread and updates the monitored memory in a certain interval. 
/// </summary>
template<typename ...Memory_T>
inline MemoryTracker::MemoryTracker(unsigned int interval_milli, Memory_T& ...Memories) {
	(m_queue.push_back(&Memories), ...);
	m_interval_milli = std::chrono::duration<unsigned int, std::milli>(interval_milli);
	if (GetConsoleWindow() == NULL) {
		AllocConsole();
		freopen_s(&m_new_output, "CONOUT$", "w", stdout);
	}
	m_thread = std::thread(&MemoryTracker::Update, this);
}
