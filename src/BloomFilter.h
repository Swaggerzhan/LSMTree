//
// Created by swagger on 2022/1/19.
//

#ifndef LSMTREE_BLOOMFILTER_H
#define LSMTREE_BLOOMFILTER_H

#include <bitset>
#include <string>

#define LEN 1 << 26 // power of 2

typedef std::bitset<LEN> Bit;

class BloomFilter {
public:
  BloomFilter();
  ~BloomFilter();

  void add(const std::string key);
  void add(const int key);

  bool has(const std::string key);
  bool has(const int key);


  // FIXME: for debug
  std::string getString();

private:
  Bit* pbv_;

  int kRound_;
  int kM_;

};




#endif //LSMTREE_BLOOMFILTER_H
