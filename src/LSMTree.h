//
// Created by swagger on 2022/1/12.
//

#ifndef LSMTREE_LSMTREE_H
#define LSMTREE_LSMTREE_H

#include <vector>
#include "Buffer.h"
#include "Level.h"


class LSMTree {
public:

  LSMTree(int, int, int, int, float);

  void put(KEY_t, VAL_t);
  VAL_t* get(KEY_t);
  void del(KEY_t);

private:
  void merge_down(std::vector<Level>::iterator);
  Run* get_run(int index);


private:

  Buffer buffer_;
  // TODO: thread...
  // TODO: bf_bits...
  std::vector<Level> levels_;

};






#endif //LSMTREE_LSMTREE_H
