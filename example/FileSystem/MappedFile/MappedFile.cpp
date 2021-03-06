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

#include <cstring>

#include <exception>
#include <iostream>

#include "Cats/Netycat/FileSystem.hpp"

using namespace Cats::Corecat;
using namespace Cats::Netycat;

int main(int argc, char** argv) {
    
    if(argc < 3) {
        
        std::cerr << "error: file name needed" << std::endl;
        return 1;
        
    }
    try {
        
        File file1(argv[1], File::Mode::READ);
        std::size_t size = std::size_t(file1.getSize());
        MappedFile mappedFile1(file1, 0, size, MappedFile::Mode::READ);
        auto data1 = mappedFile1.getData();
        
        File file2(argv[2], File::Mode::READ_WRITE | File::Mode::CREATE);
        file2.setSize(size);
        MappedFile mappedFile2(file2, 0, size, MappedFile::Mode::READ_WRITE);
        auto data2 = mappedFile2.getData();
        
        std::memcpy(data2, data1, size);
        
    } catch(std::exception& e) { std::cout << e.what() << std::endl; }
    
    return 0;
    
}
