//
// Created by swagger on 2022/1/12.
//

#include "Run.h"
#include <cassert>
#include <fcntl.h>
#include <sys/mman.h>
#include <algorithm>

Run::Run(long max_size, float bf_bits_per_entry)
        : max_size(max_size)
        , cur_size(0)
        , tmp_file("tmp_file_name")
        , mapping_(nullptr)
        , mapping_fd_(-1)
        , mapping_length_(0)
{
  fence_pointers_.reserve( max_size / getpagesize() );
}

Run::~Run() {
  assert( mapping_ != nullptr );
  remove(tmp_file.c_str()); // ????
}


entry_t* Run::map_read(size_t len, off_t offset) {
  assert( mapping_ == nullptr );

  mapping_length_ = len;
  mapping_fd_ = open(tmp_file.c_str(), O_RDONLY);
  assert ( mapping_fd_ != -1 );

  mapping_ = (entry_t*)mmap(0, mapping_length_, PROT_READ, MAP_SHARED, mapping_fd_, offset);
  assert( mapping_ != MAP_FAILED ); // ((void*)-1)
  return mapping_;
}


entry_t* Run::map_read() {
  map_read(max_size * sizeof(entry_t), 0);
  return mapping_;
}

entry_t* Run::map_write() {
  assert ( mapping_ == nullptr );
  mapping_length_ = max_size * sizeof(entry_t);
  mapping_fd_ = open(tmp_file.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0600);
  assert( mapping_fd_ != -1 );

  int ret = lseek(mapping_fd_, mapping_length_ - 1, SEEK_SET);
  assert( ret != -1 );
  ret = write(mapping_fd_, "", 1);
  assert( ret != -1 );

  mapping_ = (entry_t*)mmap(0, mapping_length_, PROT_WRITE, MAP_SHARED, mapping_fd_, 0);
  assert( mapping_ != MAP_FAILED );
  return mapping_;
}





long Run::file_size() {
  return max_size * sizeof(entry_t);
}


void Run::put(entry_t target) {
  assert( cur_size < max_size );
  // TODO: set bloom filter

  if ( cur_size % getpagesize() == 0 ) {
    fence_pointers_.push_back(target.key);
  }

  max_key_ = target.key > max_key_ ? target.key : max_key_;
  mapping_[cur_size] = target;
  cur_size ++;
}


VAL_t* Run::get(KEY_t key) {
  vector<KEY_t>::iterator next_page;
  VAL_t* val = nullptr;

  if (key < fence_pointers_[0] || key > max_key_) { // || !bloom_filter...
    return nullptr; // nullptr
  }

  next_page = upper_bound(fence_pointers_.begin(), fence_pointers_.end(), key);
  long page_index = (next_page - fence_pointers_.begin()) - 1;

  assert ( page_index >= 0 );

  map_read(getpagesize(), page_index * getpagesize()); // once we use disk

  for (int i=0; i<getpagesize() / sizeof(entry_t); i++) {
    if ( mapping_[i].key == key ) {
      val = new VAL_t;
      *val = mapping_[i].val;
      break; // ?
    }
  }

  unmap();
  // ummap
  return val;

}

void Run::unmap() {
  assert( mapping_ != nullptr );

  munmap(mapping_, mapping_length_);
  close(mapping_fd_);

  mapping_ = nullptr;
  mapping_length_ = 0;
  mapping_fd_ = -1;
}
