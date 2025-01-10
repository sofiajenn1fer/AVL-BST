// UMBC - CMSC 341 - Fall 2022 - Proj2
// Name: Sofia Gomes
// Date: 10/25/2022
// Section: 01
// Email: sgomes2@umbc.edu
// Description: Contains Streak.h class functions implemented as well as helper functions
#include "streak.h"

// constructor, sets m_root as nullptr
Streak::Streak(){
    m_root = nullptr;
}

// destructor, calls clear()
Streak::~Streak(){
    clear();
}

// insert, checks if id is within MINID and MAXID, as well as if it is already in the tree, before inserting it
void Streak::insert(const Tiger& tiger){
    bool duplicate;
    if (tiger.getID() >= MINID && tiger.getID() <= MAXID){
        duplicate = findTiger(tiger.getID());
        if (!duplicate){
            insert(tiger, m_root);
            // updates height of m_root after inserting
            updateHeight(m_root);
        }
    }
}

// deletes tree and sets m_root to nullptr
void Streak::clear(){
    clear(m_root);
    m_root = nullptr;
}

// removes a node if it exists in the tree
void Streak::remove(int id){
    if (findTiger(id)){
        remove(id, m_root);
    }
}


// updates height of a node
void Streak::updateHeight(Tiger* aTiger){
    // checks if aTiger is null so it doesn't segfault by accessing null members
    if(aTiger != nullptr){
        // if the tiger is a leaf node the height is 0
        if (aTiger->getLeft() == nullptr && aTiger->getRight() == nullptr){
            aTiger->setHeight(0);
        // if the tiger has one child the height is that child+1
        }else if (aTiger->getLeft() == nullptr && aTiger->getRight() != nullptr){
            aTiger->setHeight(aTiger->getRight()->getHeight()+1);
        }else if (aTiger->getLeft() != nullptr && aTiger->getRight() == nullptr){
            aTiger->setHeight(aTiger->getLeft()->getHeight()+1);
        // if the tiger has both children, compares greatest height and adds 1
        }else if (aTiger->getLeft() != nullptr && aTiger->getRight() != nullptr){
            if (aTiger->getLeft()->getHeight() > aTiger->getRight()->getHeight()){
                aTiger->setHeight(aTiger->getLeft()->getHeight() + 1);
            }else{
                aTiger->setHeight(aTiger->getRight()->getHeight()+1);
            }
        }
    }
}


// checks the imbalance of a current node
int Streak::checkImbalance(Tiger* aTiger){
    // if aTiger's children are leaf nodes the imbalance factor is 0
    if (aTiger->getLeft() == nullptr && aTiger->getRight() == nullptr){
        return 0;
    }
    // if the tiger has one child, subtracts heights with factor of -1
    if (aTiger->getLeft() == nullptr && aTiger->getRight() != nullptr){
        return -1-aTiger->getRight()->getHeight();
    }
    if (aTiger->getLeft() != nullptr && aTiger->getRight() == nullptr){
        return aTiger->getLeft()->getHeight()-(-1);
    }
    // subtracts left child height from right child height
    return aTiger->getLeft()->getHeight()-aTiger->getRight()->getHeight();
}


// rebalances an imbalanced node
Tiger *Streak::rebalance(Tiger* aTiger){
    // checks if aTiger is not null so there isn't seg fault when trying to access null members
    if (aTiger != nullptr){
        // checks imbalance of current node
        int imbalance = checkImbalance(aTiger);
        // if there isn't an imbalance nothing happens
        if (imbalance < 2 && imbalance >= -1){
            return aTiger;
        // if the imbalance is on the left side
        }else if (imbalance > 1){
            // takes imbalance factor of left child to see which of it's children have more children
            int child = checkImbalance(aTiger->getLeft());
            // if the second imbalance factor is >= 0 then it is a single right rotation. returns the root of that
            // rotation
            if (child >= 0){
                Tiger *balanced = singleRight(aTiger);
                return balanced;
            }
            // if the second imbalance factor is <= 0 then it is a double left right rotation. returns the root of that
            // rotation
            if (child < 0){
                Tiger *balanced = leftRight(aTiger);
                return balanced;
            }
        // if the imbalance is on the right side
        }else if (imbalance < -1){
            // takes balance factor of its right child to see which side has the most children
            int child = checkImbalance(aTiger->getRight());
            // if the second imbalance factor is <= 0, then it is a single left rotation. returns the root of that
            // rotation
            if (child <= 0){
                Tiger *balanced = singleLeft(aTiger);
                return balanced;
            }
            // if the second imbalance factor is >= 0, then it is a double right left rotation. returns the root of that
            // rotation
            if (child > 0){
                Tiger *balanced = rightLeft(aTiger);
                return balanced;
            }
        }
    }
    // returns null if none of these conditions apply
    return nullptr;
}

void Streak::dumpTree() const {dump(m_root);}

void Streak::dump(Tiger* aTiger) const{
    if (aTiger != nullptr){
        cout << "(";
        dump(aTiger->m_left);//first visit the left child
        cout << aTiger->m_id << ":" << aTiger->m_height;//second visit the node itself
        dump(aTiger->m_right);//third visit the right child
        cout << ")";
    }
}

void Streak::listTigers() const {
    listTigers(m_root);
}

// sets state of specific tiger; checks if it exists
bool Streak::setState(int id, STATE state){
    if (findTiger(id)) {
        getTiger(id)->setState(state);
        return true;
    }
    return false;
}

void Streak::removeDead(){
    findDead(m_root);
}

// returns true if tiger is in tree. returns false if it isn't
bool Streak::findTiger(int id) const {
    bool duplicate = duplicates(id, m_root);
    if (duplicate){
        return true;
    }
    return false;
}

int Streak::countTigerCubs() const{
    int total = 0;
    countTigerCubs(total, m_root);
    return total;
}

// recursively deletes a tree by post order traversal
void Streak::clear(Tiger *aTiger) {
    if (aTiger != nullptr){
        clear(aTiger->getLeft());
        clear(aTiger->getRight());
        delete aTiger;
    }
}

// checks for duplicates in a tree recursively
bool Streak::duplicates(int id, Tiger *aTiger) const{
    // if aTiger is null that means we reached the bottom of the tree so that means the tiger did not exist
    if (aTiger == nullptr){
        return false;
    // returns true if tiger is found
    }else if (aTiger->getID() == id){
        return true;
    // if the id is larger than the current tiger's id, recurses to the right
    }else if (aTiger->getID() < id){
        return duplicates(id, aTiger->getRight());
    }else{
    // if the id is smaller than the current tiger's id, recurses to the left
        return duplicates(id, aTiger->getLeft());
    }
}

// inserts nodes into tree then updates height and rebalances the tree (recursively)
void Streak::insert(const Tiger &tiger, Tiger *&aTiger) {
    // base case, if at the bottom  of the tree, add a new tiger
    if (aTiger == nullptr){
        Tiger *newTiger = new Tiger(tiger);
        aTiger = newTiger;
    // if the id is smaller than aTiger's id, recurse to the left
    }else if (tiger.getID() < aTiger->getID()) {
        insert(tiger, aTiger->m_left);
        // recursive return call, updates height and rebalances
        updateHeight(aTiger->m_left);

        // if aTiger is the root, case is different; updates height and rebalances
        if (aTiger != m_root){
            aTiger->setLeft(rebalance(aTiger->getLeft()));
        }else{
            aTiger->setLeft(rebalance(aTiger->getLeft()));
            m_root = rebalance(aTiger);
        }
    }else{
        // if the id is larger than aTiger's id, recurse to right
        insert(tiger, aTiger->m_right);
        // recursive return call, updates height and rebalances
        updateHeight(aTiger->m_right);

        // if aTiger is the root, case is different; updates height and rebalances
        if (aTiger != m_root){
            aTiger->setRight(rebalance(aTiger->getRight()));
        }else{
            aTiger->setRight(rebalance(aTiger->getRight()));
            m_root = rebalance(aTiger);
        }
    }
}

// single left rotation
Tiger *Streak::singleLeft(Tiger *aTiger) {
    Tiger *z = aTiger;
    // gets z's right
    Tiger *y = aTiger->getRight();
    // sets z's right to y's left
    z->setRight(y->getLeft());
    // sets y's left to z
    y->setLeft(z);

    // updates heights of both y and z and returns y as root of subtree
    updateHeight(z);
    updateHeight(y);
    return y;
}

// single right rotation
Tiger *Streak::singleRight(Tiger *aTiger) {
    Tiger *z = aTiger;
    // gets z's left
    Tiger *y = z->getLeft();
    // sets z's left to y's right
    z->setLeft(y->getRight());
    // sets y's right to z
    y->setRight(z);

    // updates both heights and returns y as root of subtree
    updateHeight(z);
    updateHeight(y);
    return y;
}


// double left right rotation
Tiger *Streak::leftRight(Tiger *aTiger) {
    Tiger *z = aTiger;
    // gets left of z
    Tiger *y = z->getLeft();
    // gets right of y
    Tiger *x = y->getRight();
    // sets z's left to x's right
    z->setLeft(x->getRight());
    // sets x's right to z
    x->setRight(z);
    // sets y's right to x's left
    y->setRight(x->getLeft());
    // sets x's left to y
    x->setLeft(y);

    // updates hright of all three nodes and returns x as root of subtree
    updateHeight(z);
    updateHeight(y);
    updateHeight(x);
    return x;
}


// double right left rotation
Tiger *Streak::rightLeft(Tiger *aTiger) {
    Tiger *z = aTiger;
    // gets z's right
    Tiger *y = z->getRight();
    // gets y's left
    Tiger *x = y->getLeft();
    // sets z's right to x's left
    z->setRight(x->getLeft());
    // sets x's left to z
    x->setLeft(z);
    // sets y's left ro x's right
    y->setLeft(x->getRight());
    // sets x's right to y
    x->setRight(y);

    // updates height of all three nodes and returns x as root of subtree
    updateHeight(z);
    updateHeight(y);
    updateHeight(x);
    return x;
}

// recursively removes a node while updating height and rebalancing tree
void Streak::remove(int id, Tiger *&aTiger) {
    // if id is greater than aTiger's id, recurse to aTiger's right
    if (aTiger->getID() < id){
        remove(id, aTiger->m_right);
        // recursive return, update height and rebalance
        updateHeight(aTiger->m_right);

        // if aTiger is the root, case is different; updates height and rebalances
        if (aTiger != m_root){
            aTiger->setRight(rebalance(aTiger->m_right));
        }else{
            aTiger->setRight(rebalance(aTiger->m_right));
            m_root = rebalance(aTiger);
        }
    // if id is less than aTiger's id, recurse to aTiger's left
    }else if (aTiger->getID() > id){
        remove(id, aTiger->m_left);
        // recursive return, update height and rebalance
        updateHeight(aTiger->m_left);

        // if aTiger is the root, case is different; updates height and rebalances
        if (aTiger != m_root){
            aTiger->setLeft(rebalance(aTiger->getLeft()));
        }else{
            aTiger->setLeft(rebalance(aTiger->getLeft()));
            m_root = rebalance(aTiger);
        }
    // if the id matches aTiger's id
    }else if (aTiger->getID() == id) {
        // if aTiger is the root and the root is the only node in the tree, delete the root
        if (aTiger == m_root && !aTiger->getLeft() && !aTiger->getRight()){
            delete aTiger;
            m_root = nullptr;
        // if the tiger is a leaf node
        }else if (aTiger->getLeft() == nullptr && aTiger->getRight() == nullptr) {
            // keep temporary variable as tiger and retrieve tiger pointing to it
            // delete the tiger and update height of the tiger which was pointing to it
            Tiger *toDelete = aTiger;
            Tiger *temp = removeHelper(aTiger->getID(), m_root);
            delete toDelete;
            updateHeight(temp);
        // if the tiger has one child
        }else if (aTiger->getLeft() != nullptr && aTiger->getRight() == nullptr){
            // if the tiger has a left child, copy variables of its child to itself
            Tiger *temp = aTiger->getLeft();
            assignTiger(1, aTiger, temp);

            // delete the child and update height of the tiger, then rebalance
            temp->setLeft(nullptr);
            temp->setRight(nullptr);
            delete temp;
            updateHeight(aTiger);
            aTiger->setLeft(rebalance(aTiger->getLeft()));
        }else if (aTiger->getRight() != nullptr && aTiger->getLeft() == nullptr){
            // if the tiger has a right child, copy variables of its child to itself
            Tiger *temp = aTiger->getRight();
            assignTiger(1, aTiger, temp);

            // deletes the child and update the height of the tiger, then rebalance
            temp->setLeft(nullptr);
            temp->setRight(nullptr);
            delete temp;
            updateHeight(aTiger);
            aTiger->setRight(rebalance(aTiger->getRight()));
        // if the tiger has two children
        }else if (aTiger->getLeft() != nullptr && aTiger->getRight() != nullptr){
            // if the tiger is not the root
            if (aTiger != m_root){
                if (aTiger->getRight()->getLeft() == nullptr){
                    Tiger *temp = aTiger->getRight();
                    assignTiger(2, aTiger, temp);
                    aTiger->setRight(nullptr);
                    delete temp;
                }else{
                    // normal case, update height and rebalance
                    Tiger *temp = minTigerParent(aTiger->getRight());
                    Tiger *toDelete = temp->getLeft();
                    assignTiger(2, aTiger, toDelete);
                    twoChildrenRemove(temp, toDelete);
                    updateHeight(temp);
                    removeRebalance(temp->getID(), m_root);
                }
            // if tiger is m_root
            }else if (aTiger == m_root){
                if (aTiger->getRight()->getLeft() == nullptr){
                    Tiger *temp = aTiger->getRight();
                    assignTiger(2, aTiger, temp);
                    if (temp->getRight()){
                        aTiger->setRight(temp->getRight());
                    }
                    delete temp;
                }else{
                    // normal case, update height and rebalance
                    Tiger *temp = minTigerParent(aTiger->getRight());
                    Tiger *toDelete = temp->getLeft();
                    assignTiger(2, aTiger, toDelete);
                    twoChildrenRemove(temp, toDelete);
                    updateHeight(temp);
                    removeRebalance(temp->getID(), m_root);
                }
                m_root = rebalance(aTiger);
            }
        }
    }
}


// copies member variables from one tiger to the other
void Streak::assignTiger(int num, Tiger *aTiger, Tiger *secondTiger) {
    aTiger->setID(secondTiger->getID());
    aTiger->setState(secondTiger->getState());
    aTiger->setAge(secondTiger->getAge());
    if (num == 1){
        aTiger->setLeft(secondTiger->getLeft());
        aTiger->setRight(secondTiger->getRight());
    }
}

Tiger *Streak::getTiger(int id) {
    return getTigerHelper(id, m_root);
}


// travers through tree and gets specific tiger
Tiger *Streak::getTigerHelper(int id, Tiger *aTiger) {
    // if it finds the tiger it returns it
    if (id == aTiger->getID()){
        return aTiger;
    // if the id is less than the current tiger's id, recurses to the left
    }else if (id < aTiger->getID()){
        return getTigerHelper(id, aTiger->getLeft());
    // if the id is greater than the current tiger's id, recurses to the right
    }else{
        return getTigerHelper(id, aTiger->getRight());
    }
}

Tiger *Streak::minTigerParent(Tiger *aTiger) {
    if (aTiger->getLeft()->getLeft() == nullptr){
        return aTiger;
    }else{
        return minTigerParent(aTiger->getLeft());
    }
}

void Streak::twoChildrenRemove(Tiger *parent, Tiger *toDelete) {
    if (toDelete->getRight() == nullptr){
        parent->setLeft(nullptr);
        delete toDelete;
    }else if(toDelete->getRight() != nullptr){
        Tiger *temp = toDelete->getRight();
        parent->setLeft(temp);
        delete toDelete;
    }
}

// lists tigers and their elements, in order traversal
void Streak::listTigers(Tiger *aTiger) const{
    // checks if not nullptr to avoid segfault from accessing null members
    if (aTiger != nullptr){
        listTigers(aTiger->getLeft());
        cout << aTiger->getID() << ":" << aTiger->getAgeStr() << ":" << aTiger->getGenderStr() << ":" << aTiger->getStateStr()
        << endl;
        listTigers(aTiger->getRight());
    }
}


// finds all dead tigers and deletes them, in order traversal
void Streak::findDead(Tiger *aTiger) {
    // checks if not nullptr to avoid segfault from accessing null members
    if (aTiger != nullptr){
        findDead(aTiger->getLeft());
        // if it finds the dead tiger, deletes it
        if (aTiger->getState() == 1){
            deadTiger(aTiger->getID());
            return;
        }
        findDead(aTiger->getRight());
    }
}

// counts all tiger cubs in a tree, in order traversal
void Streak::countTigerCubs(int &total, Tiger *aTiger) const {
    // checks if not nullptr to avoid segfault from accessing null members
    if (aTiger != nullptr){
        countTigerCubs(total, aTiger->getLeft());
        // if it finds a cub, increments the total
        if (aTiger->getAge() == 0){
            total++;
        }
        countTigerCubs(total, aTiger->getRight());
    }
}

Tiger* Streak::removeHelper(int id, Tiger *aTiger) {
    if (aTiger->getLeft() && aTiger->getLeft()->getID() == id){
        aTiger->setLeft(nullptr);
        return aTiger;
    }else if (aTiger->getRight() && aTiger->getRight()->getID() == id){
        aTiger->setRight(nullptr);
        return aTiger;
    }else if (aTiger->getID() < id){
        return removeHelper(id, aTiger->getRight());
    }
    else{
        return removeHelper(id, aTiger->getLeft());
    }
}

void Streak::removeRebalance(int id, Tiger *aTiger) {
    if (aTiger->getID() == id){
        updateHeight(aTiger);
    }else if (aTiger->getID() < id){
        removeRebalance(id, aTiger->getRight());
        updateHeight(aTiger->getRight());
        aTiger->setRight(rebalance(aTiger->getRight()));
    }else{
        removeRebalance(id, aTiger->getLeft());
        updateHeight(aTiger->getLeft());
        aTiger->setLeft(rebalance(aTiger->getLeft()));
    }
}

// deletes dead tiger
void Streak::deadTiger(int id) {
    if (id >= MINID && id <= MAXID){
        remove(id);
        findDead(m_root);
    }
}

