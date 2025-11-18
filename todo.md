# Tetris TODO

1. **Set up working curses loop**
   - [*] Create an init function to run all starting functions.

2. **Represent game state in arrays**
   - [ ] Create 2D array of ints or chars (0 for empty, 1 for filled).
   - [ ] (Optional) Add colors later.
   - [ ] Start with one shape.
   - [ ] Loop over field and put `.` for empty and `#` for filled.
   - [ ] Loop over shapes and draw at `(x, y)`.

3. **Implement movement, gravity, and collision checks**
   - [ ] Every N milliseconds the piece tries to move down by 1.
   - [ ] Create `can_place` function to check if pieces can move.
   - [ ] Create `lock_piece` function for when the piece can't move.
   - [ ] In the main loop, track time and:
     - [ ] Use `can_place` to decide whether to move the piece down.
     - [ ] Otherwise call `lock_piece`.
   - [ ] Add horizontal movement based on `KEY_LEFT` and `KEY_RIGHT`:
     - [ ] Change x value accordingly.

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