// ===============================
// AStar.cpp
// ===============================
#include "AStar.h"
#include <algorithm>

AStar::AStar() = default;

void AStar::SetTileSize(float tileWidth, float tileHeight)
{
    m_tileWidth = tileWidth;
    m_tileHeight = tileHeight;
}

AStarNode AStar::WorldToTile(float worldX, float worldY) const
{
    return {
        static_cast<int>(worldX / m_tileWidth),
        static_cast<int>(worldY / m_tileHeight)
    };
}

void AStar::TileToWorld(const AStarNode& tile, float& outWorldX, float& outWorldY) const
{
    // Center of tile (nice for sprites)
    outWorldX = tile.x * m_tileWidth + m_tileWidth * 0.5f;
    outWorldY = tile.y * m_tileHeight + m_tileHeight * 0.5f;
}

AStar::AStar(int width, int height)
{
    ResizeGrid(width, height);
}

AStar::AStar(const Grid& grid)
{
    SetGrid(grid);
}

void AStar::ResizeGrid(int width, int height)
{
    m_width = width;
    m_height = height;
    m_grid.assign(height, std::vector<int>(width, 0));
    m_costGrid.assign(height, std::vector<float>(width, 1.0f));
}

void AStar::SetGrid(const Grid& grid)
{
    m_grid = grid;
    m_height = (int)grid.size();
    m_width = m_height > 0 ? (int)grid[0].size() : 0;
    m_costGrid.assign(m_height, std::vector<float>(m_width, 1.0f));
}

void AStar::SetWalkable(int x, int y, bool walkable)
{
    if (InBounds(x, y))
        m_grid[y][x] = walkable ? 0 : 1;
}

void AStar::SetTileCost(int x, int y, float cost)
{
    if (InBounds(x, y) && cost >= 1.0f)
        m_costGrid[y][x] = cost;
}

float AStar::GetTileCost(int x, int y) const
{
    return InBounds(x, y) ? m_costGrid[y][x] : std::numeric_limits<float>::infinity();
}

void AStar::EnableDiagonalMovement(bool enable)
{
    m_allowDiagonal = enable;
}
bool AStar::isDiagonalMovementEnabled() 
{ return m_allowDiagonal; }

bool AStar::IsWalkableCell(int x, int y) const
{
    return InBounds(x, y) && m_grid[y][x] == 0;
}

std::vector<AStarNode> AStar::FindPath(AStarNode start, AStarNode goal)
{
    std::priority_queue<NodeRecord, std::vector<NodeRecord>, Compare> open;
    std::unordered_map<AStarNode, float, AStarNodeHash> gScore;
    std::unordered_map<AStarNode, AStarNode, AStarNodeHash> cameFrom;

    // Closed set: tracks nodes already fully processed
    std::vector<std::vector<bool>> closed(m_height, std::vector<bool>(m_width, false));

    gScore[start] = 0.0f;
    open.push({ start, Heuristic(start, goal) });

    while (!open.empty())
    {
        AStarNode current = open.top().node;
        open.pop();

        if (closed[current.y][current.x])
            continue;

        if (current == goal)
            return ReconstructPath(cameFrom, current);

        closed[current.y][current.x] = true;

        for (const AStarNode& neighbor : GetNeighbors(current))
        {
            if (closed[neighbor.y][neighbor.x])
                continue;

            float moveCost = GetTileCost(neighbor.x, neighbor.y);
            float diagonalPenalty =
                (neighbor.x != current.x && neighbor.y != current.y) ? 1.41421356f : 1.0f;

            float tentativeG = gScore[current] + moveCost * diagonalPenalty;

            if (!gScore.count(neighbor) || tentativeG < gScore[neighbor])
            {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentativeG;
                float f = tentativeG + Heuristic(neighbor, goal);
                open.push({ neighbor, f });
            }
        }
    }

    return {};
}

bool AStar::InBounds(int x, int y) const
{
    return x >= 0 && y >= 0 && x < m_width && y < m_height;
}

float AStar::Heuristic(const AStarNode& a, const AStarNode& b) const
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

std::vector<AStarNode> AStar::GetNeighbors(const AStarNode& n) const
{
    static const int dirs4[4][2] =
    {
        { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }
    };

    static const int dirs8[8][2] =
    {
        { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 },
        { 1, 1 }, { -1, -1 }, { 1, -1 }, { -1, 1 }
    };

    const int (*dirs)[2] = m_allowDiagonal ? dirs8 : dirs4;
    int count = m_allowDiagonal ? 8 : 4;

    std::vector<AStarNode> result;
    for (int i = 0; i < count; ++i)
    {
        int nx = n.x + dirs[i][0];
        int ny = n.y + dirs[i][1];
        if (IsWalkableCell(nx, ny))
            result.push_back({ nx, ny });
    }
    return result;
}


std::vector<AStarNode> AStar::ReconstructPath(
    std::unordered_map<AStarNode, AStarNode, AStarNodeHash>& cameFrom,
    AStarNode current)
{
    std::vector<AStarNode> path;
    path.push_back(current);

    while (cameFrom.count(current))
    {
        current = cameFrom[current];
        path.push_back(current);
    }

    std::reverse(path.begin(), path.end());
    return path;
}
