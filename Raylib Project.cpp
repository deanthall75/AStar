#include "raylib.h"
#include "AStar.h"


const int WORLD_WIDTH = 30;
const int WORLD_HEIGHT = 20;
const int CELL_WIDTH = 32;
const int CELL_HEIGHT = 32;

AStar astar;
AStarNode start{ 2, 2 };
AStarNode goal{ 20, 15 };

static void DrawGrid()
{
    for (int grid_y = 0; grid_y < WORLD_HEIGHT; grid_y++)
    {
        for (int grid_x = 0; grid_x < WORLD_WIDTH; grid_x++)
        {
			// current node
            AStarNode node{ grid_x, grid_y };

			// define cell rectangle
            Rectangle nodeRec = {
                node.x* CELL_WIDTH,
                node.y* CELL_HEIGHT,
                CELL_WIDTH,
                CELL_HEIGHT
            };

			// draw start cell
            if (node == start)
            {
				DrawRectangleRec(nodeRec, GREEN);
            }
            
			// draw goal cell
            if (node == goal)
            {
                DrawRectangleRec(nodeRec, BLUE);
            }

			// draw blocked cells
            if (!astar.IsWalkableCell(node.x, node.y))
            {
                DrawRectangleRec(nodeRec, RED);
			}
            

            // draw grid cell
            DrawRectangleLines(nodeRec.x, nodeRec.y, nodeRec.width, nodeRec.height, DARKGRAY);

            // draw cost
            const char* text = TextFormat("%i", (int)astar.GetTileCost(node.x, node.y));
            DrawText(text, nodeRec.x+8, nodeRec.y+8, 12, RED);

        }
    }

    // draw path
    std::vector path = astar.FindPath(start, goal);
    for (AStarNode node : path)
    {
        float wx, wy;
        astar.TileToWorld(node, wx, wy);

        DrawCircle(wx, wy, 8, GREEN);
    }
}


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1024;
    const int screenHeight = 768;

    InitWindow(screenWidth, screenHeight, "raylib Astar example");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // setup astar world
    astar.ResizeGrid(WORLD_WIDTH, WORLD_HEIGHT);
    astar.SetTileSize(CELL_WIDTH, CELL_HEIGHT);
    astar.EnableDiagonalMovement(true);


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update

        AStarNode node = astar.WorldToTile(GetMouseX(), GetMouseY());
        if (astar.InBounds(node.x, node.y))
        {
            // set path start
            if (IsKeyPressed(KEY_S))
            {
                start = node;
            }

            // set path goal
            if (IsKeyPressed(KEY_G))
            {
                goal = node;
            }

            // set solid cell
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                if (astar.InBounds(node.x, node.y))
                {
                    astar.SetWalkable(node.x, node.y, false);
                }
            }

            // clear solid cell
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            {
                if (astar.InBounds(node.x, node.y))
                {
                    astar.SetWalkable(node.x, node.y, true);
                }
            }

            // set use diag
            if (IsKeyPressed(KEY_D))
            {
                bool diag = astar.isDiagonalMovementEnabled();
                astar.EnableDiagonalMovement(!diag);
            }

            // set tile cost
            if (IsKeyDown(KEY_ONE)) { astar.SetTileCost(node.x, node.y, 1); }
            if (IsKeyDown(KEY_TWO)) { astar.SetTileCost(node.x, node.y, 2); }
            if (IsKeyDown(KEY_THREE)) { astar.SetTileCost(node.x, node.y, 3); }


        }
		
        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawGrid();

        DrawText("Left Button - Set cell solid", 10, 650, 20, BLACK);
        DrawText("Right Button - CLear cell solid", 10, 670, 20, BLACK);
        
        const char * diag = TextFormat("D - Toggle diagonal (%s)", astar.isDiagonalMovementEnabled() ? "On" : "Off");;
        DrawText(diag, 10, 690, 20, BLACK);
        
        DrawText("1,2,3 - Set tile cost", 10, 710, 20, BLACK);



        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}