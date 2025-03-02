
#include <stdint.h>
#include <sys/mman.h>
#include <iostream>
#include <cerrno>  // 用于错误处理，获取错误码
#include <cstring> // 用于获取错误信息

extern "C" {
  #include <hugetlbfs.h>
}


void* AllocateAtAddress(void* requested_address, size_t size) {
  size_t hugepagesize = gethugepagesize();
  if (hugepagesize <= 0) {
    std::cerr << "Error getting default huge page size" << std::endl;
  }
  size = (size + (hugepagesize - 1)) & ~(hugepagesize - 1);
  void* buf =
        mmap(requested_address, size, PROT_READ | PROT_WRITE,
             MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_FIXED, -1, 0);
  if (buf == MAP_FAILED || buf != requested_address) {
    std::cerr << "Error allocating memory region" << std::endl;
  }
  return buf;
}



int main(int argc, char **argv) {
  // allocate 2MB of memory
  size_t size = 2 * 1024 * 1024;
  void* buf = AllocateAtAddress((void*)(1L << 44), size);
  std::cout << "Allocated " << size << "B buffer at address " << buf << std::endl;
  // free the memory
  munmap(buf, size);
  return 0;
}