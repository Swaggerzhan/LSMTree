//
// Created by swagger on 2022/1/19.
//

#include "BloomFilter.h"
#include "MurmurHash3.h"

using std::string;

// for debug
#include <iostream>
using std::endl;
using std::cout;

BloomFilter::BloomFilter()
: pbv_(nullptr)
, kRound_(5)
, kM_((LEN)-1)
{
  pbv_ = new Bit;
}

BloomFilter::~BloomFilter() {
  delete pbv_;
}

void BloomFilter::add(const string key) {
  uint32_t seed = 0;
  for (int i=0; i<kRound_; i++) {
    uint32_t pos = 0;
    MurmurHash3_x86_32(key.c_str(), key.length(), seed, &pos);
    seed = pos; // for next loop
    pos &= kM_;
    pbv_->set(pos);
  }
}

void BloomFilter::add(const int key) {
  uint32_t seed = 0;
  for (int i=0; i<kRound_; i++) {
    uint32_t pos = 0;
    MurmurHash3_x86_32(&key, sizeof(int), seed, &pos);
    seed = pos; // for next loop
    pos &= kM_;
    pbv_->set(pos);
  }
}



bool BloomFilter::has(const string key) {
  uint32_t seed = 0;
  for (int i=0; i<kRound_; i++) {
    uint32_t pos = 0;
    MurmurHash3_x86_32(key.c_str(), key.length(), seed, &pos);
    seed = pos;
    pos &= kM_;
    if ( !pbv_->test(pos) )
      return false;
  }
  return true;
}

bool BloomFilter::has(const int key) {
  uint32_t seed = 0;
  for (int i=0; i<kRound_; i++) {
    uint32_t pos = 0;
    MurmurHash3_x86_32(&key, sizeof(int), seed, &pos);
    seed = pos;
    pos &= kM_;
    if ( !pbv_->test(pos) )
      return false;
  }
  return true;
}

string BloomFilter::getString() {
  return pbv_->to_string();
}