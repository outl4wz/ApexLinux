#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <sys/uio.h>
#include <unistd.h>
#include <vector>
#include <cstring>

pid_t _processid;

namespace Process
{
    int GetProcessId()
    {
        std::string command = "pidof R5Apex.exe";
        FILE* fp = popen(command.c_str(), "r");
        if (!fp) {
            std::cerr << "[!] Failed to execute command" << "\n";
            return 0;
        }

        char output[128];
        if (fgets(output, sizeof(output), fp) == NULL) {
            std::cerr << "[!] Failed to read output" << "\n";
            return 0;
        }
        int pid = std::stoi(output);

        _processid = pid;

        return pid;
    }

    bool ReadRaw(unsigned long address, void* buffer, size_t size) {
        if(size <= 0)
            return false;

        void* addressPtr = (void*)address;

        pid_t processId = _processid;

        struct iovec iovLocalAddressSpace[1]{0};
        struct iovec iovRemoteAddressSpace[1]{0};

        iovLocalAddressSpace[0].iov_base = buffer;
        iovLocalAddressSpace[0].iov_len = size;

        iovRemoteAddressSpace[0].iov_base = addressPtr;
        iovRemoteAddressSpace[0].iov_len = size;

        ssize_t returnSize = process_vm_readv(
            processId,
            iovLocalAddressSpace,
            1,
            iovRemoteAddressSpace,
            1,
            0
        );

        if(returnSize == (ssize_t)size) {
            return true;
        }

        return false;
    }

    template<typename T>
    T Read(unsigned long address) {
        T value{};
        bool success = ReadRaw(address, &value, sizeof(value));
        return success ? value : T();
    }

    void WriteRaw(size_t address, const void* data, size_t size) {
        struct iovec local[1];
        struct iovec remote[1];
        local[0].iov_base = const_cast<void*>(data);
        local[0].iov_len = size;
        remote[0].iov_base = reinterpret_cast<void*>(address);
        remote[0].iov_len = size;

        ssize_t number_of_bytes_written = process_vm_writev(_processid, local, 1, remote, 1, 0);
    }

    template <typename T>
    void Write(uintptr_t address, const T& value) {
        WriteRaw(address, reinterpret_cast<const void*>(&value), sizeof(value));
    }

    uintptr_t GetModuleBaseAddress(const std::string& moduleName) {
        int pid = _processid;
        std::stringstream ss;
        ss << "/proc/" << pid << "/maps";
        std::ifstream ifs(ss.str());
        if (!ifs) {
            std::cerr << "[!] Failed to open " << ss.str() << "\n";
            return 0;
        }
        // std::cout << "[+] Successfully opened " << ss.str() << "\n";

        uintptr_t baseAddress = 0;
        std::string line;
        while (std::getline(ifs, line)) {
            size_t pos = line.find(moduleName);
            if (pos != std::string::npos) {
                size_t hyphenPos = line.find_first_of('-');
                if (hyphenPos != std::string::npos) {
                    std::stringstream converter(line.substr(0, hyphenPos));
                    converter >> std::hex >> baseAddress;
                    break;
                }
            }
        }
        ifs.close();
        return baseAddress;
    }
}
