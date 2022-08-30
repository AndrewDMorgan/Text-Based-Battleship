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
    static int[]    shipSizes = new int   [] {4, 3, 2, 2, 1};  // 1 smaller than the real size
    // the names of the different types of ships
    static String[] shipNames = new String[] {"Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer"};

    static int totalShips = numCarriers + numBattleships + numCruisers + numSubmarines + numDestroyers;
    static int[] numShips = new int[] {numCarriers, numBattleships, numCruisers, numSubmarines, numDestroyers};

    public static void main(String[] args)
    {
        // creating an instant of Scanner for player input
        Scanner scanner = new Scanner(System.in);

        // all the battleships
        BattleShip[] battleShips1 = GetBattleships(scanner);  // replace the 5 with totalShips
        BattleShip[] battleShips2 = GetBattleships(scanner);  // replace the 5 with totalShips
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

            // looping untill the ship has been placed
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
        System.out.println("----------------------");  // creating the boarder
        // looping through the layers
        for (int y = 0; y < gridY; y++)
        {
            String line = "|";  // adding the sides to the boarder
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
        System.out.println("----------------------");
    }
}
