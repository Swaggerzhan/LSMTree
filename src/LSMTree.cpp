//
// Created by swagger on 2022/1/12.
//

#include "LSMTree.h"
#include "Merge.h"
#include <cassert>

using std::vector;


// for debug
#include <iostream>
using std::cout;
using std::endl;

LSMTree::LSMTree(int depth, int fanout, int runSize)
: depth_(depth)
, buffer_(3)
, fanout_(fanout)
, runSize_(runSize)
{
  for (int i=0; i<depth; i++) {
    levels_.emplace_back(fanout, runSize_);
    runSize_ *= fanout_;
  }
}


void LSMTree::merge_down(vector<Level>::iterator current) {
  assert( current >= levels_.begin());
  MergeContext mergeContext;
  vector<Level>::iterator next;
  entry_t entry;

  if ( current->remainning() ) {
    return;
  }else if ( current >= levels_.end() - 1 ) {
    // Error! current超出当前LevelList的范围了
  }else {
    next = current + 1; // 移动到下一个Level上
  }

  if ( next->remainning() == 0 ) {
    merge_down(next); // 递归向下Merge
    assert(next->remainning() > 0); // ?
  }

  // 当前current->runs中需要被合并的entry都放到mergeContext中，准备合并
  for (auto& run: current->runs) {
    mergeContext.add(run.map_read(), run.cur_size);
  }

  // 在下一个Level层中推入一个关于当前层数大小的Run
  next->runs.emplace_front(next->max_run_size, 0);
  next->runs.front().map_write(); // 开启写，开始将上层的东西写入到下层中

  while ( !mergeContext.done() ) {
    entry = mergeContext.next();
    if ( !(next == levels_.end() - 1 && entry.val == VAL_TOMBSTONE)) {
      next->runs.front().put(entry); // 合并到新加入的run对象中
    }
  }
  // 合并结束，释放run中关于磁盘的读写
  next->runs.front().unmap();

  for (auto & run : current->runs ) {
    run.unmap();
  }
  // 清空当前的run
  current->runs.clear();
}


void LSMTree::put(KEY_t key, VAL_t val) {
  if ( buffer_.put(key, val) ) {
    return; // success
  }

  merge_down(levels_.begin());

  //levels_.front().runs.emplace_front(levels_.front().max_run_size, 1.1);
  levels_.front().newRunAndWrite();

  // 持久化: 写入到run中mmap得到的内存地址 -> 磁盘
  for (const auto& i: buffer_.entries_) {
    entry_t target;
    target.key = i.first;
    target.val = i.second;
    levels_.front().runs.front().put(target);
  }

  levels_.front().runs.front().unmap(); // 持久化到磁盘

  buffer_.empty();
  assert( buffer_.put(key, val));
}


VAL_t* LSMTree::get(KEY_t key) {
  // 直接在内存中找到
  VAL_t* value = buffer_.get(key);
  if ( value != nullptr )  {
    return value;
  }

  Run* run = nullptr;
  VAL_t* val = nullptr;
  int run_index = 0;
  while ( true ) {
    if ((run = get_run(run_index)) == nullptr) {
      return nullptr;
    }
    if ( (val = run->get(key)) == nullptr ) {
      run_index ++;
      continue;
    }
    return val;
  }
}

Run* LSMTree::get_run(int index) {
  for (const auto& level: levels_) {
    if ( index < level.runs.size() ) {
      return (Run*)&level.runs[index];
    }else {
      index -= level.runs.size();
    }
  }
  return nullptr;
}
