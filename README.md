https://github.com/user-attachments/assets/fa0effbf-9cb4-4ab9-8fd8-f5a717b15864

This Falling Sands Simulation project showcases a dynamic terrain simulation featuring interactive elements and optimized performance. Here’s a closer look at how it works:

1. Procedural Terrain Generation: The terrain is procedurally generated at the start, providing a unique environment each time.

2. Elemental Interactions: Various environmental blocks, such as sand, water, and other materials, interact with each other based on simulated physics and properties. This creates realistic reactions between elements.

3. Object-Oriented Design: The project is structured using Object-Oriented Programming principles, with all environmental blocks derived from a common Pixel class. This approach avoids the need for costly dynamic casting. Each block type inherits from Pixel and is stored in a 2D array of Pixel pointers. To boost performance, simple boolean flags are used to differentiate block types instead of relying on frequent type checking.

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
  1. Only pixels within a specified radius of the player are updated and rendered, allowing natural-looking interactions to occur as the player moves closer.
  2. Environmental blocks are processed in 16x16 sections, with every other section handled by different threads. Since each pixel has a limited movement range, this setup prevents conflicts between threads, ensuring smoother performance.

6. Next Steps for Optimization: To further reduce processing load, additional methods could be developed to determine precisely which areas of the environment need updating, refining the simulation's efficiency even more.
  1. This project was inspired by insights from the developers of Noita, a similar game, and has been a learning experience in balancing complex visual effects with performance optimization.

https://github.com/user-attachments/assets/38046bf6-7dca-4b54-8351-5009c625cfeb
