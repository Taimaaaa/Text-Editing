# Text Editing with Undo/Redo Using Stacks and Queues in C

## Project Overview
This project implements a **text editor** that provides basic text manipulation features such as inserting and removing text, with the ability to perform **undo** and **redo** operations. It is implemented using **stacks** and **queues** in C, ensuring efficient management of changes and their order.

### Features
- **Stacks for Undo/Redo**: 
  - The project uses two stacks: one for undo operations and one for redo operations.
  - Users can undo the most recent changes and redo them if necessary.
- **Queue for Managing Text**:
  - Text is managed in a queue structure for efficient sequential processing.
- **Original String Manipulation**:
  - The program maintains an original string using a doubly linked list, allowing modifications like insertions and deletions at different points in the text.
- **File Operations**:
  - Load a string from a file and save the modified string back to a file.

### Data Structures Used
- **Stacks**: Used to store operations for undo and redo functionality.
- **Queues**: Manage the text being manipulated.
- **Doubly Linked List**: Used to store and manage the original string with pointers to efficiently navigate and modify the string.

### Technologies
- **Language**: C
- **Data Structures**:
  - Stack
  - Queue
  - Doubly Linked List

### How to Use
1. **Compile the Program**: Ensure you have a C compiler installed. Use the following command to compile the program:
    ```bash
    gcc -o text_editor P2_1222640_TaymaaNasser_5.c
    ```
2. **Run the Program**: After compilation, run the program:
    ```bash
    ./text_editor
    ```
3. **Menu Options**: The program presents a menu to perform various operations, including:
   - Insert text
   - Remove text
   - Undo the last operation
   - Redo an undone operation
   - Load or save a file

### Future Improvements
- Add support for more complex text editing commands.
- Fix Undo and Redo functions
- Implement more robust error handling.
