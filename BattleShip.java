
// a class to store and act on data for individual battleships
class BattleShip
{
    // the different used variables
    public int sizeX, sizeY, posX, posY, shipLength;
    public boolean hit[];

    // a function to initialize the variables
    public void Init(int sizeX_, int sizeY_, int posX_, int posY_, int shipLength_, boolean[] hit_)
    {
        sizeX = sizeX_;
        sizeY = sizeY_;
        posX = posX_;
        posY = posY_;
        shipLength = shipLength_;

        boolean hit[] = hit_;
    }

    // checks for a point colliding with the ship
    public boolean PointCollision(int x, int y)
    {
        // using inequalities to check for collision with the bounding rect of the ship
        boolean xCol = x >= posX && x <= posX + sizeX;
        boolean yCol = y >= posY && y <= posY + sizeY;
        return xCol && yCol;
    }

    public boolean CheckMovement(int dx, int dy, int gridX, int gridY)
    {
        int x1 = posX + dx;
        int y1 = posY + dy;

        int x2 = x1 + sizeX;
        int y2 = y1 + sizeY;

        if (x1 >= 0 && x1 < gridX && x2 >= 0 && x2 < gridX)
        {
            if (y1 >= 0 && y1 < gridY && y2 >= 0 && y2 < gridY)
            {
                return true;
            }
        }
        return false;
    }

    public boolean CheckCollisionShip(BattleShip battleShip)
    {
        // making sure both x points don't collide along a 1D line (ship 1)
        boolean ship1X1 = (posX         >= battleShip.posX) && (posX         >= battleShip.posX + battleShip.sizeX);
        boolean ship1X2 = (posX + sizeX >= battleShip.posX) && (posX + sizeX >= battleShip.posX + battleShip.sizeX);

        // making sure both y points don't collide along a 1D line (ship 1)
        boolean ship1Y1 = (posY         >= battleShip.posY) && (posY         >= battleShip.posY + battleShip.sizeY);
        boolean ship1Y2 = (posY + sizeY >= battleShip.posY) && (posY + sizeY >= battleShip.posY + battleShip.sizeY);

        // making sure both x points don't collide along a 1D line (ship 2)
        boolean ship2X1 = (battleShip.posX                    >= posX) && (battleShip.posX                    >= posX + sizeX);
        boolean ship2X2 = (battleShip.posX + battleShip.sizeX >= posX) && (battleShip.posX + battleShip.sizeX >= posX + sizeX);

        // making sure both y points don't collide along a 1D line (ship 2)
        boolean ship2Y1 = (battleShip.posY                    >= posY) && (battleShip.posY                    >= posY + sizeY);
        boolean ship2Y2 = (battleShip.posY + battleShip.sizeY >= posY) && (battleShip.posY + battleShip.sizeY >= posY + sizeY);

        // checking if the two ships overlap
        boolean collided = (ship1X1 || ship1X2 || ship2X1 || ship2X2) && (ship1Y1 || ship1Y2 || ship2Y1 || ship2Y2);

        // returning the value of collided
        return collided;
    }
}
