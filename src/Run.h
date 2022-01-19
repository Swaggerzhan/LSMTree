//
// Created by swagger on 2022/1/12.
//

#ifndef LSMTREE_RUN_H
#define LSMTREE_RUN_H

#include <vector>
#include "types.h"
#include <unistd.h>
#include <string>

using std::vector;
using std::string;


class Run {
public:

  /**
   * @brief Construct a new Run object
   *  long max_size: 文件最大大小
   *  float bf_bits_per_entry:
   */
  Run(long, float);
  ~Run();

  /**
   * @brief open tmp_file and mmap it, read only
   * @size_t len: the length that we want mmap size
   * @off_t offset: the begin of mmap offset
   * @return entry_t*
   */
  entry_t* map_read(size_t, off_t);
  entry_t* map_read();

  /**
   * @brief open tmp_file and mmap it and it can be write
   *
   * @return entry_t*
   */
  entry_t* map_write();
  void unmap();

  VAL_t* get(KEY_t);
  void put(entry_t);


public:
  long cur_size; // 当前szie

  long max_size; // 可用总size?

  string tmp_file;

private:

  // return the size of current file
  long file_size(); // ?


private:
  // TODO: bloom filter...
  vector<KEY_t> fence_pointers_; // for fence pointer
  KEY_t max_key_;

  int mapping_fd_; // mmap
  entry_t* mapping_; // the address of mmap return value
  size_t mapping_length_;



};


#endif //LSMTREE_RUN_H
