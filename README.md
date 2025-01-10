
# README: Project Overview and Usage

## **Project Description**
This project consists of a binary search tree (BST) implementation, referred to as a **Streak**, with functionality to manage a collection of objects (Tigers). The primary goal is to efficiently manage insertions, deletions, rebalancing, and state management within the tree, while ensuring correct binary search properties and AVL balancing rules.

## **File Descriptions**

1. **`streak.h`**
   - Defines the core data structures and public/private methods for the `Streak` and `Tiger` classes.
   - **Key Classes:**
     - `Tiger`: Represents nodes in the BST, with attributes like ID, age, gender, and state.
     - `Streak`: Manages the BST structure with methods for insertion, deletion, rebalancing, and traversal.
   - **Key Features:**
     - AVL balancing through rotations (`singleLeft`, `singleRight`, `leftRight`, `rightLeft`).
     - Management of specific node properties (e.g., counting tiger cubs, removing dead nodes).
     - Recursive traversal methods for tree operations.

2. **`streak.cpp`**
   - Implements all the methods declared in `streak.h`.
   - **Key Implementations:**
     - `insert`: Adds nodes while maintaining AVL properties.
     - `remove`: Deletes nodes, with specialized cases for nodes with zero, one, or two children.
     - `rebalance`: Ensures the tree stays balanced after insertions/deletions.
     - `clear`: Frees all memory by deleting nodes in a post-order traversal.
     - Auxiliary operations such as `findDead` (removing dead nodes) and `countTigerCubs`.

3. **`mytest.cpp`**
   - A comprehensive test suite that validates the functionality of the `Streak` class.
   - **Tested Scenarios:**
     - Insertion, removal, rebalancing, and edge cases for BST operations.
     - Checking BST properties (e.g., duplicate prevention, height balance).
     - Performance benchmarks for insertion and removal with timing comparisons against theoretical expectations.
   - **Example Outputs:**
     - Tests like `INSERT NORMAL PASSED`, `REMOVE LARGE PASSED`, and time complexity validation with acceptable ratios.

4. **`makefile`**
   - Automates the build process for the project using `make`.
   - Includes compilation instructions for `mytest.cpp`, linking it with `streak.cpp` and the header file.

---

## **Compilation and Usage**

1. **Build the Project**
   - Run the following command to compile the test program:
     ```bash
     make
     ```
   - This will generate an executable `mytest`.

2. **Run the Test Suite**
   - Execute the test program using:
     ```bash
     ./mytest
     ```
   - If `valgrind` is available, memory leaks and other issues can be checked:
     ```bash
     valgrind ./mytest
     ```

3. **Expected Output**
   - The program validates all scenarios with outputs like:
     - `"INSERT NORMAL PASSED"`
     - `"REBALANCE PASSED"`
     - `"COUNT TIGER CUBS PASSED"`
   - Additionally, time complexity checks provide ratios between insertion/removal scenarios for performance evaluation.

---

## **Code Highlights**

- **Balanced Tree Implementation:** Ensures AVL balance with efficient rotation mechanisms.
- **Extensive Error Handling:** Prevents duplicates, invalid IDs, and handles edge cases gracefully.
- **Performance Validation:** Uses theoretical comparisons (`n*log2(n)`) to ensure operations meet expected complexity.

---

## **Developer Notes**

- **Customization:**
  - Modify constants like `MINID` and `MAXID` in `streak.h` for testing with different ID ranges.
- **Debugging:**
  - Use `dumpTree()` for a detailed view of the current tree structure.
- **Extensions:**
  - Add functionality to handle additional states or properties for Tigers if needed.

---

## **Known Limitations**
- The test suite assumes fixed seed randomization for reproducibility. Adjust the `Random` class seed in `mytest.cpp` if true randomness is desired.

---

This README provides a comprehensive overview of the project, helping developers and users understand its structure, purpose, and functionality. For questions or contributions, refer to the developer's contact information in the file headers.
