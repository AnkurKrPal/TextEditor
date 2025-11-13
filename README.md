# Superfast Text Editor — Powered by Piece Table + AVL Tree

A **high-performance text editor** built using C++ and Qt, designed to handle large files efficiently with **instant editing** and **smooth cursor movement**.
At its core, it uses a **Piece Table** integrated with an **AVL Tree**, providing extremely fast insertions and deletions without copying large amounts of text.

---

## Key Features

* Fast, memory-efficient text editing
* Handles large files easily
* Open / Save functionality
* Keyboard-based navigation (arrows, Enter, Backspace)
* Mouse cursor movement with live updates
* Lightweight UI built using Qt

---

## 🧠 Core Data Structure: Piece Table + AVL Tree

### 🧩 Piece Table

A **Piece Table** is a text-editing data structure that avoids moving or rewriting text.
It maintains two buffers:

* **Original buffer** — the file content loaded initially (read-only)
* **Add buffer** — new text appended during editing

Instead of editing buffers directly, it stores **pieces** referencing sections of these buffers.
Each piece keeps track of which buffer it belongs to, and the start and length of that text.

```cpp
struct pieceNode {
    BufferType source;   // ORIGINAL or ADD
    size_t start;        // start index in the buffer
    size_t length;       // number of characters
    pieceNode *left, *right;
    int height;  // AVL balancing field
    int weight;  // Number of characters in the left subtree
};
```

This approach ensures:

* No large-scale data copying on edits
* Minimal memory usage
* Efficient support for random insertions and deletions

---

### AVL Tree Integration

The Piece Table’s pieces are stored inside a **self-balancing AVL tree**:

* `weight` holds the total number of characters in the left subtree.
* Each insert/delete maintains tree balance via rotations.
* Cursor navigation, insertion, and deletion all operate in **O(log n)** time.

AVL Trees ensure the editor remains efficient even with thousands of edits:

* Keeps the height of the tree logarithmic
* Guarantees stable, predictable performance
* Supports quick traversal to locate the nth character or insertion point

---

## ⚙️ Comparison with Other Data Structures

| Operation                 | Array Buffer | Gap Buffer  | Rope Tree | **Piece Table + AVL Tree (This Project)** |
| ------------------------- | ------------ | ----------- | --------- | ----------------------------------------- |
| Insert in middle          | O(n)         | O(n) amort. | O(log n)  | **O(log n)** ✅                            |
| Delete in middle          | O(n)         | O(n) amort. | O(log n)  | **O(log n)** ✅                            |
| Memory usage              | High         | High        | Moderate  | **Low (metadata only)** ✅                 |
| Implementation complexity | Low          | Moderate    | High      | **Moderate, but efficient** ✅             |
| Large file performance    | ❌ Poor       | ⚠️ Medium   | ✅ Good    | **✅ Excellent**                           |

---

## 🛠️ Build Instructions

### Requirements

* Qt 6+ (for GUI)
* C++17 or newer

### Steps

```bash
mkdir build && cd build
qmake ../
make
./TextEditor
```

---

## 📁 File Overview

| File                  | Description                                    |
| --------------------- | ---------------------------------------------- |
| `PieceTable.h / .cpp` | Core data structure & AVL logic for editing    |
| `helper.cpp`          | AVL tree balancing and utility functions       |
| `mainwindow.cpp / .h` | Qt-based editor interface and event handling   |
| `main.cpp`            | Entry point of the Qt application              |
| `test.cpp`            | Command-line tester for Piece Table operations |

---

## 📈 Performance Highlights

* Large files are represented compactly through references — not copied.
* Edits modify only metadata (pointers and lengths).
* Cursor movements and inserts are logarithmic in complexity.
* The AVL structure ensures consistent speed even after thousands of edits.

---

## Authors

* **Prajwal Patil**
* **Sajidhussain Agharia**
* **Ankur Kumar Pal**

---
