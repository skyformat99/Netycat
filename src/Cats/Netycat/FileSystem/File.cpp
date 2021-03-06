/*
 *
 * MIT License
 *
 * Copyright (c) 2016-2018 The Cats Project
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "Cats/Netycat/FileSystem/File.hpp"

#include <stdexcept>


namespace Cats {
namespace Netycat {
inline namespace FileSystem {

File::File(String8 path, Mode mode) {
    
    DWORD access;
    switch(mode & Mode::READ_WRITE) {
    case Mode::READ: access = GENERIC_READ, readable = true; break;
    case Mode::WRITE: access = GENERIC_WRITE, writable = true; break;
    case Mode::READ_WRITE: access = GENERIC_READ | GENERIC_WRITE, readable = true, writable = true; break;
    default: throw std::invalid_argument("Invalid mode");
    }
    
    DWORD share = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
    
    DWORD disposition;
    switch(mode & Mode::CREATE_TRUNCATE_EXCLUDE) {
    case Mode::NONE:
    case Mode::EXCLUDE: disposition = OPEN_EXISTING; break;
    case Mode::CREATE: disposition = OPEN_ALWAYS; break;
    case Mode::TRUNCATE:
    case Mode::TRUNCATE_EXCLUDE: disposition = TRUNCATE_EXISTING; break;
    case Mode::CREATE_TRUNCATE: disposition = CREATE_ALWAYS; break;
    case Mode::CREATE_EXCLUDE:
    case Mode::CREATE_TRUNCATE_EXCLUDE: disposition = CREATE_NEW; break;
    default: throw std::invalid_argument("Invalid mode");
    }
    
    DWORD attribute = FILE_ATTRIBUTE_NORMAL;
    
    if(!(handle = CreateFileA(path.getData(), access, share, nullptr, disposition, attribute, nullptr))) {
        
        std::cout << GetLastError() << std::endl;
        throw std::runtime_error("CreateFileA failed");
        
    }
    
}
void File::read(Byte* buffer, std::size_t count, std::uint64_t offset) {
    
    assert(handle);
    
    if(offset + count > getSize())
        throw std::invalid_argument("End of file");
    
    OVERLAPPED overlapped = {};
    overlapped.Offset = static_cast<DWORD>(offset);
    overlapped.OffsetHigh = offset >> 32;
    DWORD byteCount;
    if(!ReadFile(handle, buffer, count, &byteCount, &overlapped)) {
        
        std::cout << GetLastError() << std::endl;
        throw std::runtime_error("ReadFile failed");
        
    }
    
}
void File::write(const Byte* buffer, std::size_t count, std::uint64_t offset) {
    
    assert(handle);
    
    OVERLAPPED overlapped = {};
    overlapped.Offset = static_cast<DWORD>(offset);
    overlapped.OffsetHigh = offset >> 32;
    DWORD byteCount;
    if(!WriteFile(handle, buffer, count, &byteCount, &overlapped)) {
        
        std::cout << GetLastError() << std::endl;
        throw std::runtime_error("WriteFile failed");
        
    }
    
}
void File::flush() {
    
    if(!FlushFileBuffers(handle)) {
        
        std::cout << GetLastError() << std::endl;
        throw std::runtime_error("FlushFileBuffers failed");
        
    }
    
}
std::uint64_t File::getSize() {
    
    assert(handle);
    
    LARGE_INTEGER s;
    if(!GetFileSizeEx(handle, &s)) {
        
        std::cout << GetLastError() << std::endl;
        throw std::runtime_error("GetFileSizeEx failed");
        
    }
    return s.QuadPart;
    
}
void File::setSize(std::uint64_t size) {
    
    assert(handle);
    
    LARGE_INTEGER s;
    s.QuadPart = size;
    if(!SetFilePointerEx(handle, s, nullptr, FILE_BEGIN)) {
        
        std::cout << GetLastError() << std::endl;
        throw std::runtime_error("SetFilePointerEx failed");
        
    }
    if(!SetEndOfFile(handle)) {
        
        std::cout << GetLastError() << std::endl;
        throw std::runtime_error("SetEndOfFile failed");
        
    }
    
}

}
}
}
