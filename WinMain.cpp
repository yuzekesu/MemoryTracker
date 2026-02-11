#include "MemoryTracker.h"
#include <Windows.h>
#include <thread>


int WINAPI WinMain(_In_ HINSTANCE, _In_ HINSTANCE, _In_opt_ LPSTR, _In_ int) {

	// Fill the info we want to monitor.
	const char* value1 = "You can manuelly stop it with member function Stop(), or it automatically destroys itself in destructor";
	Memory<const char*> mem1(value1);
	MemoryTracker t1(300, mem1);
	std::this_thread::sleep_for(std::chrono::seconds(4));


	// Terminate the terminal.
	t1.Stop();

	// Re-open the terminal.
	float value2 = 1.5f;
	int value3 = 25;
	char value4 = 'A';
	char value5_buffer = 'A'; // 65
	unsigned value5 = value5_buffer;
	const char* value6 = "I'm Bob";
	std::string value7 = "error";
	Memory<float> mem2(value2, "float");
	Memory<int> mem3(value3, "int");
	Memory<char> mem4(value4, "char");
	Memory<unsigned> mem5(value5, "char to unsigned");
	Memory<const char*> mem6(value6, "const char*");
	Memory<std::string> mem7(value7, "std::string");
	MemoryTracker t2(300, mem2, mem3, mem4, mem5, mem6, mem7);
	std::this_thread::sleep_for(std::chrono::seconds(4));
	return 0;
}

