import java.util.Scanner;  // for player input

// the main logic for the board
public class MainProgram
{
    // the size of the grid
    static int gridX = 10;
    static int gridY = 10;

    // the different numbers of each type of ship, so it can be changed (because I don't know how many of each there are)
    static int numCarriers    = 1;
    static int numBattleships = 1;
    static int numCruisers    = 1;
    static int numSubmarines  = 1;
    static int numDestroyers  = 1;

    // the sizes of the different types of ships
    static int[]    shipSizes = {4, 3, 2, 2, 1};  // 1 smaller than the real size
    // the names of the different types of ships
    static String[] shipNames = {"Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer"};

    // the letters for the board positions
    static String[] abcd      = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};

    static int totalShips = numCarriers + numBattleships + numCruisers + numSubmarines + numDestroyers;
    static int[] numShips = new int[] {numCarriers, numBattleships, numCruisers, numSubmarines, numDestroyers};

    public static void main(String[] args)
    {
        // creating an instant of Scanner for player input
        Scanner scanner = new Scanner(System.in);

        System.out.println("Player 1's Turn");

        // getting the users input on having an AI set up the board for them
        String usrInput;
        System.out.print("Settings\n(y/n) AI Opponent\n > ");
        usrInput = scanner.nextLine();

        // checking if an AI set up was selected
        boolean AI = false;
        if (usrInput.equalsIgnoreCase("y")) AI = true;

        // the first users battleships
        BattleShip[] battleShips1 = new BattleShip[totalShips];

        // checking if the AI is being used
        if (!AI)
        {
            // having the player set up their ships
            battleShips1 = GetBattleships(scanner);

            System.out.println("Type Anything To Continue\n > ");
            scanner.nextLine();  // waiting for the player to be ready to continue
        }
        else  // the AI was used
        {
            // using AI to set up the board
            battleShips1 = SetupAI();

            RenderBoard(battleShips1, new int[0], new int[0], 0, new int[0], new int[0], 0, totalShips);

            System.out.println("Ships Auto Placed\nType Anything To Continue\n > ");
            scanner.nextLine();  // waiting for the player to be ready to continue
        }

        // clearing the screen to prevent cheating
        Clear();

        // waiting for player 2 to take over
        System.out.println("Player 2's Turn\nType Anything To Continue\n > ");
        scanner.nextLine();

        // getting the users input on having an AI set up the board for them
        System.out.print("Settings\n(y/n) AI Opponent\n > ");
        usrInput = scanner.nextLine();

        // checking if an AI set up was selected
        AI = false;
        if (usrInput.equalsIgnoreCase("y")) AI = true;

        // the second users battleships
        BattleShip[] battleShips2 = new BattleShip[totalShips];

        // checking if the AI is being used
        if (!AI)
        {
            // getting the users ship layout
            battleShips2 = GetBattleships(scanner);

            System.out.println("Type Anything To Continue\n > ");
            scanner.nextLine();  // waiting for the player to be ready to continue
        }
        else
        {
            // using an AI to set up the board
            battleShips2 = SetupAI();

            // rendering the board
            RenderBoard(battleShips2, new int[0], new int[0], 0, new int[0], new int[0], 0, totalShips);

            System.out.println("Ships Auto Placed\nType Anything To Continue\n > ");
            scanner.nextLine();  // waiting for the player to be ready to continue
        }

        // the area of the grid
        int gridArea = gridX * gridY;

        // all the hits for player 1
        int[] hits1X   = new int[gridArea];
        int[] hits1Y   = new int[gridArea];
        int   numHits1 = 0;
        // all the misses for player 1
        int[] misses1X   = new int[gridArea];
        int[] misses1Y   = new int[gridArea];
        int   numMisses1 = 0;

        // all the hits for player 2
        int[] hits2X   = new int[gridArea];
        int[] hits2Y   = new int[gridArea];
        int   numHits2 = 0;
        // all the misses for player 2
        int[] misses2X   = new int[gridArea];
        int[] misses2Y   = new int[gridArea];
        int   numMisses2 = 0;

        // the starting turn
        int turn = (int)Math.round(Math.random());

        // the gameplay
        while (true)
        {
            // clearing the screen, so you don't see the others ships
            Clear();

            // waiting for the next player to take over
            System.out.print("Player " + Integer.toString(turn + 1) + "'s Turn\nType Anything To Continue\n > ");
            scanner.nextLine();  // waiting for the player to have taken over

            if (turn == 0)
            {
                // showing your hits/misses
                RenderBoard(new BattleShip[0], hits1X, hits1Y, numHits1, misses1X, misses1Y, numMisses1, 0);
                System.out.println("ba");  // skipping 1 line
                // showing your ships and the ships you've hit and your misses
                RenderBoard(battleShips1, hits2X, hits2Y, numHits2, misses2X, misses2Y, numMisses2, totalShips);

                // positions for a hit/miss
                String stringY;
                int x = 0;
                int y = 0;

                // looping until a valid guess
                boolean valid = false;  // making sure it's a new guess
                while (!valid)
                {
                    // getting the guess coordinates
                    System.out.print("(number) X\n > ");
                    x = Integer.parseInt(scanner.nextLine()) - 1;
                    System.out.print("(letter) Y\n > ");
                    stringY = scanner.nextLine();

                    // getting the int version of the y coordinates
                    y = GetIndex(abcd, stringY);

                    // looping over all the other guesses to check validity
                    boolean overlapped = false;
                    for (int i = 0; i < numMisses1; i++)
                    {
                        if (misses1X[i] == x && misses1Y[i] == y)
                        {
                            overlapped = true;
                            break;
                        }
                    }
                    for (int i = 0; i < numHits1; i++)
                    {
                        if (hits1X[i] == x && hits1Y[i] == y)
                        {
                            overlapped = true;
                            break;
                        }
                    }

                    if (!overlapped) valid = true;
                }

                // checking for a hit
                boolean hitShip = false;
                // looping over all ships
                for (int i = 0; i < totalShips; i++)
                {
                    // checking if the point is on the given ship
                    if (battleShips2[i].PointCollision(x, y))
                    {
                        // adding a new hit
                        hitShip = true;

                        hits1X[numHits1] = x;
                        hits1Y[numHits1] = y;

                        numHits1++;

                        System.out.println("Hit!!!!!!");

                        // looping over all hits
                        int numPoints = 0;
                        for (int j = 0; j < numHits1; j++)
                        {
                            // checking if the point lies on the ship
                            if (battleShips2[i].PointCollision(hits1X[j], hits1Y[j])) numPoints++;
                        }

                        // checking if there were as many hits on the ship as it is long (if 4 long needs 4 hits)
                        if (numPoints > battleShips2[i].shipLength) System.out.println("Ship SUnk!!");

                        break;
                    }
                }

                // checking if the guess was a miss
                if (!hitShip)
                {
                    // adding a new miss
                    misses1X[numMisses1] = x;
                    misses1Y[numMisses1] = y;

                    numMisses1++;

                    System.out.println("Miss...");
                }

                // setting the turn to the next players
                turn = 1;

                System.out.println("Type Anything To Continue\n > ");
                scanner.nextLine();  // waiting for the player to be ready to continue
            }
            else
            {
                // showing your hits/misses
                RenderBoard(new BattleShip[0], hits2X, hits2Y, numHits2, misses2X, misses2Y, numMisses2, 0);
                System.out.println("ba");  // skipping 1 line
                // showing your ships and the ships you've hit and your misses
                RenderBoard(battleShips2, hits1X, hits1Y, numHits1, misses1X, misses1Y, numMisses1, totalShips);

                // positions for a hit/miss
                String stringY;
                int x = 0;
                int y = 0;

                // looping until a valid guess
                boolean valid = false;  // making sure it's a new guess
                while (!valid)
                {
                    // getting the guess coordinates
                    System.out.print("(number) X\n > ");
                    x = Integer.parseInt(scanner.nextLine()) - 1;
                    System.out.print("(letter) Y\n > ");
                    stringY = scanner.nextLine();

                    // getting the int version of the y coordinates
                    y = GetIndex(abcd, stringY);

                    // looping over all the other guesses to check validity
                    boolean overlapped = false;
                    for (int i = 0; i < numMisses2; i++)
                    {
                        if (misses2X[i] == x && misses2Y[i] == y)
                        {
                            overlapped = true;
                            break;
                        }
                    }
                    for (int i = 0; i < numHits2; i++)
                    {
                        if (hits2X[i] == x && hits2Y[i] == y)
                        {
                            overlapped = true;
                            break;
                        }
                    }

                    if (!overlapped) valid = true;
                }

                // checking for a hit
                boolean hitShip = false;
                // looping over all ships
                for (int i = 0; i < totalShips; i++)
                {
                    // checking if the point is on the given ship
                    if (battleShips1[i].PointCollision(x, y))
                    {
                        // adding a new hit
                        hitShip = true;

                        hits2X[numHits1] = x;
                        hits2Y[numHits1] = y;

                        numHits2++;

                        System.out.println("Hit!!!!!!");

                        // looping over all hits
                        int numPoints = 0;
                        for (int j = 0; j < numHits2; j++)
                        {
                            // checking if the point lies on the ship
                            if (battleShips1[i].PointCollision(hits2X[j], hits2Y[j])) numPoints++;
                        }

                        // checking if there were as many hits on the ship as it is long (if 4 long needs 4 hits)
                        if (numPoints > battleShips1[i].shipLength) System.out.println("Ship SUnk!!");

                        break;
                    }
                }

                // checking if the guess was a miss
                if (!hitShip)
                {
                    // adding a new miss
                    misses2X[numMisses2] = x;
                    misses2Y[numMisses2] = y;

                    numMisses2++;

                    System.out.println("Miss...");
                }

                // setting the turn to the next players
                turn = 0;

                System.out.println("Type Anything To Continue\n > ");
                scanner.nextLine();  // waiting for the player to be ready to continue
            }
        }

        // all the battleships
        //BattleShip[] battleShips1 = GetBattleships(scanner);  // replace the 5 with totalShips
        //BattleShip[] battleShips2 = GetBattleships(scanner);  // replace the 5 with totalShips
    }

    private static int GetIndex(String[] ar, String item)
    {
        for (int i = 0; i < ar.length; i++)
        {
            if (ar[i].equalsIgnoreCase(item)) return i;
        }
        return -1;
    }

    // clearing the screen so you can't cheat
    private static void Clear()
    {
        System.out.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" +
                           "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" +
                           "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    }

    private static BattleShip[] SetupAI()
    {
        // the current battleships and finished battleships
        BattleShip battleShip;
        BattleShip[] ships = new BattleShip[totalShips];

        // looping through all the ships and creating them
        float currentType = 0;
        for (int i = 0; i < totalShips; i++)
        {
            // the current type of ship
            int shipIndex = (int)currentType;

            // looping until the ship is placed
            boolean placed = false;
            while (!placed)
            {
                // finding some random values about the ship
                int r = (int)Math.round(Math.random());
                int x = (int)Math.round(Math.random() * (gridX - 1));
                int y = (int)Math.round(Math.random() * (gridY - 1));

                // getting the length
                int length = shipSizes[shipIndex];
                boolean[] hit = new boolean[length];

                // the size of the ship
                int sizeX = length;
                int sizeY = 0;

                // rotating the ship
                if (r == 0)
                {
                    // changing the rotation of the ship
                    sizeX = 0;
                    sizeY = length;
                }

                // creating a new ship
                battleShip = CreateBattleship(sizeX, sizeY, x, y, length, hit);

                // looping over every placed ship
                boolean collided = false; // checking if the ship fits
                for (int j = 0; j < i; j++)
                {
                    // checking for the collision
                    collided = battleShip.CheckCollisionShip(ships[j]);
                    if (collided) break;
                }

                // checking if the ship collided
                placed = !collided;
                if (placed)
                {
                    // finding the bounds of the ship
                    int x2 = x + sizeX;
                    int y2 = y + sizeY;

                    // checking if the ship is on the board
                    if (x >= 0 && x2 < gridX && y >= 0 && y2 < gridY) ships[i] = battleShip;
                    else placed = false;  // the ship wasn't fully on the board
                }
            }

            // changing the ship type to the new one
            currentType += 1f/(float)numShips[shipIndex];
        }

        // returning all the ships
        return ships;
    }

    private static BattleShip[] GetBattleships(Scanner scanner)
    {
        // all the battleships
        BattleShip[] battleShips = new BattleShip[totalShips];

        // looping over every ship being created
        float currentType = 0f;
        for (int i = 0; i < totalShips; i++)
        {
            // checking the type of ship
            int shipIndex = (int)currentType;

            // the position of the ship
            int xPos = 0;
            int yPos = 0;

            // the length of the ship
            int shipLength = shipSizes[shipIndex];

            // the size of the ship
            int xSize = shipLength;
            int ySize = 0;

            // creating a new ship
            battleShips[i] = CreateBattleship(xSize, ySize, xPos, yPos, shipLength, new boolean[5]);

            // looping until the ship has been placed
            boolean placed = false;
            while (!placed)
            {
                // rendering the board
                RenderBoard(battleShips, new int[0], new int[0], 0, new int[0], new int[0], 0, i + 1);

                PrintShipInfo(shipIndex);  // printing the info on the ship

                // printing the commands/inputs/keybindings
                System.out.println("p to place\nr to rotate\nwasd to move\n > \r");
                String input = scanner.nextLine();

                if (input.equalsIgnoreCase("p"))
                {
                    // looping through all the ships
                    boolean collided = false;
                    for (int j = 0; j < i; j++)
                    {
                        // checking if the ships overlap
                        collided = battleShips[i].CheckCollisionShip(battleShips[j]);
                        if (collided) break;  // leaving the loop, the ships overlap
                    }

                    placed = !collided;  // checking if there was overlap or not
                }
                // moving the ships
                else if (input.equalsIgnoreCase("w") && battleShips[i].CheckMovement(0 , -1, gridX, gridY)) yPos--;
                else if (input.equalsIgnoreCase("a") && battleShips[i].CheckMovement(-1,  0, gridX, gridY)) xPos--;
                else if (input.equalsIgnoreCase("s") && battleShips[i].CheckMovement(0 ,  1, gridX, gridY)) yPos++;
                else if (input.equalsIgnoreCase("d") && battleShips[i].CheckMovement(1 ,  0, gridX, gridY)) xPos++;
                else if (input.equalsIgnoreCase("r"))
                {
                    // rotating the ship
                    int nxs, nys;

                    // checking the current rotation and finding the new one
                    if (ySize == 0)
                    {
                        nxs = 0;
                        nys = shipLength;
                    }
                    else
                    {
                        nxs = shipLength;
                        nys = 0;
                    }

                    // creating a new ship with the rotated position
                    BattleShip newShip = CreateBattleship(nxs, nys, xPos, yPos, shipLength, new boolean[5]);
                    // checking if it is a valid movement (if it is still on the board)
                    if (newShip.CheckMovement(0, 0, gridX, gridY))
                    {
                        // setting the current ship to this new ship
                        xSize = nxs;
                        ySize = nys;
                    }
                }

                // updating the ship
                battleShips[i] = CreateBattleship(xSize, ySize, xPos, yPos, shipLength, new boolean[5]);
            }

            // moving the current type of ship
            currentType += 1f/(float)numShips[shipIndex];
        }

        // returning the array of ships
        return battleShips;
    }

    // prints the info about a given ship
    private static void PrintShipInfo(int shipType)
    {
        // prints the name of the ship
        System.out.println(shipNames[shipType]);
        // prints the size of the ship
        System.out.println("Size: " + (shipSizes[shipType] + 1));  // might need Integer.toString?
    }

    // creates a battleship and initializes it
    private static BattleShip CreateBattleship(int sx, int sy, int px, int py, int l, boolean[] h)
    {
        BattleShip battleShip = new BattleShip();
        battleShip.Init(sx, sy, px, py, l, h);
        return battleShip;
    }

    // renders the board
    private static void RenderBoard(BattleShip[] battleShips, int[] hitsX, int[] hitsY, int hits, int[] missesX, int[] missesY, int misses, int totalShips)
    {
        System.out.println("  1 2 3 4 5 6 7 8 9 10\n ----------------------");  // creating the boarder
        // looping through the layers
        for (int y = 0; y < gridY; y++)
        {
            String line = abcd[y] + "|";  // adding the sides to the boarder
            // looping through a row
            for (int x = 0; x < gridX; x++)
            {
                String piece = "  ";  // starting as water/air/nothing
                // looping through the misses
                for (int i = 0; i < misses; i++)
                {
                    if (missesX[i] == x && missesY[i] == y)  // checking for a miss on the current position
                    {
                        piece = "~~";
                        break;
                    }
                }
                // looping through the ships
                for (int i = 0; i < totalShips; i++)
                {
                    if (battleShips[i].PointCollision(x, y))  // checking for a ship on the current position
                    {
                        piece = "[]";
                        break;
                    }
                }
                // looping through the hits
                for (int i = 0; i < hits; i++)
                {
                    if (hitsX[i] == x && hitsY[i] == y)  // checking for a hit (on a ship) on the current position
                    {
                        piece = "><";
                        break;
                    }
                }
                line += piece;  // adding the piece to the layer/row
            }
            line += "|";  // adding the sides to the boarder
            System.out.println(line);
        }
        // rendering the bottom part of the boarder
        System.out.println(" ----------------------");
    }
}
