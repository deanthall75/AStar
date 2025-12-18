// ===============================
// AStar.h
// ===============================
#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>

struct AStarNode
{
    int x = 0;
    int y = 0;

    bool operator==(const AStarNode& other) const
    {
        return x == other.x && y == other.y;
    }
};

struct AStarNodeHash
{
    std::size_t operator()(const AStarNode& n) const
    {
        return (std::hash<int>()(n.x) << 1) ^ std::hash<int>()(n.y);
    }
};

class AStar
{
public:
    using Grid = std::vector<std::vector<int>>; // 0 = walkable, 1 = blocked

    AStar();
    AStar(int width, int height);
    AStar(const Grid& grid);

    // Grid management
    void ResizeGrid(int width, int height);
    void SetGrid(const Grid& grid);

    void SetWalkable(int x, int y, bool walkable);
    bool IsWalkableCell(int x, int y) const;

    // Per-tile movement cost (>= 1.0f)
    void SetTileCost(int x, int y, float cost);
    float GetTileCost(int x, int y) const;

    // Diagonal movement
    void EnableDiagonalMovement(bool enable);
	bool isDiagonalMovementEnabled();

    // -----------------------------
    // World <-> Tile helpers (raylib-friendly)
    // -----------------------------
    void SetTileSize(float tileWidth, float tileHeight);

    AStarNode WorldToTile(float worldX, float worldY) const;
    void TileToWorld(const AStarNode& tile, float& outWorldX, float& outWorldY) const;
    bool InBounds(int x, int y) const;

    // Pathfinding
    std::vector<AStarNode> FindPath(AStarNode start, AStarNode goal);

private:
    struct NodeRecord
    {
        AStarNode node;
        float fScore;
    };

    struct Compare
    {
        bool operator()(const NodeRecord& a, const NodeRecord& b) const
        {
            return a.fScore > b.fScore;
        }
    };

    Grid m_grid;
    std::vector<std::vector<float>> m_costGrid;
    int m_width = 0;
    int m_height = 0;

    float m_tileWidth = 1.0f;
    float m_tileHeight = 1.0f;

    bool m_allowDiagonal = false;

    float Heuristic(const AStarNode& a, const AStarNode& b) const;
    std::vector<AStarNode> GetNeighbors(const AStarNode& n) const;
    std::vector<AStarNode> ReconstructPath(
        std::unordered_map<AStarNode, AStarNode, AStarNodeHash>& cameFrom,
        AStarNode current);
};


