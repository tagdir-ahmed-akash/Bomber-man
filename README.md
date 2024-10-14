# Grid-Based Bomb Game

## Overview
This 2D grid-based game, developed in C++, allows players to navigate a strategic environment while avoiding enemies and utilizing bombs to clear obstacles. Players collect gift bags to enhance their abilities and survive against increasingly challenging foes.

## Game Mechanics

### Player Control
- **Movement**: The player can move in four directions (up, down, left, right) using the arrow keys.
- **Bomb Deployment**: Pressing the spacebar deploys a bomb at the player's current location, which can eliminate enemies and destroy obstacles.

### Grid Layout
- The game features a 17x17 grid where each cell can contain:
  - **Walls/Boxes**: Impassable areas that block movement.
  - **Gift Bags**: Collectibles that enhance the player's bomb range when acquired.

### Enemy Behavior
- Enemies move randomly within the grid and will collide with the player if they occupy the same cell.
- The player must avoid enemy collisions to prevent game loss.

### Bomb Mechanics
- Bombs have a specific range and can destroy adjacent obstacles and enemies upon explosion.
- The bomb activates after a short delay when deployed.

### Collectibles
- **Gift Bags**: Found randomly in the grid, these increase the player's bomb range, allowing for more strategic plays.

## Setup Instructions
1. **Install a C++ Compiler**: Use an IDE like Code::Blocks or Visual Studio.
2. **Set up the graphics library**: Ensure that the BGI graphics library is properly configured.
3. **Compile and Run**: Open the project in your IDE, compile the code, and run the executable.

## Controls
- **Arrow Keys**: Move the player character.
- **Spacebar**: Deploy a bomb.

## Game Objective
The goal is to survive as long as possible while navigating the grid, avoiding enemy encounters, and utilizing bombs strategically to clear paths and defeat enemies.

## Contributing
Feel free to fork the repository and submit pull requests for improvements or new features.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments
Special thanks to the open-source community for their contributions to game development resources.

How to Use
Copy the above text.
Create a file named README.md in your project directory.
Paste the copied text into the file and save it.
This README provides a clear overview of how your game works, including controls, mechanics, and setup instructions!
