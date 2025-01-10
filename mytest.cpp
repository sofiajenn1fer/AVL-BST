#include "streak.h"
#include <vector>
#include <random>
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

class Tester{
public:
    //Tester functions
    void insert(); // tests normal/edge/error insert
    void insertTest(bool &, Tiger *aTiger); // checks if nodes were inserted properly
    void duplicate(int&, int, Tiger *aTiger); // checks for duplicates in tree
    void rebalance(); // tests rebalance
    int checkImbalance(Tiger *aTiger); // checks if node is imbalanced
    void imbalanceCheck(bool&, Tiger *aTiger); // checks if tree is imbalanced
    void remove(); // tests normal/edge/error remove
    void removeRebalance(); // tests if remove is balanced
    void insertLarge(); // tests if a large number of nodes inserted works properly
    void removeLarge(); // tests if a large number of nodes removed works properly
    void removeDead(); // tests if remove dead functions properly
    void findDead(bool &, Tiger *aTiger); // checks to see if any dead are in the tree
    void countTigerCubs(); // tests if count tiger cubs works properly
    void countCubs(int &, Tiger *aTiger); // checks if count tiger cubs works properly
    bool checkBSTProperty(Streak& aTree){
        return checkBSTProperty(aTree.m_root);
    }
    bool checkBSTProperty(Tiger *aTiger); // checks if bst properties are kept during insertion and removal
    void insertTime(); // time complexity of insertion
    void removeTime(); // time complexity of remove
};

int main(){
    // tester calls
    Tester tester;
    tester.insert();
    tester.rebalance();
    tester.remove();
    tester.removeRebalance();
    tester.insertLarge();
    tester.removeLarge();
    tester.removeDead();
    tester.countTigerCubs();
    tester.insertTime();
    tester.removeTime();

    return 0;
}

// function tests different cases of insert to ensure they work properly
void Tester::insert() {
    // generates random ID, age, and gender
    Random idGen(MINID,MAXID);
    Random ageGen(0,2);
    Random genderGen(0,2);
    {
        // creating the tree
        Streak streak;
        int streakSize = 10;
        int ID = 0;

        // inserting the nodes
        for(int i=0;i<streakSize;i++){
            ID = idGen.getRandNum();
            Tiger tiger(ID,
                        static_cast<AGE>(ageGen.getRandNum()),
                        static_cast<GENDER>(genderGen.getRandNum()));
            streak.insert(tiger);
        }

        // checking if tree is imbalanced, if the BST property is kept, and if inserted tree correctly
        // normal case
        bool imbalanced = false;
        imbalanceCheck(imbalanced, streak.m_root);
        bool insert = true;
        insertTest(insert, streak.m_root);
        if (insert && !imbalanced && checkBSTProperty(streak)){
            cout << "INSERT NORMAL PASSED" << endl;
        }else{
            cout << "INSERT NORMAL FAILED" << endl;
        }
    }


    // creating a second tree
    Streak streak2;
    int streakSize = 4;
    int counter = 58000;

    // inserting the nodes
    for (int i = 0; i < streakSize; i++){
        int id = counter + i*1000;
        Tiger tiger(id,
                    static_cast<AGE>(ageGen.getRandNum()),
                    static_cast<GENDER>(genderGen.getRandNum()));
        streak2.insert(tiger);
    }

    // checking if tree is imbalanced, if the BST property is kept, and if inserted tree correctly
    // edge case of inserting 4 nodes
    bool imbalanced = false;
    imbalanceCheck(imbalanced, streak2.m_root);
    bool insert = true;
    insertTest(insert, streak2.m_root);
    if (insert && !imbalanced && checkBSTProperty(streak2)){
        cout << "INSERT EDGE1 PASSED" << endl;
    }else{
        cout << "INSERT EDGE1 FAILED" << endl;
    }


    // creating third tree
    Streak streak3;
    streakSize = 3;
    counter = 58000;

    // inserting the nodes
    for (int i = 0; i < streakSize; i++){
        int id = counter + i*1000;
        Tiger tiger(id,
                    static_cast<AGE>(ageGen.getRandNum()),
                    static_cast<GENDER>(genderGen.getRandNum()));
        streak3.insert(tiger);
    }

    // checking if tree is imbalanced, if the BST property is kept, and if inserted tree correctly
    // edge case of inserting 3 nodes
    imbalanced = false;
    imbalanceCheck(imbalanced, streak3.m_root);
    insert = true;
    insertTest(insert, streak3.m_root);
    if (insert && !imbalanced && checkBSTProperty(streak3)){
        cout << "INSERT EDGE2 PASSED" << endl;
    }else{
        cout << "INSERT EDGE2 FAILED" << endl;
    }


    // creating fourth tree
    Streak streak4;
    streakSize = 4;

    // inserting the nodes
    for(int i=0;i<streakSize;i++){
        int ID = idGen.getRandNum();
        Tiger tiger(ID,
                    static_cast<AGE>(ageGen.getRandNum()),
                    static_cast<GENDER>(genderGen.getRandNum()));
        streak4.insert(tiger);
    }

    // inserting additional edge case nodes (values which are MINID/MAXID, etc.
    streak4.insert(99998);
    streak4.insert(10000);
    streak4.insert(99999);

    // checking if tree is imbalanced, if the BST property is kept, and if inserted tree correctly
    imbalanced = false;
    imbalanceCheck(imbalanced, streak4.m_root);
    insert = true;
    insertTest(insert, streak4.m_root);
    if (insert && !imbalanced && checkBSTProperty(streak4)){
        cout << "INSERT EDGE3 PASSED" << endl;
    }else{
        cout << "INSERT EDGE3 FAILED" << endl;
    }


    // creating fifth tree
    Streak streak5;
    // inserting error values
    streak5.insert(999);
    streak5.insert(563782939);
    streakSize = 6;
    counter = 63000;

    // inserting valid nodes
    for (int i = 0; i < streakSize; i++){
        int id = counter - i*1000;
        Tiger tiger(id,
                    static_cast<AGE>(ageGen.getRandNum()),
                    static_cast<GENDER>(genderGen.getRandNum()));
        streak5.insert(tiger);
    }

    // inserting duplicate
    streak5.insert(63000);

    // checking if tree is imbalanced, if the BST property is kept, and if inserted tree correctly as well as duplicate
    // values
    // error case
    imbalanced = false;
    imbalanceCheck(imbalanced, streak5.m_root);
    insert = true;
    insertTest(insert, streak5.m_root);
    if (streak5.findTiger(999)){
        cout << "INSERT ERROR FAILED" << endl;
    }
    if (streak5.findTiger(563782939)){
        cout << "INSERT ERROR FAILED" << endl;
    }

    int count = 0;
    duplicate(count, 58000, streak5.m_root);

    if (!streak5.findTiger(999) && !streak5.findTiger(563782939) && insert && count < 2
    && !imbalanced && checkBSTProperty(streak5)){
        cout << "INSERT ERROR PASSED" << endl;
    }
}

//  checks if values after insertion are where they are supposed to be
void Tester::insertTest(bool &insert, Tiger *aTiger) {
    // runs if aTiger is not null so there isn't seg fault
    if (aTiger != nullptr){
        // recursive call to aTiger's left
        insertTest(insert, aTiger->getLeft());
        // checks if aTiger's left is less than aTiger and if aTiger's right is greater than aTiger
        // if it is not, becomes false
        if (aTiger->getLeft() && !aTiger->getRight()) {
            if (aTiger->getID() < aTiger->getLeft()->getID())
                insert = false;
        }else if (!aTiger->getLeft() && aTiger->getRight()){
            if (aTiger->getID() > aTiger->getRight()->getID())
                insert = false;
        }else if (aTiger->getLeft() && aTiger->getRight())
            if (aTiger->getID() < aTiger->getLeft()->getID() && aTiger->getID() > aTiger->getRight()->getID()){
                insert = false;
            }
        // recursive call to aTiger's right
        insertTest(insert, aTiger->getRight());
    }
}

// checks for duplicates in a tree
void Tester::duplicate(int &counter, int id, Tiger *aTiger) {
    // checks if aTiger is not null else will seg fault if trying to access variable members
    if (aTiger != nullptr){
        // recursive call to aTiger's left
        duplicate(counter, id, aTiger->getLeft());
        // if aTiger exists, increments counter
        if (aTiger->getID() == id){
            counter++;
        }
        // recursive call to aTiger's right
        duplicate(counter, id, aTiger->getRight());
    }
}

// checks if rebalance works properly
void Tester::rebalance() {
    // creating a tree
    Random idGen(MINID,MAXID);
    Random ageGen(0,2);
    Random genderGen(0,2);
    {
        Streak streak;
        int streakSize = 300;
        int ID = 0;
        int tempID = 0;

        // inserting the nodes
        for(int i=0;i<streakSize;i++){
            ID = idGen.getRandNum();
            if (i == streakSize / 2) tempID = ID;
            Tiger tiger(ID,
                        static_cast<AGE>(ageGen.getRandNum()),
                        static_cast<GENDER>(genderGen.getRandNum()));
            streak.insert(tiger);
        }

        // removing a node and checking if the tree is balanced/inserted correctly/has BST properties
        streak.remove(tempID);
        bool imbalanced = false;
        imbalanceCheck(imbalanced, streak.m_root);
        bool insert = true;
        insertTest(insert, streak.m_root);
        if (!imbalanced && insert && checkBSTProperty(streak)){
            cout << "REBALANCE PASSED" << endl;
        }else{
            cout << "REBALANCE FAILED" << endl;
        }
    }
}

// checks imbalance of current node
int Tester::checkImbalance(Tiger *aTiger) {
    // if aTiger is a leaf node, return 0
    if (aTiger->getLeft() == nullptr && aTiger->getRight() == nullptr){
        return 0;
    }

    // if a tiger has a right child, return -1-height of right child
    if (aTiger->getLeft() == nullptr && aTiger->getRight() != nullptr){
        return -1-aTiger->getRight()->getHeight();
    }

    // if a tiger has left child, return height of left child - (-1)
    if (aTiger->getLeft() != nullptr && aTiger->getRight() == nullptr){
        return aTiger->getLeft()->getHeight()-(-1);
    }

    // if aTiger has both children, return difference between left and right child
    return aTiger->getLeft()->getHeight()-aTiger->getRight()->getHeight();
}

// checks for imbalances through the tree
void Tester::imbalanceCheck(bool &imbalanced, Tiger *aTiger) {
    // checks if aTiger is null, otherwise will seg fault if trying to access null member variables
    if (aTiger != nullptr){
        // recurse through left child
       imbalanceCheck(imbalanced,aTiger->getLeft());
       // checks balance of current tiger
       int imbalance = checkImbalance(aTiger);
       // if it is imbalanced, that means the tree is not balanced
       if (imbalance > 1 || imbalance < -1){
           imbalanced = true;
           return;
       }
       // recurse through right child
        imbalanceCheck(imbalanced,aTiger->getRight());
    }
}

// tests remove normal/edge/error cases
void Tester::remove() {
    // creates a tree
    Random idGen(MINID,MAXID);
    Random ageGen(0,2);
    Random genderGen(0,2);
    {
        Streak streak;
        int streakSize = 10;
        int tempID = 0;
        int ID = 0;

        // inserts nodes
        for(int i=0;i<streakSize;i++){
            ID = idGen.getRandNum();
            if (i == streakSize / 2) tempID = ID;//we store this ID for later use
            Tiger tiger(ID,
                        static_cast<AGE>(ageGen.getRandNum()),
                        static_cast<GENDER>(genderGen.getRandNum()));
            streak.insert(tiger);
        }

        // removes a node and checks if tree is balanced/inserted correctly/node is truly deleted/if BST properties
        // are kept
        // normal case, removing leaf node
        streak.remove(tempID);
        bool imbalanced = false;
        imbalanceCheck(imbalanced, streak.m_root);
        bool insert = true;
        insertTest(insert, streak.m_root);
        if (!streak.findTiger(tempID) && insert && !imbalanced && checkBSTProperty(streak)){
            cout << "REMOVE NORMAL1 PASSED" << endl;
        }else{
            cout << "REMOVE NORMAL1 FAILED" << endl;
        }
    }


    // creating second tree
    Streak streak2;
    int streakSize = 12;
    int counter = 58000;

    // inserting nodes
    for (int i = 0; i < streakSize; i++){
        int id = counter + i*1000;
        Tiger tiger(id,
                    static_cast<AGE>(ageGen.getRandNum()),
                    static_cast<GENDER>(genderGen.getRandNum()));
        streak2.insert(tiger);
    }

    // removing a node and checking if tree is still balanced/inserted correctly/node is truly deleted/BST
    // properties are kept after deletion
    // normal case
    streak2.remove(61000);
    bool imbalanced = false;
    imbalanceCheck(imbalanced, streak2.m_root);
    bool insert = true;
    insertTest(insert, streak2.m_root);
    if (!streak2.findTiger(61000) && insert && !imbalanced && checkBSTProperty(streak2)){
        cout << "REMOVE NORMAL2 PASSED" << endl;
    }else{
        cout << "REMOVE NORMAL2 FAILED" << endl;
    }


    // creating third tree
    Streak streak3;
    streakSize = 10;
    int ID = 0;

    // inserting nodes
    for(int i=0;i<streakSize;i++){
        ID = idGen.getRandNum();
        Tiger tiger(ID,
                    static_cast<AGE>(ageGen.getRandNum()),
                    static_cast<GENDER>(genderGen.getRandNum()));
        streak3.insert(tiger);
    }

    // removing node with one child and checking if tree is still balanced/inserted correctly/node is truly deleted/BST
    // properties are kept after remove
    // normal case
    streak3.remove(33900);
    imbalanced = false;
    imbalanceCheck(imbalanced, streak3.m_root);
    insert = true;
    insertTest(insert, streak3.m_root);
    if (!streak3.findTiger(33900) && insert && !imbalanced && checkBSTProperty(streak3)){
        cout << "REMOVE NORMAL3 PASSED" << endl;
    }else{
        cout << "REMOVE NORMAL3 FAILED" << endl;
    }


    // creating fourth tree and inserting nodes
    Streak streak4;
    streak4.insert(54513);
    streak4.insert(36888);
    streak4.insert(54865);
    streak4.insert(11867);
    streak4.insert(40000);
    streak4.insert(54600);
    streak4.insert(54650);
    streak4.insert(54650);
    streak4.insert(54660);
    streak4.insert(39000);
    streak4.insert(39500);

    // removing the root and checking if tree is still balanced/inserted properly/node is truly deleted/BST properties
    // are kept after remove
    // normal case
    streak4.remove(54513);
    imbalanced = false;
    imbalanceCheck(imbalanced, streak4.m_root);
    insert = true;
    insertTest(insert, streak4.m_root);
    if (!streak4.findTiger(54513) && insert && !imbalanced && checkBSTProperty(streak4)){
        cout << "REMOVE NORMAL4 PASSED" << endl;
    }else{
        cout << "REMOVE NORMAL4 FAILED" << endl;
    }


    // creating fifth tree and checking to see if removing only node in tree works
    // edge case
    Streak streak5;
    streak5.insert(12345);
    streak5.remove(12345);
    if (streak5.m_root == nullptr){
        cout << "REMOVE EDGE1 PASSED" << endl;
    }else{
        cout << "REMOVE EDGE1 FAILED" << endl;
    }


    // creating sixth tree
    Streak streak6;
    // inserting nodes
    streak6.insert(54513);
    streak6.insert(36888);
    streak6.insert(54865);
    streak6.insert(11867);
    streak6.insert(40000);
    streak6.insert(39000);

    // removing a node and checking if the tree is still balanced/inserted properly/node is truly deleted/BST properties
    // are kept after remove
    // edge case
    streak6.remove(54865);
    imbalanced = false;
    imbalanceCheck(imbalanced, streak6.m_root);
    insert = true;
    insertTest(insert, streak6.m_root);
    if (!streak6.findTiger(54865) && insert && !imbalanced && checkBSTProperty(streak6)){
        cout << "REMOVE EDGE2 PASSED" << endl;
    }else{
        cout << "REMOVE EDGE2 FAILED" << endl;
    }


    // creating seventh tree
    Streak streak7;
    // inserting nodes
    streak7.insert(54513);
    streak7.insert(36888);
    streak7.insert(54865);
    streak7.insert(11867);
    streak7.insert(40000);
    streak7.insert(39000);

    // removing a node and checking if tree is still balanced/inserted properly/node is truly deleted/BST property is
    // kept after remove
    // edge case
    streak7.remove(40000);
    imbalanced = false;
    imbalanceCheck(imbalanced, streak7.m_root);
    insert = true;
    insertTest(insert, streak7.m_root);
    if (insert && !imbalanced
    && !streak7.findTiger(40000) && checkBSTProperty(streak7)){
        cout << "REMOVE EDGE3 PASSED" << endl;
    }else{
        cout << "REMOVE EDGE3 FAILED" << endl;
    }
}

// checks if tree is rebalanced properly after removing a number of nodes
void Tester::removeRebalance() {
    // creating a tree
    Random idGen(MINID,MAXID);
    Random ageGen(0,2);
    Random genderGen(0,2);
    Streak streak;
    int streakSize = 20;
    int counter = 58000;

    // inserting nodes
    for (int i = 0; i < streakSize; i++){
        int id = counter + i*1000;
        Tiger tiger(id,
                    static_cast<AGE>(ageGen.getRandNum()),
                    static_cast<GENDER>(genderGen.getRandNum()));
        streak.insert(tiger);
    }

    // removing multiple nodes from the tree and checking if they no longer exist in the tree
    streak.remove(65000);
    bool found = streak.findTiger(65000);
    for (int i = 0; i < 6; i++){
        streak.remove(counter+i*1000);
        found = streak.findTiger(counter+i*1000);
    }

    // also checks if after removal the tree is balanced/was inserted properly/BST properties were kept
    bool imbalanced = false;
    imbalanceCheck(imbalanced, streak.m_root);
    bool insert = true;
    insertTest(insert, streak.m_root);
    if (!found && insert && !imbalanced && checkBSTProperty(streak)){
        cout << "REMOVE REBALANCE PASSED" << endl;
    }else{
        cout << "REMOVE REBALANCE FAILED" << endl;
    }
}

// tests if tree is balanced after a large number of insertions
void Tester::insertLarge() {
    // creating tree
    Random idGen(MINID,MAXID);
    Random ageGen(0,2);
    Random genderGen(0,2);
    {
        Streak streak;
        int streakSize = 1000;
        int ID = 0;

        // inserting nodes
        for(int i=0;i<streakSize;i++){
            ID = idGen.getRandNum();
            Tiger tiger(ID,
                        static_cast<AGE>(ageGen.getRandNum()),
                        static_cast<GENDER>(genderGen.getRandNum()));
            streak.insert(tiger);
        }

        // checks if tree is balanced/inserted properly/BST properties are kept after insertion
        bool imbalanced = false;
        imbalanceCheck(imbalanced, streak.m_root);
        bool insert = true;
        insertTest(insert, streak.m_root);
        if (insert && !imbalanced && checkBSTProperty(streak)){
            cout << "INSERT LARGE PASSED" << endl;
        }else{
            cout << "INSERT LARGE FAILED" << endl;
        }
    }
}

// checks if tree is balanced after a large number of removals
void Tester::removeLarge() {
    // creating a tree
    Random idGen(MINID,MAXID);
    Random ageGen(0,2);
    Random genderGen(0,2);
    Streak streak;
    int streakSize = 100;
    int counter = 10000;

    // inserting nodes
    for (int i = 0; i < streakSize; i++){
        int id = counter + i*1000;
        Tiger tiger(id,
                    static_cast<AGE>(ageGen.getRandNum()),
                    static_cast<GENDER>(genderGen.getRandNum()));
        streak.insert(tiger);
    }

    // removing multiple nodes and checking if nodes still exist in the tree
    streak.remove(41000);
    bool found = streak.findTiger(41000);
    for (int i = 0; i < 10; i++){
        streak.remove(counter+i*1000);
        found = streak.findTiger(counter+i*1000);
    }

    // checks if tree is still balanced/inserted properly/BST properties are kept after removal
    bool imbalanced = false;
    imbalanceCheck(imbalanced, streak.m_root);
    bool insert = true;
    insertTest(insert, streak.m_root);
    if (!found && insert && !imbalanced && checkBSTProperty(streak)){
        cout << "REMOVE LARGE PASSED" << endl;
    }else{
        cout << "REMOVE LARGE FAILED" << endl;
    }
}

// tests if all dead tigers are removed
void Tester::removeDead() {
    // creates a tree
    Random idGen(MINID,MAXID);
    Random ageGen(0,2);
    Random genderGen(0,2);
    Random stateGen(0, 1);
    Streak streak;
    int streakSize = 20;
    int counter = 58000;

    // inserts nodes
    for (int i = 0; i < streakSize; i++){
        int id = counter + i*1000;
        Tiger tiger(id,
                    static_cast<AGE>(ageGen.getRandNum()),
                    static_cast<GENDER>(genderGen.getRandNum()),
                    static_cast<STATE>(stateGen.getRandNum()));
        streak.insert(tiger);
    }

    // removes all dead tigers and checks if they are all removed. then, checks balance of the tree, insertion,
    // and the BST properties of the tree
    streak.removeDead();
    bool dead = false;
    findDead(dead, streak.m_root);
    bool imbalanced = false;
    imbalanceCheck(imbalanced, streak.m_root);
    bool insert = true;
    insertTest(insert, streak.m_root);
    if (!dead && insert && !imbalanced && checkBSTProperty(streak)){
        cout << "REMOVE DEAD PASSED" << endl;
    }else{
        cout << "REMOVE DEAD FAILED" << endl;
    }

}


// checks if there are any dead in tree
void Tester::findDead(bool &dead, Tiger *aTiger) {
    // checks if aTiger is not null to prevent seg fault from accessing null members
    if (aTiger != nullptr){
        // recurse to aTiger's left
        findDead(dead, aTiger->getLeft());
        // if aTiger is dead
        if (aTiger->getState() == 1){
            dead = true;
            return;
        }
        // recurse to aTiger's right
        findDead(dead, aTiger->getRight());
    }
}


// tests how many cubs are in a tree
void Tester::countTigerCubs() {
    // creates a tree
    Random idGen(MINID,MAXID);
    Random ageGen(0,2);
    Random genderGen(0,2);
    Random stateGen(0, 1);
    Streak streak;
    int streakSize = 20;
    int id = 0;

    // inserts nodes
    for (int i = 0; i < streakSize; i++){
        id = idGen.getRandNum();
        Tiger tiger(id,
                    static_cast<AGE>(ageGen.getRandNum()),
                    static_cast<GENDER>(genderGen.getRandNum()),
                    static_cast<STATE>(stateGen.getRandNum()));
        streak.insert(tiger);
    }

    // counts cubs and sees if the number is correct
    int count = 0;
    countCubs(count, streak.m_root);
    int count2 = streak.countTigerCubs();

    // checks if tree is balanced/inserted properly/BST properties are kept
    bool imbalanced = false;
    imbalanceCheck(imbalanced, streak.m_root);
    bool insert = true;
    insertTest(insert, streak.m_root);
    if (count == count2 && insert && !imbalanced && checkBSTProperty(streak)){
        cout << "COUNT TIGER CUBS PASSED" << endl;
    }else{
        cout << "COUNT TIGER CUBS FAILED" << endl;
    }
}


// checks how many cubs are in a tree
void Tester::countCubs(int &total, Tiger *aTiger) {
    // checks if a Tiger is not null else will segfault due to null member variable access
    if (aTiger != nullptr){
        // recurse to aTiger's left
        countCubs(total, aTiger->getLeft());
        // if it a cub; increment total
        if (aTiger->getAge() == 0){
            total++;
        }
        // recurse to aTiger's right
        countCubs(total, aTiger->getRight());
    }
}

// checks BST property of tree
bool Tester::checkBSTProperty(Tiger* aNode){
    bool result = true;
    // this function performs pre-order traversal
    // first we visit the node
    // protection for empty root
    if (aNode == nullptr ||
        // leaf node
        ((aNode->getLeft() == nullptr) && (aNode->getRight() == nullptr)))
        result = true;
        // the case of a node with only right child
    else if (aNode->getLeft() == nullptr && aNode->getRight() != nullptr){
        result = result &&
                 (aNode->getID() < aNode->getRight()->getID());
    }
        // the case of a node with only left child
    else if (aNode->getRight() == nullptr && aNode->getLeft() != nullptr){
        result = result &&
                 (aNode->getID() > aNode->getLeft()->getID());
    }
        // the case of a node with two children
    else{
        result = result &&
                 (aNode->getID() > aNode->getLeft()->getID()) &&
                 (aNode->getID() < aNode->getRight()->getID());
    }
    // recurse to aTiger's left
    if (aNode->getLeft() != nullptr)
        result = result && checkBSTProperty(aNode->getLeft());
    // recurse to aTiger's right
    if (aNode->getRight() != nullptr)
        result = result && checkBSTProperty(aNode->getRight());
    // return result
    return result;
}

// checks time complexity for insertion time (if it is accepted)
void Tester::insertTime() {
    // creating a tree of 1000 nodes and getting the start time and end time of insertion
    Streak streak;

    auto startTime = clock();
    for (int i = 0; i < 1000; i++) {
        Tiger tiger(MINID+i);
        streak.insert(tiger);
    }
    // getting T1 and clearing the tree
    auto endTime = clock();
    double T1 = endTime - startTime;
    streak.clear();

    // getting T2, 2000 nodes
    startTime = clock();
    for (int i = 0; i < 2000; i++) {
        Tiger tiger(MINID+i);
        streak.insert(tiger);
    }
    endTime = clock();
    double T2 = endTime - startTime;
    streak.clear();

    // T3, 3000 nodes
    startTime = clock();
    for (int i = 0; i < 3000; i++) {
        Tiger tiger(MINID+i);
        streak.insert(tiger);
    }
    endTime = clock();
    double T3 = endTime - startTime;
    streak.clear();

    // calculating the ratio
    double ratio = 1000*log2(1000);

    // calculating expected ratio of current insertions
    int n = 2000;
    double expectedRatio = (n*log2(n))/ratio;

    // calculating my ratio and printing values
    double myRatio = T2/T1;
    cout << "This is the expected ratio for insert (n=2000): " << expectedRatio << endl;
    cout << "T2: " << T2 << ". T1: " << T1  << "." << endl;
    cout << "This is my ratio: " << myRatio << endl;

    // checking if my ratio is acceptable
    if ((expectedRatio-0.4) < (myRatio) && (myRatio) < (expectedRatio+0.4)){
        cout << "acceptable ratio" << endl;
    }

    // calculating expected ratio of current insertions
    n = 3000;
    expectedRatio = (n*log2(n))/ratio;

    // calculating my ratio and printing values
    myRatio = T3/T1;
    cout << "This is the expected ratio for insert (n=3000): " << expectedRatio << endl;
    cout << "T3: " << T3 << ". T1: " << "." << T1 << endl;
    cout << "This is my ratio: " << myRatio << endl;

    // checking if my ratio is acceptable
    if ((expectedRatio-0.4) < (myRatio) && (myRatio) < (expectedRatio+0.4)){
        cout << "acceptable ratio" << endl;
    }
}


// checking time complexity of remove
void Tester::removeTime() {
    // creating a tree
    Streak streak;

    // getting T1, removal of 1000 nodes
    for (int i = 0; i < 1000; i++) {
        Tiger tiger(MINID + i);
        streak.insert(tiger);
    }
    auto startTime = clock();
    for (int i = 0; i < 1000; i++) {
        streak.remove(MINID + i);
    }
    auto endTime = clock();
    double T1 = endTime - startTime;
    streak.clear();

    // getting T2, removal of 2000 nodes
    for (int i = 0; i < 2000; i++) {
        Tiger tiger(MINID + i);
        streak.insert(tiger);
    }
    startTime = clock();
    for (int i = 0; i < 2000; i++) {
        streak.remove(MINID + i);
    }
    endTime = clock();
    double T2 = endTime - startTime;
    streak.clear();

    // getting T3, removal of 3000 nodes
    for (int i = 0; i < 3000; i++) {
        Tiger tiger(MINID + i);
        streak.insert(tiger);
    }
    startTime = clock();
    for (int i = 0; i < 3000; i++) {
        streak.remove(MINID + i);
    }
    endTime = clock();
    double T3 = endTime - startTime;
    streak.clear();

    // getting ratio and expected ratio for n = 2000
    double ratio = 1000 * log2(1000);

    int n = 2000;
    double expectedRatio = (n * log2(n)) / ratio;

    // getting my ratio and printing values
    double myRatio = T2 / T1;
    cout << "This is the expected ratio for remove (n=2000): " << expectedRatio << endl;
    cout << "T2: " << T2 << ". T1: " << T1 << "." << endl;
    cout << "This is my ratio: " << myRatio << endl;

    // checking if my ratio is acceptable
    if ((expectedRatio - 0.4) < (myRatio) && (myRatio) < (expectedRatio + 0.4)) {
        cout << "acceptable ratio" << endl;
    }

    // getting ratio and expected ratio for n = 3000
    n = 3000;
    expectedRatio = (n * log2(n)) / ratio;

    // getting my ratio and printing values
    myRatio = T3 / T1;
    cout << "This is the expected ratio for remove (n=3000): " << expectedRatio << endl;
    cout << "T3: " << T3 << ". T1: " << T1 << "." << endl;
    cout << "This is my ratio: " << myRatio << endl;

    // checking if my ratio is acceptable
    if ((expectedRatio - 0.4) < (myRatio) && (myRatio) < (expectedRatio + 0.4)) {
        cout << "acceptable ratio" << endl;
    }
}
