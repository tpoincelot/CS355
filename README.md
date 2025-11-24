# CS355
Tetris Project for CS355 coded in C

# Tetris TODO

1. **Set up working curses loop**
   - [x] Create an init function to run all starting functions.

2. **Represent game state in arrays**
   - [x] Create 2D array of ints or chars (0 for empty, 1 for filled).
   - [x] (Optional) Add colors later.
   - [x] Start with one shape.
   - [x] Create a 3D array (or possibly 4D to track piece type) to track rotations of pieces(row, column).
   - [x] Loop over field and put `.` for empty and `#` for filled.
   - [x] Loop over shapes and draw at `(x, y)`.

3. **Implement movement, gravity, and collision checks**
   - [x] Every N milliseconds the piece tries to move down by 1.
   - [x] Create `can_place` function to check if pieces can move.
   - [x] Create `lock_piece` function for when the piece can't move.
   - [x] In the main loop, track time and:
     - [x] Use `can_place` to decide whether to move the piece down.
     - [x] If place can't be moved use a flag to mark it as locked.
   - [x] Add horizontal movement based on `KEY_LEFT` and `KEY_RIGHT`:
     - [x] Change x value accordingly.
   - [x] For any reused code create functions (clear line).

4. **Add line clearing and game over screen**
   - [x] After `lock_piece`, for each row:
     - [x] Check if every cell is 1.
     - [x] Delete the row if so.
     - [x] Move all above rows down 1.
     - [x] Clear the top row.
   - [x] Add a game over screen.

5. **Add rotation**
   - [x] Predefine 4 rotation states for each piece.
   - [x] Update `can_place` and input handling to support rotation.

6. **Add random pieces**
   - [x] Randomly choose piece type when spawning a new piece.
7. **Final touches**
   - [x] Fix spacing of play field.