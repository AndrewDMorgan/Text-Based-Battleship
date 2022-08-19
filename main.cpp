#include <iostream>

// the grid size (so you could change it if you wanted to)
static int gridX = 10;
static int gridY = 10;

// stores the info on a ship (reducing code and simplifying the code)
class BattleShip
{
    public:

        int sizeX, sizeY;  // the size of the ship
        int posX , posY;   // the position of the ship
        bool* hit;

        BattleShip() = default;
        
        // the initzialization of the BattleShip class
        BattleShip(int sizeX_, int sizeY_, int posX_, int posY_, bool* hit_)
        {
            sizeX = sizeX_;
            sizeY = sizeY_;
            posX  = posX_;
            posY  = posY_;

            hit = hit_;
        }
    
        // checks for collition between the ship and any 2D point
        bool PointRectCollition(int x, int y)
        {
            bool xCol = x >= posX && x <= posX + sizeX;
            bool yCol = y >= posY && y <= posY + sizeY;
            return xCol && yCol;
        }

        // checks for collition between two rectangles
        bool CheckCollition(int sx, int sy, int px, int py)
        {
            bool tlPoint = PointRectCollition(px     , py + sy);
            bool trPoint = PointRectCollition(px + sx, py + sy);
            bool blPoint = PointRectCollition(px     , py     );
            bool brPoint = PointRectCollition(px + sx, py     );
            return tlPoint && trPoint && blPoint && brPoint;
        }
};


// renders the baord
void RenderBaord(BattleShip battleShips[], int hitsX[], int hitsY[], int hits, int missesX[], int missesY[], int misses, int totalShips)
{
    std::cout << "----------------------" << std::endl;
    // looping through the layers
    for (int y = 0; y < gridY; y++)
    {
        std::cout << "|";
        // looping through the rows
        for (int x = 0; x < gridX; x++)
        {
            std::string peice = "  ";
            // looping through the hits
            for (int i = 0; i < hits; i++)
            {
                if (hitsX[i] == x && hitsY[i] == y)
                {
                    peice = "><";
                    goto end;  // ending the search for the correct peice because it was found
                }
            }
            // looping through the misses
            for (int i = 0; i < misses; i++)
            {
                if (missesX[i] == x && missesY[i] == y)
                {
                    peice = "~~";
                    goto end;  // ending the search for the correct peice because it was found
                }
            }
            // looping through the ships
            for (int i = 0; i < totalShips; i++)
            {
                if (battleShips[i].PointRectCollition(x, y))
                {
                    peice = "[]";
                    goto end;  // ending the search for the correct peice because it was found
                }
            }
        end:  // to skip the other loops for preformace if one was found
            std::cout << peice;
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "----------------------" << std::endl;
}

// the main program
int main()
{
    // the number of each ship (so its changeable and because I don't know how many of each there are)
    int numCarriers    = 1;
    int numBattleships = 1;
    int numCruisers    = 1;
    int numSubmarines  = 1;
    int numDestroyers  = 1;

    int shipSizes[5] = {4, 3, 2, 2, 1};  // 1 less than the actuall size btw
    std::string shipNames[5] = {"Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer"};

    // some info on the ships (based on the settings)
    int totalShips = numCarriers + numBattleships + numCruisers + numSubmarines + numDestroyers;
    int numShips[5] = {numCarriers, numBattleships, numCruisers, numSubmarines, numDestroyers};

    BattleShip ships1[5];

    // an empty list of ints (for use when placing ships)
    int emptyIntList[0];

    // placing ships on the board
    float currentType = 0;
    for (int i = 0; i < totalShips; i++)
    {
        int shipIndex = currentType;

        // basic veriables for the ship
        bool hit[5];
        int xPos = 0;
        int yPos = 0;
        int ySize = 0;
        std::string input;
        bool placed = false;
        int xSize = shipSizes[shipIndex];
        ships1[i] = BattleShip(xSize, ySize, xPos, yPos, hit);
        // rendering the board (so you can see where the ship is)
        RenderBaord(ships1, emptyIntList, emptyIntList, 0, emptyIntList, emptyIntList, 0, i + 1);
        while (!placed)
        {
            // printing out the basic information (size, inputs, ect...)
            std::cout << shipNames[shipIndex] << "\nSize: " << shipSizes[shipIndex] + 1 << "\np to place\nr to rotate\nwasd to move\n > ";
            std::cin >> input;
            
            // interpreting the inputed information
            if (input == "p") placed = true;  // loop i-1 times and check for collition before placing pices
            else if (input == "w") yPos--;
            else if (input == "a") xPos--;
            else if (input == "s") yPos++;
            else if (input == "d") xPos++;
            else if (input == "r")
            {
                // checking the current rotation and changing the rotation from there
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

            // creating the new battleship
            ships1[i] = BattleShip(xSize, ySize, xPos, yPos, hit);

            // rendering the new setup
            RenderBaord(ships1, emptyIntList, emptyIntList, 0, emptyIntList, emptyIntList, 0, i + 1);
        }

        currentType += 1./(float)numShips[shipIndex];  // incrementing the type index
    }

    return 0;  // ending the program
}

