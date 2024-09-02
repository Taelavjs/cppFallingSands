Falling Sands Simulation Demonstration :

https://github.com/user-attachments/assets/fa0effbf-9cb4-4ab9-8fd8-f5a717b15864

Involves initial procedural generation of the terrain
Elemental effects between each other
OOP principles through abstraction.

Pixels are split into 16x16 sections split between several threads for updating. To avoid collisions, every other block of 16x16 is rendered by another thread, and as pixels have a max distance travelled, the core grid cannot be dirty read/modified.

https://github.com/user-attachments/assets/38046bf6-7dca-4b54-8351-5009c625cfeb




