//
// Created by swagger on 2022/1/12.
//

#include "Buffer.h"


Buffer::Buffer(int max_size)
        : maxSize_(max_size)
        , entries_()
{}

VAL_t* Buffer::get(KEY_t key) const {

  auto it = entries_.find(key);

  if ( it == entries_.end() )
    return nullptr;

  VAL_t* ret = new VAL_t;
  *ret = it->second;
  return ret;
}

bool Buffer::put(KEY_t key, VAL_t val) {
  if ( entries_.size() == maxSize_ )
    return false;
  entries_[key] = val;
  return true;
}

void Buffer::empty() {
  entries_.clear();
}