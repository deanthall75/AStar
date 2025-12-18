<h2>AStar – Lightweight Grid-Based Pathfinding (C++)</h2>

This project provides a clean, engine-agnostic A* pathfinding implementation designed for tile-based games. It is written in modern C++ with a focus on clarity, performance, and easy integration into frameworks such as raylib.

<img width="1028" height="815" alt="Screenshot 2025-12-18 222241" src="https://github.com/user-attachments/assets/f40501a4-1d0f-4e7e-9c88-10eb5ba0070d" />



The code supports dynamic grid creation, weighted tiles, optional diagonal movement, and world ↔ tile coordinate conversion, making it suitable for real-time gameplay and editor tools.

<h2>Features</h2>

- Grid-based A* pathfinding
- 4-way or 8-way movement (runtime toggle)
- Per-tile movement costs (roads, mud, water, etc.)
- Closed set optimization for efficient searches
- World ↔ tile coordinate helpers (raylib-friendly)
- Engine-agnostic (no dependencies on rendering or math libraries)

Grid cells are stored as:
- 0 → walkable
- 1 → blocked

Each tile also has an independent movement cost (default: 1.0f)

<h2>Basic Usage</h2>

```
AStar astar;
astar.ResizeGrid(20, 15);
astar.SetTileSize(32.0f, 32.0f);
astar.EnableDiagonalMovement(true);

astar.SetWalkable(5, 5, false);      // Block a tile
astar.SetTileCost(3, 4, 5.0f);       // Increase traversal cost

AStarNode start{ 1, 1 };
AStarNode goal{ 18, 10 };

auto path = astar.FindPath(start, goal);
```

<h2>World ↔ Tile Conversion</h2>
Designed to work naturally with raylib-style coordinates:

```
AStarNode tile = astar.WorldToTile(playerPos.x, playerPos.y);

float wx, wy;
astar.TileToWorld(tile, wx, wy); // Returns center of tile
```
