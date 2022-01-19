//
// Created by swagger on 2022/1/19.
//
#include "../../src/BloomFilter.h"
#include <iostream>
#include <unistd.h>
#include <cassert>
#include <fcntl.h>

using std::cout;
using std::endl;
using std::string;


int main() {
  BloomFilter f;
  string key = "test";
  string key2 = "this";
  cout << "has key?:  " << f.has(key) << endl; // 0
  cout << "has key2?: " << f.has(key2) << endl; // 0
  f.add(key);
  cout << "has key?:  " << f.has(key) << endl; // 1
  cout << "has key2?: " << f.has(key2) << endl; // 0
  f.add(key2);
  cout << "has key2?: " << f.has(key2) << endl; // 1

  string data = f.getString();

  int fd = open("./tmp.txt", O_CREAT | O_RDWR);
  assert( fd > 1 );

  int ret = write(fd, data.c_str(), data.length() );
  assert (ret == data.length());
  close(fd);
}



