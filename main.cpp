#include <iostream>

// the grid size (so you could change it if you wanted to)
static int gridX = 10;
static int gridY = 10;


class BattleShip
{
    public:

        int sizeX, sizeY;  // the size of the ship
        int posX , posY;   // the position of the ship
        bool* hit;

        BattleShip() = default;
        

        BattleShip(int sizeX_, int sizeY_, int posX_, int posY_, bool* hit_)
        {
            sizeX = sizeX_;
            sizeY = sizeY_;
            posX  = posX_;
            posY  = posY_;

            hit = hit_;
        }
    
        bool PointRectCollition(int x, int y)
        {
            bool xCol = x >= posX && x <= posX + sizeX;
            bool yCol = y >= posY && y <= posY + sizeY;
            return xCol && yCol;
        }

        bool CheckCollition(int sx, int sy, int px, int py)
        {
            bool tlPoint = PointRectCollition(px     , py + sy);
            bool trPoint = PointRectCollition(px + sx, py + sy);
            bool blPoint = PointRectCollition(px     , py     );
            bool brPoint = PointRectCollition(px + sx, py     );
            return tlPoint && trPoint && blPoint && brPoint;
        }
};


void RenderBaord(BattleShip battleShips[], int hitsX[], int hitsY[], int hits, int missesX[], int missesY[], int misses, int totalShips)
{
    std::cout << "----------------------" << std::endl;
    for (int y = 0; y < gridY; y++)
    {
        std::cout << "|";
        for (int x = 0; x < gridX; x++)
        {
            std::string peice = "  ";
            for (int i = 0; i < hits; i++)
            {
                if (hitsX[i] == x && hitsY[i] == y)
                {
                    peice = "><";
                    goto end;
                }
            }
            for (int i = 0; i < misses; i++)
            {
                if (missesX[i] == x && missesY[i] == y)
                {
                    peice = "~~";
                    goto end;
                }
            }
            for (int i = 0; i < totalShips; i++)
            {
                if (battleShips[i].PointRectCollition(x, y))
                {
                    peice = "[]";
                    goto end;
                }
            }
        end:  // to skip the other loops for preformace if one was found
            std::cout << peice;
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "----------------------" << std::endl;
}


int main()
{
    int numCarriers    = 1;
    int numBattleships = 1;
    int numCruisers    = 1;
    int numSubmarines  = 1;
    int numDestroyers  = 1;

    int shipSizes[5] = {4, 3, 2, 2, 1};  // 1 less than the actuall size btw
    std::string shipNames[5] = {"Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer"};

    int totalShips = numCarriers + numBattleships + numCruisers + numSubmarines + numDestroyers;
    int numShips[5] = {numCarriers, numBattleships, numCruisers, numSubmarines, numDestroyers};

    BattleShip ships1[5];

    int emptyIntList[0];

    float currentType = 0;
    for (int i = 0; i < totalShips; i++)
    {
        int shipIndex = currentType;

        bool hit[5];
        int xPos = 0;
        int yPos = 0;
        int ySize = 0;
        std::string input;
        bool placed = false;
        int xSize = shipSizes[shipIndex];
        ships1[i] = BattleShip(xSize, ySize, xPos, yPos, hit);
        RenderBaord(ships1, emptyIntList, emptyIntList, 0, emptyIntList, emptyIntList, 0, i + 1);
        while (!placed)
        {
            std::cout << shipNames[shipIndex] << "\nSize: " << shipSizes[shipIndex] << "\np to place\nr to rotate\nwasd to move\n > ";
            std::cin >> input;
            
            if (input == "p") placed = true;  // loop i-1 times and check for collition before placing pices
            else if (input == "w") yPos--;
            else if (input == "a") xPos--;
            else if (input == "s") yPos++;
            else if (input == "d") xPos++;
            else if (input == "r")
            {
                if (ySize == 0)
                {
                    xSize = 0;
                    ySize = shipSizes[shipIndex];
                }
                else
                {
                    xSize = shipSizes[shipIndex];
                    ySize = 0;
                }
            }

            ships1[i] = BattleShip(xSize, ySize, xPos, yPos, hit);

            RenderBaord(ships1, emptyIntList, emptyIntList, 0, emptyIntList, emptyIntList, 0, i + 1);
        }

        currentType += 1./(float)numShips[shipIndex];
    }

    return 0;
}

