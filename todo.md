1. Set up working curses loop.
    a. Create an init function to run all starting functions.
2. Represent game state in arrays.
    a. Create 2d array of ints or chars. 0 for empty 1 for filled. Add colors if possible later. Start with one shape.
    b. Loop over field and put . for empty and # for filled. Loop over shapes and draw at (x,y).
3. Implement movement, gravity, and collision checks.
    a. Every n milliseconds the piece trys to move down by 1. It can't if any of the spaces it needs are filled. Create a can_place function to check if pieces can move. Create lock_piece function for when the piece can't move. In the main loop track time and check can_place and either decrease piece y axis or lock_piece.
    b. Add horizonal movement based on KEY_LEFT and KEY_RIGHT. Change x value accordingly. 
4. Add line clearing and game over screen.
    a. After lock_piece for each row check if every cell is 1 and delete the row if so. Move all above rows down 1. Clear the top row.
5. Add rotation by predefining 4 states for each piece. 
6. Add random pieces to game.