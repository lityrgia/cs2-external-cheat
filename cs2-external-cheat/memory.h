#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>

#include <cstdint>
#include <vector>

class Memory
{
public:
	std::uintptr_t processId = 0;
	void* processHandle = nullptr;

	// Constructor that finds the process id
	// and opens a handle
	Memory() {}

	// Destructor that frees the opened handle
	~Memory()
	{
		if (processHandle)
			::CloseHandle(processHandle);
	}

	const void attachProcess(const wchar_t* processName) noexcept {
		::PROCESSENTRY32 entry = { };
		entry.dwSize = sizeof(::PROCESSENTRY32);

		const auto snapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		while (::Process32Next(snapShot, &entry))
		{
			if (!_wcsicmp(entry.szExeFile, processName))
			{
				processId = entry.th32ProcessID;
				processHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
				break;
			}
		}

		// Free handle
		if (snapShot)
			::CloseHandle(snapShot);
	}

	// Returns the base address of a module by name
	const std::uintptr_t GetModuleAddress(const wchar_t* moduleName) const noexcept
	{
		::MODULEENTRY32 entry = { };
		entry.dwSize = sizeof(::MODULEENTRY32);

		const auto snapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);

		std::uintptr_t result = 0;

		while (::Module32Next(snapShot, &entry))
		{
			if (!_wcsicmp(entry.szModule, moduleName))
			{
				result = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
				break;
			}
		}

		if (snapShot)
			::CloseHandle(snapShot);

		return result;
	}

	const HMODULE GetModuleAddressHandle(const wchar_t* moduleName) const noexcept
	{
		::MODULEENTRY32 entry = { };
		entry.dwSize = sizeof(::MODULEENTRY32);

		const auto snapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);

		HMODULE result = nullptr;

		while (::Module32Next(snapShot, &entry))
		{
			if (!_wcsicmp(entry.szModule, moduleName))
			{
				result = entry.hModule;;
				break;
			}
		}

		if (snapShot)
			::CloseHandle(snapShot);

		return result;
	}

	const std::uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets) const noexcept
	{
		uintptr_t address = ptr;

		for (unsigned int i = 0; i < offsets.size(); ++i) {
			BOOL rpmReturn = ReadProcessMemory(processHandle, (LPVOID)address, &address, sizeof(address), 0);

			address += offsets[i];
		}

		return address;
	}

	void ReadRaw(const std::uintptr_t& address, char* buffer, size_t size) const noexcept
	{
		::ReadProcessMemory(processHandle, reinterpret_cast<const void*>(address), buffer, size, NULL);
	}

	template <typename T>
	constexpr const T Read(const std::uintptr_t& address) const noexcept
	{
		T value = { };
		::ReadProcessMemory(processHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
		return value;
	}

	template <typename T>
	constexpr void Write(const std::uintptr_t& address, const T& value) const noexcept
	{
		::WriteProcessMemory(processHandle, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
	}
};

inline Memory mem; 