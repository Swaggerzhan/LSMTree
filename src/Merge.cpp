//
// Created by swagger on 2022/1/12.
//

#include "Merge.h"

entry_t merge_entry::head() const {
  return entries[current_index];
}

bool merge_entry::done() const {
  return current_index == num_entries;
}

bool merge_entry::operator>(const merge_entry &other) const {
  if (head() == other.head()) {
    assert( precedence != other.precedence );
    return precedence > other.precedence;
  }else {
    return head() > other.head();
  }
}

// ---------- MergeContext -------

void MergeContext::add(entry_t *entries, long num_entries) {
  merge_entry_t mergeEntry;
  if ( num_entries > 0 ) {
    mergeEntry.entries = entries;
    mergeEntry.num_entries = num_entries;
    mergeEntry.precedence = queue_.size(); // ??? 先进去的优先度低？
    queue_.push(mergeEntry);
  }
}

entry_t MergeContext::next() {
  merge_entry_t current, next;

  current = queue_.top();
  next = current;

  while (next.head().key == current.head().key && ! queue_.empty()) {
    queue_.pop();

    next.current_index ++;
    if ( !next.done() ) {
      queue_.push(next);
    }

    next = queue_.top();
  }
  return current.head();
}

bool MergeContext::done() {
  return queue_.empty();
}