# Text Editor - Using Piece Table & AVL Tree

A **high-performance text editor** built using C++ and Qt, designed to handle large files efficiently with **instant editing** and **smooth cursor movement**.
At its core, it uses a **Piece Table** integrated with an **AVL Tree**, providing extremely fast insertions and deletions without copying large amounts of text.

---

## Key Features

* Fast, memory-efficient text editing
* Handles large files easily
* Undo / Redo functionality
* Open / Save functionality
* Keyboard-based navigation (arrows, Enter, Backspace)
* Mouse cursor movement with live updates
* Lightweight UI built using Qt

---

## Core Data Structure: Piece Table + AVL Tree

### Piece Table

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

## Comparison with Other Data Structures

| Operation                 | Array Buffer | Gap Buffer  | Rope Tree | **Piece Table + AVL Tree (This Project)** |
| ------------------------- | ------------ | ----------- | --------- | ----------------------------------------- |
| Insert in middle          | O(n)         | O(n) amort. | O(log n)  | **O(log n)**                              |
| Delete in middle          | O(n)         | O(n) amort. | O(log n)  | **O(log n)**                              |
| Memory usage              | High         | High        | Moderate  | **Low (metadata only)**                   |
| Implementation complexity | Low          | Moderate    | High      | **Moderate, but efficient**               |
| Large file performance    |  Poor        | Medium      | Good      | **Excellent**                             |

---

## Undo / Redo Support

The editor uses **two stacks** for undo and redo operations:  
one for tracking edits (undo stack) and another for reverted actions (redo stack).  
Each insert/delete pushes metadata onto the stacks, enabling undo/redo with full edit history.
Each undo/redo step internally performs AVL-based insert/delete operations, resulting in **O(k log n)** for reversing k edits.

> Simple stack-based design ensures fast, reliable undo/redo without affecting performance.

---

## Build Instructions

### Requirements

* Qt 6+ (for GUI)
* C++17 or newer

### Steps

In Command Palette (Ctrl + Shift + P) run these commands:
```
CMake: Build
Cmake: Run Without Debugging
```

---

## File Overview

| File                  | Description                                    |
| --------------------- | ---------------------------------------------- |
| `PieceTable.h / .cpp` | Core data structure & AVL logic for editing    |
| `helper.cpp`          | AVL tree balancing and utility functions       |
| `mainwindow.cpp / .h` | Qt-based editor interface and event handling   |
| `main.cpp`            | Entry point of the Qt application              |
| `test.cpp`            | Command-line tester for Piece Table operations |

---

## Performance Highlights

* Large files are represented compactly through references — not copied.
* Edits modify only metadata (pointers and lengths).
* Cursor movements and inserts are logarithmic in complexity.
* The AVL structure ensures consistent speed even after thousands of edits.

---

## Time Complexity Summary

| Operation   | Complexity | Description                         |
|-------------|------------|-------------------------------------|
| Insert      | O(log n)   | Balanced AVL insertion              |
| Delete      | O(log n)   | Weight rebalancing on deletion      |
| Undo/Redo   | O(k log n) | Reverts k edits using AVL operations|
| Cursor Move | O(log n)   | Index-based navigation              |
| Rendering   | O(V × H)   | Visible lines × characters          |

---

## Why This Approach is Optimal

- Unlike traditional buffer-based editors that rewrite large chunks of text,  
  this editor only **modifies structural metadata** (Piece Table + AVL nodes).
- **No character copying** on insert/delete operations.
- **Balanced tree indexing** ensures predictable logarithmic performance.
- Scales linearly with file size, while maintaining constant-time user interaction.

> In short — this editor achieves **modern IDE-level responsiveness** using pure **DSA principles**.

---
