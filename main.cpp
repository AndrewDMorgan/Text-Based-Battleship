#include <iostream>
#include <math.h>

// the grid size (so you could change it if you wanted to)
static int gridX = 10;
static int gridY = 10;

// the number of each ship (so its changeable and because I don't know how many of each there are)
static int numCarriers    = 1;
static int numBattleships = 1;
static int numCruisers    = 1;
static int numSubmarines  = 1;
static int numDestroyers  = 1;

static int shipSizes[5] = {4, 3, 2, 2, 1};  // the length of the ships (1 less than the actuall size)
static std::string shipNames[5] = {"Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer"};

static std::string abcd[10] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "J"};

// some info on the ships (based on the settings)
static int totalShips = numCarriers + numBattleships + numCruisers + numSubmarines + numDestroyers;
static int numShips[5] = {numCarriers, numBattleships, numCruisers, numSubmarines, numDestroyers};

// the total combined length of all the ships (used for detecting a win)
static int totalLength = (shipSizes[0]+1) * numCarriers + (shipSizes[1]+1) * numBattleships + (shipSizes[2]+1) * numCruisers + (shipSizes[3]+1) * numSubmarines + (shipSizes[4]+1) * numDestroyers;

// an empty list of ints (for use when placing ships sense there are no hits/misses)
static int emptyIntList[0];


// stores the info on a ship (reducing code and simplifying the code)
class BattleShip
{
    public:

        int sizeX, sizeY;  // the size of the ship
        int posX , posY;   // the position of the ship
        int length;  // the length of the battleship
        bool* hit;

        BattleShip() = default;
        
        // the initzialization of the BattleShip class
        BattleShip(int sizeX_, int sizeY_, int posX_, int posY_, int length_, bool* hit_)
        {
            // initializing the variables
            sizeX = sizeX_;
            sizeY = sizeY_;
            posX  = posX_;
            posY  = posY_;

            length = length_;

            hit = hit_;
        }
    
        // checks for collition between the ship and any 2D point
        bool PointRectCollition(int x, int y)
        {
            bool xCol = x >= posX && x <= posX + sizeX;
            bool yCol = y >= posY && y <= posY + sizeY;
            return xCol && yCol;
        }

        // checks if a movement will keep the ship on the board
        bool CheckMovement(int dx, int dy)
        {
            // finding the two ends of the ship
            int x1 = posX  + dx;
            int x2 = sizeX + x1;

            int y1 = posY  + dy;
            int y2 = sizeY + y1;

            // checking if both the ends are within the boarder
            bool x = (x1 >= 0 && x1 < gridX) && (x2 >= 0 && x2 < gridX);
            bool y = (y1 >= 0 && y1 < gridY) && (y2 >= 0 && y2 < gridY);

            return x && y;
        }

        // checks if another ship collides with this one
        bool CheckCollitionShip (BattleShip battleShip)
        {
            // making sure both x points dont collide along a 1d line (ship1)
            bool ship1X1 = (posX         >= battleShip.posX) && (posX         <= battleShip.posX + battleShip.sizeX);
            bool ship1X2 = (posX + sizeX >= battleShip.posX) && (posX + sizeX <= battleShip.posX + battleShip.sizeX);

            // making sure both y points dont collide along a 1d line (ship1)
            bool ship1Y1 = (posY         >= battleShip.posY) && (posY         <= battleShip.posY + battleShip.sizeY);
            bool ship1Y2 = (posY + sizeY >= battleShip.posY) && (posY + sizeY <= battleShip.posY + battleShip.sizeY);

            // making sure both x points dont collide along a 1d line (ship2)
            bool ship2X1 = (battleShip.posX                    >= posX) && (battleShip.posX                    <= posX + sizeX);
            bool ship2X2 = (battleShip.posX + battleShip.sizeX >= posX) && (battleShip.posX + battleShip.sizeX <= posX + sizeX);

            // making sure both y points dont collide along a 1d line (ship2)
            bool ship2Y1 = (battleShip.posY                    >= posY) && (battleShip.posY                    <= posY + sizeY);
            bool ship2Y2 = (battleShip.posY + battleShip.sizeY >= posY) && (battleShip.posY + battleShip.sizeY <= posY + sizeY);

            // checking the first ships collition
            bool collided = (ship1X1 || ship1X2 || ship2X1 || ship2X2) && (ship1Y1 || ship1Y2 || ship2Y1 || ship2Y2);

            return collided;
        }
};


// renders the baord
void RenderBaord(BattleShip* battleShips, int* hitsX, int* hitsY, int hits, int* missesX, int* missesY, int misses, int totalShips)
{
    std::cout << " -1-2-3-4-5-6-7-8-9-10-" << std::endl;
    std::cout << " ----------------------" << std::endl;

    // looping through the layers
    for (int y = 0; y < gridY; y++)
    {
        std::cout << abcd[y] << "|";
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
    std::cout << " ----------------------" << std::endl;
}


// gets the battle ships
BattleShip* GetBattleships()
{
    BattleShip* ships = new BattleShip[totalShips];  // all the ships

    // placing ships on the board
    float currentType = 0;
    for (int i = 0; i < totalShips; i++)
    {
        int shipIndex = currentType;

        // basic veriables for the ship
        int xPos = 0;
        int yPos = 0;
        int ySize = 0;
        
        std::string input;
        
        int length = shipSizes[shipIndex];
        int xSize = length;

        bool hit[length];

        bool collided;
        bool placed = false;
        ships[i] = BattleShip(xSize, ySize, xPos, yPos, length, hit);  // adding a temporary battleship (gets updated, used for rendering)
        while (!placed)
        {
            RenderBaord(ships, emptyIntList, emptyIntList, 0, emptyIntList, emptyIntList, 0, i + 1);
            // printing out the basic information (size, inputs, ect...)
            std::cout << shipNames[shipIndex] << "\nSize: " << shipSizes[shipIndex] + 1 << "\np to place\nr to rotate\nwasd to move\n > ";
            std::cin >> input;
            
            // interpreting the inputed information
            if (input == "p")
            {
                // checking if the ship is overlapping another ship
                collided = false;
                for (int j = 0; j < i; j++)
                {
                    // checking for collision with the other ship
                    collided = ships[i].CheckCollitionShip(ships[j]);
                    if (collided) break;
                }

                // checking if any ship overlapped the one being placed
                placed = !collided;
            }
            else if (input == "w" && ships[i].CheckMovement( 0, -1)) yPos--;
            else if (input == "a" && ships[i].CheckMovement(-1,  0)) xPos--;
            else if (input == "s" && ships[i].CheckMovement( 0,  1)) yPos++;
            else if (input == "d" && ships[i].CheckMovement( 1,  0)) xPos++;
            else if (input == "r")
            {
                int nx, ny;

                // checking the current rotation and changing the rotation from there
                if (ySize == 0)
                {
                    nx = 0;
                    ny = length;
                }
                else
                {
                    nx = length;
                    ny = 0;
                }

                // checking if the rotation is within bounds
                if (BattleShip(nx, ny, xPos, yPos, length, hit).CheckMovement(0, 0))
                {
                    // rotating the ship
                    xSize = nx;
                    ySize = ny;
                }
            }

            // rendering the new setup
            ships[i] = BattleShip(xSize, ySize, xPos, yPos, length, hit);  // adding/overwriting with a new battleship
        }

        currentType += 1./(float)numShips[shipIndex];  // incrementing the type index
    }

    return ships;
}


// auto sets up a board
BattleShip* SetupAI()
{
    BattleShip battleShip;
    BattleShip* ships = new BattleShip[totalShips];  // all the ships

    float currentType = 0;
    for (int i = 0; i < totalShips; i++)
    {
        int shipIndex = currentType;

        bool placed = false;
        while (!placed)
        {
            // finding the random values
            int r = round((float)rand() / RAND_MAX);
            int x = round((((float)rand() / RAND_MAX) * (gridX - 1)));
            int y = round((((float)rand() / RAND_MAX) * (gridY - 1)));

            // finding the size
            int length = shipSizes[shipIndex];
            bool hit[length];

            int sizeX = length;
            int sizeY = 0;

            if (r == 0)
            {
                sizeX = 0;
                sizeY = length;
            }

            // creating a new ship
            battleShip = BattleShip(sizeX, sizeY, x, y, length, hit);

            // checking if the ship fits
            bool collided = false;
            for (int j = 0; j < i; j++)
            {
                collided = battleShip.CheckCollitionShip(ships[j]);
                if (collided) break;
            }

            // adding the ship
            placed = !collided;
            if (placed)
            {
                int x1 = x;
                int x2 = x + sizeX;

                int y1 = y;
                int y2 = y + sizeY;
                
                // checking if the ship is on the board (in bounds)
                if (x1 >= 0 && x2 < gridX && y1 >= 0 && y2 < gridY) ships[i] = battleShip;
                else placed = false;
            }
        }

        currentType += 1./(float)numShips[shipIndex];
    }

    return ships;
}


// clears the terminal (sort of)
void Clear()
{
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
}


template<typename T>
int GetIndex(T* array, T item, int l)
{
    for (int i = 0; i < l; i++)
    {
        if (array[i] == item) return i;
    }
    return -1;
}


// the main program
int main()
{
    srand((unsigned)time(NULL));  // setting up for random numbers

    std::string null;  // used  for awaiting the players next action (never is read just used to write junk to something)
    std::string usrInput;  // used for the players input (mostly on settings and stuff)

    // checking if the first player is using auto fill
    std::cout << "Setings\n(y/n) AI Opponent > ";
    std::cin >> usrInput;

    // checking if the player chose AI or not
    bool AI = false;
    if (usrInput == "y") AI = true;

    BattleShip* battleShips1;
    
    if (!AI)
    {
        battleShips1 = GetBattleships();  // getting the first players ship setup

        std::cout << "Type Anything To Continue\n > ";
        std::cin >> null;  // waiting for the next player to take over
    }
    else
    {
        battleShips1 = SetupAI();  // getting the random board

        // rendering the placed ships
        RenderBaord(battleShips1, emptyIntList, emptyIntList, 0, emptyIntList, emptyIntList, 0, totalShips);
        std::cout << "\nShips Auto Placed" << std::endl;

        // waiting for the player to look at their board
        std::cout << "Type Anything To Continue\n > ";
        std::cin >> null;  // waiting for the next player to take over
    }

    // clearing the board so you can't cheat
    Clear();
    std::cout << "Player 2's Turn\nType Anything To Continue\n > ";
    std::cin >> null;  // waiting for the next player to take over
    
    // checking if the second player is using auto fill
    std::cout << "Setings\n(y/n) AI Opponent > ";
    std::cin >> usrInput;

    // checking if the player chose AI or not
    AI = false;
    if (usrInput == "y") AI = true;

    BattleShip* battleShips2;

    if (!AI)
    {
        battleShips2 = GetBattleships();  // getting the second players ship setup

        std::cout << "Type Anything To Continue\n > ";
        std::cin >> null;  // waiting for the next player to take over
    }
    else
    {
        battleShips2 = SetupAI();  // getting the random board

        // rendering the placed ships
        RenderBaord(battleShips2, emptyIntList, emptyIntList, 0, emptyIntList, emptyIntList, 0, totalShips);
        std::cout << "\nShips Auto Placed" << std::endl;

        // waiting for the player to look at their board
        std::cout << "Type Anything To Continue\n > ";
        std::cin >> null;  // waiting for the next player to take over
    }

    // all the hits for player 1
    int hits1X[gridX * gridY];
    int hits1Y[gridX * gridY];
    int numHits1 = 0;
    // all the misses for player 1
    int misses1X[gridX * gridY];
    int misses1Y[gridX * gridY];
    int numMisses1 = 0;

    // all the hits from player 1 on player 2
    int hits2X[gridX * gridY];
    int hits2Y[gridX * gridY];
    int numHits2 = 0;
    // all the misses for player 2 on player 1
    int misses2X[gridX * gridY];
    int misses2Y[gridX * gridY];
    int numMisses2 = 0;

    // resseting the screen before starting the game
    Clear();

    // starting on a random players turn
    int turn = round((float)rand() / RAND_MAX);

    // a loop that goes while the game is running
    while (true)
    {
        // clearing the screen so you can't see the other players ships
        Clear();
        std::cout << "Player " << turn + 1 <<  "'s turn\nType Anything To Continue\n > ";
        std::cin >> null;  // waiting for the next player to take over

        if (turn == 0)
        {
            // showing your hits and opponents hits
            RenderBaord(battleShips1, hits1X, hits1Y, numHits1, misses1X, misses1Y, numMisses1, 0);
            std::cout << " " << std::endl;
            RenderBaord(battleShips1, hits2X, hits2Y, numHits2, misses2X, misses2Y, numMisses2, totalShips);

            // positions for a hit/miss
            std::string stringX, stringY;
            int x, y;

            bool valid = false;  // making sure you not guessing ontop of a guess

            while (!valid)
            {
                // finding the pos of the new hit/miss
                std::cout << "(number) X > ";
                std::cin >> stringX;
                std::cout << "(letter) Y > ";
                std::cin >> stringY;

                // getting the intiger position of the point
                x = stoi(stringX) - 1;
                y = GetIndex(abcd, stringY, 10);

                // checking for overlapping
                bool overlapped = false;
                for (int i = 0; i < numMisses1; i++)
                {
                    if (misses1X[i] == x && misses1Y[i] == y)
                    {
                        // going to the end and setting it to overlapping (saves processing time i guess)
                        overlapped = true;
                        goto end1;
                    }
                }

                for (int i = 0; i < numHits1; i++)
                {
                    if (hits1X[i] == x && hits1Y[i] == y)
                    {
                        // going to the end and setting it to overlapping (saves processing time i guess)
                        overlapped = true;
                        goto end1;
                    }
                }

            end1:

                // checking for any overlap
                if (!overlapped) valid = true;

            }

            // checking for a hit
            bool hitShip = false;
            for (int i = 0; i < totalShips; i++)
            {
                if (battleShips2[i].PointRectCollition(x, y))
                {
                    // adding a new hit
                    hitShip = true;

                    hits1X[numHits1] = x;
                    hits1Y[numHits1] = y;

                    numHits1++;
                    
                    std::cout << "Hit!!!!!" << std::endl;

                    // checking if the ship was sunk
                    int shiftX = 1. / battleShips2[i].length * (battleShips2[i].sizeX == 0 ? 0 : 1);
                    int shiftY = 1. / battleShips2[i].length * (battleShips2[i].sizeY == 0 ? 0 : 1);

                    // looping across the entire ship
                    int numPoints = 0;
                    for (int j = 0; j < numHits1; j++)
                    {
                        // checking for the point
                        if (battleShips2[i].PointRectCollition(hits1X[j], hits1Y[j])) numPoints++;
                    }

                    // checking if ever pos of the ship was struck
                    if (numPoints > battleShips2[i].length) std::cout << "Ship Sunk!!" << std::endl;

                }
            }

            if (!hitShip)
            {
                // adding a new miss
                misses1X[numMisses1] = x;
                misses1Y[numMisses1] = y;

                numMisses1++;

                std::cout << "Miss..." << std::endl;
            }

            turn = 1;

            std::cout << "Type Anything To Continue\n > ";
            std::cin >> null;  // waiting for the next player to take over
        }
        else
        {
            // showing your hits and opponents hits
            RenderBaord(battleShips2, hits2X, hits2Y, numHits2, misses2X, misses2Y, numMisses2, 0);
            std::cout << " " << std::endl;
            RenderBaord(battleShips2, hits1X, hits1Y, numHits1, misses1X, misses1Y, numMisses1, totalShips);

            // positions for a hit/miss
            std::string stringX, stringY;
            int x, y;

            bool valid = false;  // making sure you not guessing ontop of a guess

            while (!valid)
            {
                // finding the pos of the new hit/miss
                std::cout << "(number) X > ";
                std::cin >> stringX;
                std::cout << "(letter) Y > ";
                std::cin >> stringY;

                // getting the intiger position of the point
                x = stoi(stringX) - 1;
                y = GetIndex(abcd, stringY, 10);

                // checking for overlapping
                bool overlapped = false;
                for (int i = 0; i < numMisses2; i++)
                {
                    if (misses2X[i] == x && misses2Y[i] == y)
                    {
                        // going to the end and setting it to overlapping (saves processing time i guess)
                        overlapped = true;
                        goto end2;
                    }
                }

                for (int i = 0; i < numHits2; i++)
                {
                    if (hits2X[i] == x && hits2Y[i] == y)
                    {
                        // going to the end and setting it to overlapping (saves processing time i guess)
                        overlapped = true;
                        goto end2;
                    }
                }

            end2:

                // checking for any overlap
                if (!overlapped) valid = true;

            }

            // checking for a hit
            bool hitShip = false;
            for (int i = 0; i < totalShips; i++)
            {
                if (battleShips1[i].PointRectCollition(x, y))
                {
                    // adding a new hit
                    hitShip = true;

                    hits2X[numHits2] = x;
                    hits2Y[numHits2] = y;

                    numHits2++;

                    std::cout << "Hit!!!!!" << std::endl;

                    // checking if the ship was sunk
                    int shiftX = 1. / battleShips1[i].length * (battleShips1[i].sizeX == 0 ? 0 : 1);
                    int shiftY = 1. / battleShips1[i].length * (battleShips1[i].sizeY == 0 ? 0 : 1);

                    // looping across the entire ship
                    int numPoints = 0;
                    for (int j = 0; j < numHits2; j++)
                    {
                        // checking for the point
                        if (battleShips1[i].PointRectCollition(hits2X[j], hits2Y[j])) numPoints++;
                    }
                    
                    // checking if ever pos of the ship was struck
                    if (numPoints > battleShips1[i].length) std::cout << "Ship Sunk!!" << std::endl;
                }
            }

            if (!hitShip)
            {
                // adding a new miss
                misses2X[numMisses2] = x;
                misses2Y[numMisses2] = y;

                numMisses2++;

                std::cout << "Miss..." << std::endl;
            }

            turn = 0;

            std::cout << "Type Anything To Continue\n > ";
            std::cin >> null;  // waiting for the next player to take over
        }

        // checking if player 1 won
        if (numHits1 == totalLength)
        {
            Clear();
            std::cout << "Player 1 Wins!!!!!!!";

            break;
        }
        else if (numHits2 == totalLength)  // checking if player 2 won
        {
            Clear();
            std::cout << "Player 2 Wins!!!!!!!";

            break;
        }
    }

    // cleaning up the memory
    delete battleShips1;
    delete battleShips2;

    return 0;  // ending the program
}

