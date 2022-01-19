//
// Created by swagger on 2022/1/12.
//

#ifndef LSMTREE_LEVEL_H
#define LSMTREE_LEVEL_H

#include "Run.h"
#include <queue>

/**
 * @brief Level中有许多Run，每个Run对应一个文件
 *  Run即文件读写使用的
 */
class Level {
public:

  Level(int n, long s)
          : max_runs(n)
          , max_run_size(s)
  {}

  /**
   * @brief
   * 是否还有Run可放到当前Level中去
   * @return true has space
   * @return false  not more space
   */
  bool remainning() const {
    return max_runs - runs.size();
  }

  void newRunAndWrite() {
    runs.emplace_front(max_run_size, test_filter_);
    runs.front().map_write();
  }

public:

  float test_filter_;
  int max_runs;
  long max_run_size; //??
  std::deque<Run> runs; // ?

};

#endif //LSMTREE_LEVEL_H
