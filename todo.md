# Tetris TODO

1. **Set up working curses loop**
   - [x] Create an init function to run all starting functions.

2. **Represent game state in arrays**
   - [x] Create 2D array of ints or chars (0 for empty, 1 for filled).
   - [ ] (Optional) Add colors later.
   - [x] Start with one shape.
   - [x] Create a 3D array (or possibly 4D to track piece type) to track rotations of pieces(row, column).
   - [x] Loop over field and put `.` for empty and `#` for filled.
   - [x] Loop over shapes and draw at `(x, y)`.

3. **Implement movement, gravity, and collision checks**
   - [ ] Every N milliseconds the piece tries to move down by 1.
   - [ ] Create `can_place` function to check if pieces can move.
   - [ ] Create `lock_piece` function for when the piece can't move.
   - [ ] In the main loop, track time and:
     - [ ] Use `can_place` to decide whether to move the piece down.
     - [ ] If place can't be moved use a flag to mark it as locked.
   - [ ] Add horizontal movement based on `KEY_LEFT` and `KEY_RIGHT`:
     - [ ] Change x value accordingly.
   - [ ] For any reused code create functions (clear line).

4. **Add line clearing and game over screen**
   - [ ] After `lock_piece`, for each row:
     - [ ] Check if every cell is 1.
     - [ ] Delete the row if so.
     - [ ] Move all above rows down 1.
     - [ ] Clear the top row.
   - [ ] Add a game over screen.

5. **Add rotation**
   - [ ] Predefine 4 rotation states for each piece.
   - [ ] Update `can_place` and input handling to support rotation.

6. **Add random pieces**
   - [ ] Randomly choose piece type when spawning a new piece.
7. **Final touches**
   - [ ] Fix spacing of play field.