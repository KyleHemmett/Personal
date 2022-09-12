package researchsim.util;

import researchsim.entities.Size;
import researchsim.logging.MoveEvent;
import researchsim.map.Coordinate;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.jar.JarOutputStream;

/**
 * Denotes a specific type of entity that can move around in the simulation.
 * <p>
 * <b>NOTE:</b> <br> Read the documentation for these methods well.
 * This is one of the harder parts of A2.
 * <br> It is recommended that you create some private helper methods to assist with these
 * functions. <br> Some example methods might be: <br> checkTile - see if a Tile can be moved to
 * <br> checkTraversal - see if all tiles along a path can be moved to
 *
 * @ass2
 */
public interface Movable {

    /**
     * Returns a List of all the possible coordinates that the entity can
     * move to.
     * The possible coordinates that the entity can move to are defined as:
     * Any Coordinate in checkRange(int, Coordinate) (checkRange(move
     * distance, current coordinate)) that the entity can move to (See
     * canMove(Coordinate)).
     * Any CoordinateOutOfBoundsException's thrown by canMove(Coordinate) are
     * squashed.
     *
     * The order of the returned coordinates does not matter.
     *
     * @return list of possible movements
     */
    List<Coordinate> getPossibleMoves();


    /**
     * Move the entity from its current Coordinate to the given coordinate.
     * A MoveEvent should be created and added to the current scenario logger.
     *
     * @param coordinate - The new coordinate to move to
     * Requires:
     * the entity canMove(Coordinate) == true
     * Ensures:
     * the state of the tile that the entity is inhabiting, the tile that the
     * entity is going to are both updated, the event is logged
     */
    void move(Coordinate coordinate);


    /**
     *Determines if the entity can move to the specified coordinate.
     * An entity can move to the new coordinate if ALL of the following
     * conditions are satisfied:
     *
     * The coordinate given is on the scenario map.
     * The distance from the given coordinate to the current coordinate is
     * not greater than the distance the entity can move (Size.moveDistance)
     * The tile at the coordinate is NOT uninhabitable by the entity (See
     * implementing class documentation)
     * The tile at the coordinate is not already occupied
     * The entity has an unimpeded path (meaning all the above conditions are
     * true) for each tile it must traverse to reach the destination coordinate
     * An entity can only turn once.
     * i.e. can not move diagonally but rather n tiles in the horizontal
     * plane followed by m tiles in the vertical plane (or vice versa).
     * Similar to how a knight moves in chess.
     *
     * @param coordinate coordinate to check
     * @return - true if the instance can move to the specified
     * coordinate
     * else false
     * @throws CoordinateOutOfBoundsException - if the coordinate given
     * is out of bounds
     */
    boolean canMove(Coordinate coordinate) throws CoordinateOutOfBoundsException;


    /**
     * Return a list of coordinates that fall into the radius (range) of the
     * specified coordinate.
     * A coordinate is in range if the distance (number of tiles) required to
     * travel to reach this new coordinate is less than or equal (≤) to the
     * radius.
     *
     * The order of the returned coordinates does not matter.
     *
     * The Coordinates do not have to be in the bounds of the current scenario.
     *
     * The returned list should include the initial Coordinate.
     *
     * As an example. WIth an initial coordinate of (0,0) and a radius of 1
     * the following Coordinates should be in the list.
     *
     * (0,0)
     * (1,0)
     * (0,1)
     * (-1,0)
     * (0,-1)
     * As a hint in the correct direction: you can easily use a for loop to
     * generate coordinates in a square with the initial coordinate as the
     * centre, from here you can exclude any that are too far away.
     *
     * Note that this is a default method.
     *
     * @param radius - the number of tiles that the entity can move
     * @param initialCoordinate - the starting coordinate of the entity
     * @return - a List of coordinates that the entity can move to.
     */
    default List<Coordinate> checkRange(int radius,
                                        Coordinate initialCoordinate) {
        List<Coordinate> availableMoves = new ArrayList<Coordinate>();
        for (int i = initialCoordinate.getX() - radius;
             i <= initialCoordinate.getX() + radius; i++) {
            for (int j = initialCoordinate.getY() - radius;
                 j <= initialCoordinate.getY() + radius; j++) {
                Coordinate tmp = new Coordinate(i, j);
                Coordinate distanceCoordinate = initialCoordinate.distance(tmp);
                int distance =
                        distanceCoordinate.getAbsX() + distanceCoordinate.getAbsY();
                if (distance <= radius) {
                    availableMoves.add(tmp);
                }
            }
        }
        return availableMoves;
    }
}
