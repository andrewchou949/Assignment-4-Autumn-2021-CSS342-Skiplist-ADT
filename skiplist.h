/**
 * SkipList https://en.wikipedia.org/wiki/Skip_list
 * also used by MemSQL
 * https://www.singlestore.com/blog/what-is-skiplist-why-skiplist-index-for-memsql/
 * SkipList with a depth of 1 is similar to a doubly-linked list
 * Each item has a p percent chance of being at the next level up
 * For our implementation p = 50%
 * All elements are inserted at the lowest Depth (1)
 * 50% of the elements inserted in Depth = 2
 * 50% of 50%, or 25% of the elements inserted in Depth = 3
 * and so on
 *
 * If a Skip List has only 1 level, such as p = 0%
 * Insert O(n), Search O(n)
 * For Depth > 1
 * Insert O(log n), Search O(log n)
 * Modifying p allows us to trade off search speed and storage cost
 *
 */

#ifndef ASS4_SKIPLIST_H
#define ASS4_SKIPLIST_H

#include <iostream>
#include <vector>

using namespace std;

// only SkipList class can create SNode objects

class SkipList;

class SNode {
  friend class SkipList;
  friend ostream &operator<<(ostream &out, const SkipList &skip);

//? Should it be public instead? In instruction, it is public.
private:
  // constructor
  // set the data, set all other pointers to nullptr
  explicit SNode(int value);

  // data contained in the object
  // Key
  int value{0};

  // link to Next SNode 
  SNode * forward;

  // link to Prev SNode
  SNode * backward;
   
  //link to up node 
  SNode * up;

  // link to down SNode
  SNode * down;

/* Shouldn't this supposed to be under Skiplist?
  // how many forward/backward pointers it has
  int height() const;

  // increase the number of forward/backward pointers it has
  void increaseHeight();
  */
};

class SkipList {
  // display with level
  friend ostream &operator<<(ostream &out, const SkipList &skip);

private:
  // maximum number of levels
  // int depth in instruction
  int maxLevel = 1;

  // probability of inserting at a higher level
  //Probability: tossing coin using (rand() % 2) == 1 (50%)
  // as an integer between 0% and 100% (exclusive)
  int probability = 0;

  //set the height to start from at least one level (L0)
  int heightSkipList = 1;

  // head of linked levels
  // Upmost Left side
  SNode *head;

  // tail of linked levels
  // Upmost Right Side
  SNode *tail;

  // array of depth SNode* objects as frontGuards linking levels
  // if depth = 2, we'd have frontGuards[0] and frontGuards[1]
  // array of Depth SNode* objects as FrontGuards linking levels
  SNode **FrontGuards;
  // array of Depth SNode* objects as RearGuards linking levels
  SNode **RearGuards;

  // given a SNode, place it before the given nextNode
  void addBefore(SNode *newNode, SNode *nextNode);

  // return true p% of time,
  // for p = 50, each node has a 50% chance of being at higher level
  // alsoHigher() in instruction.
  bool shouldInsertAtHigher() const;

  vector<SNode *> getBeforeNodes(int data) const;

public:
  // default SkipList has only 1 level, just one doubly-linked list
  // Dynamically allocate frontGuards and rearGuards arrays, 
  // create the special SNode* objects as guards, tie all the SNode objects together (both prev-next and up-down)
  explicit SkipList(int maxLevel = 1, int probability = 0);

  // destructor
  virtual ~SkipList();

  // copy not allowed
  SkipList(const SkipList &other) = delete;

  // move not allowed
  SkipList(SkipList &&other) = delete;

  // assignment not allowed
  SkipList &operator=(const SkipList &other) = delete;

  // move assignment not allowed
  SkipList &operator=(SkipList &&other) = delete;

  // return true if successfully added, no duplicates allowed
  bool add(int value);

  //void addVoidVersion(int value);
  
  // return true if successfully added, no duplicates
  bool add(const vector<int> &values);

  // return true if successfully removed
  bool remove(int data);

  // return true if found in SkipList
  bool contains(int data) const;

  //SNode *containsSNode(int data) const;
};

#endif // ASS4_SKIPLIST_H