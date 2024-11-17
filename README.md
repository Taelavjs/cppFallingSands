(All recordings done on a 2015 Macbook Pro running OBS. Performance is noticeably worse than playing without OBS, even with efficient recording profile) 

https://github.com/user-attachments/assets/5823077d-109d-4a0c-9096-c8cc71b282ef

This Falling Sands Simulation project showcases a dynamic terrain simulation featuring interactive elements and optimized performance. Heavily inspired by Noita and their developer talks, this project includes:

1. Procedural Terrain Generation: The terrain is procedurally generated at the start, providing a unique environment each time.

2. Elemental Interactions: Various environmental blocks, such as sand, water, and other materials, interact with each other based on simulated physics and properties. This creates realistic reactions between elements(Fire burns water and will produce smoke).

3. Object-Oriented Design: The project is structured using Object-Oriented Programming principles, with all environmental blocks derived from a common Pixel class. This avoids the need for dynamic casting to convert between block types. Each block type inherits from Pixel and is stored in a 2D array of Pixel pointers. To boost performance with flammable objects, simple boolean flags are used to differentiate block types instead of relying on frequent type checking.

4. Multithreaded Chunk System: Given the computational demands, the environment is split into "chunks" and smaller "subchunks" for efficient updating. Each subchunk is updated in a staggered pattern to prevent conflicts and ensure smooth interactions between chunks. Here’s how it works:

                                                      X O X O X O
                                                      
                                                      O O O O O O
                                                      
                                                      X O X O X O
                                                      
                                                      O O O O O O
                                                      
                                                      
                                                      SECOND PASS :
                                                      
                                                      O O O O O O 
                                                      
                                                      X O X O X O 
                                                      
                                                      O O O O O O
                                                      
                                                      X O X O X O
                                                      
                                                      
                                                      THIRD PASS :
                                                      
                                                      O X O X O X
                                                      
                                                      O O O O O O
                                                      
                                                      O X O X O X
                                                      
                                                      O O O O O O
                                                      
                                                      
                                                      FOURTH PASS : 
                                                      
                                                      O O O O O O
                                                      
                                                      O X O X O X
                                                      
                                                      O O O O O O
                                                      
                                                      O X O X O X

In the first update pass, only certain subchunks (marked "X") are updated, leaving others (marked "O") unchanged.
On the next pass, the pattern shifts, updating a new set of subchunks, while skipping over others. This staggering continues across four passes, with each pass updating a different configuration of subchunks.
This staggered approach ensures that blocks don’t move far enough to interfere with neighboring subchunks in the same pass, reducing "dirty reads" and allowing efficient parallel processing. The process also splits "X" subchunks between multiple threads, improving processing time.

5. Performance Optimization: Handling tens of thousands of moving pixels can be computationally intensive. Several optimization strategies were implemented:
  1. Only pixels within a specified radius of the player are updated and rendered, allowing natural-looking interactions to occur as the player moves closer(In the videos, you can see blocks falling when the character moves to a new area as they are suspended until updated).
  2. Environmental blocks are processed in 16x16 sections, with every other section handled by different threads. Since each pixel has a limited movement range, this setup prevents conflicts between threads, ensuring smoother performance.

6. Next Steps for Optimization: To further reduce processing load, additional methods could be developed to determine precisely which areas of the environment need updating, refining the simulation's efficiency even more.
  1. A methodology for deciding which blocks require updating by keeping track of previously updated blocks and their potential new locations. This will further reduce the number of block locations needing to be checked.
  2. Smooth out players movement
  3. Add enemies and interactable elements
  4. Allow particles to show realistic movement when altered by the player(eg kicking sand in a non-linear path).

https://github.com/user-attachments/assets/72edb2c3-a8c7-46e6-bb74-40d53367f135
