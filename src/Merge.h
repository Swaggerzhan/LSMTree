//
// Created by swagger on 2022/1/12.
//

#ifndef LSMTREE_MERGE_H
#define LSMTREE_MERGE_H

#include <cassert>
#include <queue>
#include "types.h"

using std::priority_queue;
using std::vector;
using std::greater;


struct merge_entry {
public:

  entry_t head() const;
  bool done() const;

  bool operator>(const merge_entry& other) const;

public:

  int precedence; // 优先度？ 版本？

  entry_t* entries;
  long num_entries;

  int current_index = 0;
};

typedef struct merge_entry merge_entry_t;

class MergeContext {
public:

  void add(entry_t*, long);
  entry_t next();

  bool done();

private:

  priority_queue<merge_entry_t, vector<merge_entry_t>, greater<merge_entry_t>> queue_;
};








#endif //LSMTREE_MERGE_H
