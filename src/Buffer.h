//
// Created by swagger on 2022/1/12.
//

#ifndef LSMTREE_BUFFER_H
#define LSMTREE_BUFFER_H

#include <set>
#include <vector>
#include <map>
#include "types.h"

class Buffer {
public:

  explicit Buffer(int max_size);

  ~Buffer() = default;

  VAL_t* get(KEY_t) const;
  bool put(KEY_t, VAL_t);

  // it should call clear !
  void empty();

public:

  int maxSize_;

  //std::set<entry_t> entries_;
  std::map<KEY_t, VAL_t> entries_;

};

#endif //LSMTREE_BUFFER_H
