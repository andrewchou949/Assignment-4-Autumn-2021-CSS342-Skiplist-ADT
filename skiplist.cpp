// Name: Andrew Chou
// Assignment: 4
// modified by mashhadi on 14th feb to add an alternative op<<

#include <cassert>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "skiplist.h"

using namespace std;

// Pre: No display available
// Post: printing the skiplist to console.
ostream &operator<<(ostream &Out, const SkipList &SkipL) {
  for (int Index = SkipL.maxLevel - 1; Index >= 0; Index--) {
    Out << "Level: " + to_string(Index) + " -- ";
    SNode *Curr = SkipL.FrontGuards[Index];
    while (Curr != nullptr) {
      Out << to_string(Curr->value) + ", ";
      Curr = Curr->forward;
    }
    Out << "\n";
  }
  return Out;
}

// Constructor for SNode Class
// set the data, set all other pointers to nullptr
// Pre: no pointer available
// Post: assign value to private variable value
// set all pointers to nullptr
SNode::SNode(int value) : value{value} {
  forward = nullptr;
  backward = nullptr;
  up = nullptr;
  down = nullptr;
}

// Constructor for Skiplist
// Pre: no skiplist info available
// Post: assign value to maxLevel and probability
// dynamically allocated frontguard rearguard
// and store the pointers to the front guard and rear guard array
SkipList::SkipList(int maxLevel, int probability)
    : maxLevel{maxLevel}, probability{probability} {
  assert(maxLevel > 0 && probability >= 0 && probability < 100);
  // Creating the frontguard, rearguard array to store pointers
  FrontGuards = new SNode *[maxLevel];
  RearGuards = new SNode *[maxLevel];
  for (int i = 0; i < maxLevel; i++) {
    SNode *tmpFront = new SNode(INT_MIN);
    SNode *tmpRear = new SNode(INT_MAX);
    // Store in FrontGuards array
    FrontGuards[i] = tmpFront;
    // Store in RearGuards array
    RearGuards[i] = tmpRear;
    // tie nodes forward and backward
    FrontGuards[i]->forward = RearGuards[i];
    RearGuards[i]->backward = FrontGuards[i];
  }
  // tie nodes up and down
  for (int i = maxLevel - 1; i >= 1; i--) {
    FrontGuards[i]->down = FrontGuards[i - 1];
    FrontGuards[i - 1]->up = FrontGuards[i];
    RearGuards[i]->down = RearGuards[i - 1];
    RearGuards[i - 1]->up = RearGuards[i];
  }
}

// The flip coin algorithm to see if it should increase height
// Pre: no flip coin algorithm
// Post: flip coin depending on the probability provided
bool SkipList::shouldInsertAtHigher() const {
  return rand() % 100 > probability;
}

// Pre: no value added
// Post: adding values of vector to the skiplist
// No duplicate allowed
bool SkipList::add(const vector<int> &values) {
  int size = values.size();
  bool existing;
  for (int i = 0; i < size; i++) {
    existing = contains(values.at(i));
    // Not exist
    if (existing == false) {
      add(values.at(i)); // Add the non existence value
    }
  }
  return false;
}

// Pre: No value added
// Post: add one value to the skiplist accordingly
// insert that value to next level depending on probability
// No duplicate allowed
bool SkipList::add(int value) {
  // temporary node pointer
  SNode *tmp = FrontGuards[0]->forward; // Step 1
  // Step 2.
  while (tmp->forward != nullptr && tmp->value < value) {
    tmp = tmp->forward;
  }
  // Step 3.
  if (tmp->value == value) {
    return false;
  }
  // Step 4.
  SNode *newNode = new SNode(value);
  addBefore(newNode, tmp);
  int level = 1;
  // Insert at higher level?
  // While condition here is the flip coin condition, 50-50 increase height
  while (level < maxLevel) {
    if (shouldInsertAtHigher()) { // Step 8 (Toss Coin)
      // Step 1.
      SNode *newUpper = new SNode(value);
      // Step 2.
      newNode->up = newUpper;
      newUpper->down = newNode;
      // Step 3.
      SNode *curr = newNode->backward;
      while (curr->up == nullptr) {
        curr = curr->backward;
      }
      // Step 4.
      curr = curr->up;
      // Step 5.
      curr = curr->forward;
      // Step 6.
      addBefore(newUpper, curr);
      // Step 7.
      newNode = newUpper;
      level++;
    } else {
      break;
    }
  }
  return true;
}

// Pre: memory leak
// Post: delete the pointer and the array of pointers
// to prevent memory leak
SkipList::~SkipList() {
  // need to delete individual nodes
  SNode *curr;
  SNode *tmp;
  for (int i = maxLevel - 1; i >= 0; i--) {
    curr = FrontGuards[i];
    while (curr != nullptr) {
      tmp = curr;
      curr = curr->forward;
      delete tmp;
    }
  }
  delete[] FrontGuards;
  delete[] RearGuards;
}

// Pre: no value removed
// Post: disconnect the node for the data
// delete the pointer for that node for memory leak prevention
bool SkipList::remove(int data) {
  // Starting from the top left
  SNode *curr = FrontGuards[maxLevel - 1];
  SNode *tmp;

  while (curr != nullptr) {
    // moving forward to right to find the spot of data
    while (curr->forward->value < data) {
      curr = curr->forward;
    }
    if (curr->forward->value == data) {
      while (curr != nullptr) {
        tmp = curr;
        // Moving down
        curr = curr->down;
        // Skipping one node (To Delete it)
        tmp->backward->forward = tmp->forward;
        tmp->forward = nullptr;
        tmp->backward = nullptr;
        tmp->up = nullptr;
        tmp->down = nullptr;
        delete tmp; // delete to prevent memory leak
        tmp = nullptr;
      }
      return true; // Return true after successfully deleting
    }
    // keep moving down
    curr = curr->down;
  }
  return false; // If cannot find existing, cannot delete.
}

// Given a SNode, place it before the given NextNode
// Pre: two pointers node has no ref to each other
// Post: provide ref (back and forward) for two nodes
void SkipList::addBefore(SNode *NewNode, SNode *NextNode) {
  assert(NewNode != nullptr && NextNode != nullptr &&
         NewNode->value < NextNode->value);
  // Link next to node in front
  NewNode->forward = NextNode;
  // Link prev to node behind
  NewNode->backward = NextNode->backward;
  // Link node in back to new node
  NextNode->backward->forward = NewNode;
  // Link node in front to new node
  NextNode->backward = NewNode;
  assert(NewNode->forward == NextNode && NextNode->backward == NewNode);
  assert(NewNode->backward != nullptr &&
         NewNode->backward->value < NewNode->value);
}

/*
// Checks to see whether or not a data value exists in the list
// Returns the NODE if the value exists in the SkipList.
// Returns nullptr otherwise
// Must be O(logn)
SNode *SkipList::containsSNode(int data) const {
  SNode *curr = FrontGuards[maxLevel - 1];
  while (curr != nullptr) {
    while (curr->forward->value < data) {
      curr = curr->forward;
    }
    if (curr->forward->value == data) {
      return curr;
    }
  }
  return nullptr;
}
*/

// Checks to see whether or not a data value exists in the list
// Returns true if the value exists in the SkipList.
// Returns false otherwise
// Must be O(logn)
// Pre: unable to check if value exist
// Post: return true if value exist in skiplist, false otherwise
bool SkipList::contains(int data) const {
  SNode *tmp = FrontGuards[maxLevel - 1];
  while (tmp->down != nullptr) {
    tmp = tmp->down;
    while (data >= tmp->forward->value) {
      tmp = tmp->forward;
      if (data == tmp->value) {
        return true;
      }
    }
  }
  return false;
}