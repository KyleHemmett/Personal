package researchsim.entities;

import researchsim.logging.MoveEvent;
import researchsim.map.Coordinate;
import researchsim.map.Tile;
import researchsim.map.TileType;
import researchsim.scenario.ScenarioManager;
import researchsim.util.Collectable;
import researchsim.util.CoordinateOutOfBoundsException;
import researchsim.util.Movable;
import researchsim.util.NoSuchEntityException;

import java.util.ArrayList;
import java.util.List;

import static researchsim.entities.Size.MEDIUM;

/**
 * User is the player controlled character in the simulation.
 * A user can {@code collect} any class that implements the {@link researchsim.util.Collectable}
 * interface.
 * <p>
 * NOTE: Some methods in this class require interaction with the {@link ScenarioManager}. Only
 * interact with it when you need it.
 *
 * @ass2
 * @ass2_test
 */
public class User extends Entity implements Movable {

    /**
     * name of the User
     */
    private final String name;


    /**
     * Creates a user with a given coordinate and name.
     * A user a MEDIUM sized entity.
     *
     * @param coordinate - coordinate associated with the user
     * @param name - the name of this user
     */
    public User(Coordinate coordinate, String name) {
        super(MEDIUM, coordinate);
        this.name = name;
    }


    /**
     * Returns the name of this user.
     *
     * @return human-readable name
     */
    public String getName() {
        return name;
    }

    /**
     * Returns the machine-readable string representation of this user.
     * The format of the string to return is
     *
     * User-coordinate-name
     * Where:
     * coordinate is the encoding of the user's associated coordinate
     * name is the user's name
     * For example:
     * User-3,5-Bob
     *
     * @return encoded string representation of this user
     */
    @Override
    public String encode() {
        return String.format("%s-%s-%s",
                this.getClass().getSimpleName(),
                this.getCoordinate(),
                this.name);
    }


    /**
     * Returns the hash code of this user.
     * Two users that are equal according to the equals(Object) method should
     * have the same hash code.
     *
     * @return hash code of this user.
     */
    @Override
    public int hashCode() {
        return (this.getCoordinate().hashCode() * 3) + (this.name.hashCode() * 17);
    }

    /**
     * Returns true if and only if this user is equal to the other given object.
     * For two users to be equal, they must have the same coordinate and name.
     *
     * @param other - the reference object with which to compare
     * @return true if this user is the same as the other argument; false otherwise
     */
    @Override
    public boolean equals(Object other) {
        if (!(other instanceof User)) {
            return false;
        }
        User otherUser = (User) other;
        return this.getCoordinate().equals(otherUser.getCoordinate())
                && this.name.equals(otherUser.getName());
    }


    /**
     * Returns a List of all the possible coordinates that this user can move
     * to.
     * The possible coordinates that this User can move to are defined as:
     * Any Coordinate in Movable.checkRange(int, Coordinate) (checkRange(move
     * distance, current coordinate)) that this user can move to (See canMove
     * (Coordinate)).
     * Any CoordinateOutOfBoundsException's thrown by canMove(Coordinate) are
     * squashed.
     *
     * The order of the returned coordinates does not matter.
     *
     * @return list of possible movements
     */
    public List<Coordinate> getPossibleMoves() {
        int radius = 3;
        Coordinate initialCoordinate = getCoordinate();
        List<Coordinate> possibleMoves = new ArrayList<Coordinate>();
        List<Coordinate> allMoves = checkRange(radius, initialCoordinate);
        for (Coordinate move : allMoves) {
            try {
                if (canMove(move)) {
                    possibleMoves.add(move);
                }
            } catch (CoordinateOutOfBoundsException e) {
                e.printStackTrace();
            }
        }
        return possibleMoves;
    }


    /**
     * Moves the user to the new coordinate.
     * The Tile that the user moves to should now be occupied by this user.
     * The tile that the user moves from (the existing coordinate) should now
     * have no occupant.
     * A MoveEvent should be created with the animal and new coordinate.
     * If the new coordinate has an entity that implements Collectable then
     * this entity should be collected with its implementation of collect
     * (Coordinate).
     * The move event should be added to the log BEFORE calling collect
     * (Coordinate).
     * Any exceptions that might be raised from collect(Coordinate) should be
     * suppressed.
     *
     * @param coordinate - The new coordinate to move to
     *
     */
    public void move(Coordinate coordinate) {
        MoveEvent move = new MoveEvent(this, coordinate);
        ScenarioManager.getInstance().getScenario().getLog().add(move);
        Tile currentTile =
                ScenarioManager.getInstance().getScenario().getMapGrid()
                        [this.getCoordinate().getIndex()];
        currentTile.setContents(null);
        Tile newTile =
                ScenarioManager.getInstance().getScenario().getMapGrid()
                        [coordinate.getIndex()];
        if (newTile.hasContents()) {
            try {
                if (newTile.getContents() instanceof Collectable) {
                    this.collect(coordinate);
                }
            } catch (NoSuchEntityException | CoordinateOutOfBoundsException e) {
                e.printStackTrace();
            }
        }
        newTile.setContents(this);
        this.setCoordinate(coordinate);

    }

    /**
     * Gets all moves in x Axis
     *
     * @param user current user
     * @param coordinate coordinate to move to
     * @return list of all x moves
     */
    private static List<Integer> movesX(User user, Coordinate coordinate) {
        List<Integer> movesX = new ArrayList<Integer>();
        Coordinate currentCoordinate = user.getCoordinate();
        int destinationX = coordinate.getX();
        int startingX = currentCoordinate.getX();

        if (destinationX > startingX) {
            for (int i = startingX; i < destinationX; i++) {
                movesX.add(i + 1);
            }
        } else if (destinationX < startingX) {
            for (int i = startingX; i > destinationX; i--) {
                movesX.add(i - 1);
            }
        }
        return movesX;
    }

    /**
     * Gets all moves in y Axis
     *
     * @param user current user
     * @param coordinate coordinate to move to
     * @return list of all y moves
     */
    private static List<Integer> movesY(User user, Coordinate coordinate) {
        List<Integer> movesY = new ArrayList<Integer>();
        Coordinate currentCoordinate = user.getCoordinate();
        int destinationY = coordinate.getY();
        int startingY = currentCoordinate.getY();

        if (destinationY > startingY) {
            for (int i = startingY; i < destinationY; i++) {
                movesY.add(i + 1);
            }
        } else if (destinationY < startingY) {
            for (int i = startingY; i > destinationY; i--) {
                movesY.add(i - 1);
            }
        }
        return movesY;
    }

    /**
     * Gets path of coordinates moving x first
     *
     * @param movesX - list of x moves
     * @param movesY - list of y moves
     * @param user - current user
     * @return list of coordinates
     */
    private static List<Coordinate> firstPathX(List<Integer> movesX,
                                              List<Integer> movesY, User user) {
        List<Coordinate> path = new ArrayList<Coordinate>();
        if (movesX.size() != 0) {
            for (Integer i : movesX) {
                Coordinate tmp = new Coordinate(i, user.getCoordinate().getY());
                path.add(tmp);
            }
            for (Integer j : movesY) {
                Coordinate tmp = new Coordinate((movesX.get(movesX.size() - 1)),
                        j);
                path.add(tmp);
            }
        } else {
            for (Integer k : movesY) {
                Coordinate tmp = new Coordinate(user.getCoordinate().getX(), k);
                path.add(tmp);
            }
        }
        return path;
    }

    /**
     * Gets path of coordinates moving y first
     *
     * @param movesX - list of x moves
     * @param movesY - list of y moves
     * @param user - current user
     * @return list of coordinates
     */
    private static List<Coordinate> firstPathY(List<Integer> movesX,
                                              List<Integer> movesY, User user) {
        List<Coordinate> path = new ArrayList<Coordinate>();
        if (movesY.size() != 0) {
            for (Integer i : movesY) {
                Coordinate tmp = new Coordinate(user.getCoordinate().getX(), i);
                path.add(tmp);
            }
            for (Integer j : movesX) {
                Coordinate tmp = new Coordinate(j,
                        (movesY.get(movesY.size() - 1)));
                path.add(tmp);
            }
        } else {
            for (Integer k : movesX) {
                Coordinate tmp = new Coordinate(k, user.getCoordinate().getY());
                path.add(tmp);
            }
        }
        return path;
    }

    /**
     * Checks if the x path passes
     *
     * @param firstPathX - x first path
     * @return true if it passes
     */
    private static boolean checkFirstPathX(List<Coordinate> firstPathX,
                                          User user) throws CoordinateOutOfBoundsException {
        Coordinate currentCoordinate = user.getCoordinate();
        for (Coordinate coordinate : firstPathX) {
            if (currentCoordinate.equals(coordinate)) {
                return false;
            }
            if (!coordinate.isInBounds()) {
                throw new CoordinateOutOfBoundsException("out of bounds");
            }
            Tile coordinateTile =
                    ScenarioManager.getInstance().getScenario().getMapGrid()
                            [coordinate.getIndex()];
            if (coordinateTile.getType() == TileType.OCEAN
                    || (coordinateTile.getType() == TileType.MOUNTAIN)) {
                return false;
            }
        }
        return true;
    }

    /**
     * Checks if the y path passes
     *
     * @param firstPathY - y first path
     * @return true if it passes
     */
    private static boolean checkFirstPathY(List<Coordinate> firstPathY,
                                          User user) throws CoordinateOutOfBoundsException {

        Coordinate currentCoordinate = user.getCoordinate();
        for (Coordinate coordinate : firstPathY) {
            if (currentCoordinate.equals(coordinate)) {
                return false;
            }
            if (!coordinate.isInBounds()) {
                throw new CoordinateOutOfBoundsException("out of bounds");
            }
            Tile coordinateTile =
                    ScenarioManager.getInstance().getScenario().getMapGrid()
                            [coordinate.getIndex()];
            if (coordinateTile.getType() == TileType.OCEAN
                    || (coordinateTile.getType() == TileType.MOUNTAIN)) {
                return false;
            }
        }
        return true;
    }


    /**
     * Determines if the user can move to the specified coordinate.
     * A User can move to the new coordinate if ALL of the following
     * conditions are satisfied:
     *
     * The new coordinate must be different from the current coordinate.
     * The coordinate given is on the scenario map.
     * The distance from the given coordinate to the current coordinate is
     * not greater than four (4)
     * The tile at the coordinate is NOT OCEAN or MOUNTAIN
     * The entity has an unimpeded path (meaning all the above conditions are
     * true) for each tile it must traverse to reach the destination coordinate
     * A user can only turn once, i.e. can not move diagonally but rather n
     * tiles in the horizontal plane followed by m tiles in the vertical
     * plane (or vice versa). Similar to how a knight moves in chess.
     *
     * For example:
     * If the user wants to move from (0,0) to (2,1) on the following encoded
     * map.
     *
     *  LLL
     *  LSL
     *  LLL
     *
     * (The above map is not possible to be created normally as the minimum
     * dimensions are 5 x 5)
     * It would have to be able to move to all the following coordinates:
     * [(1,0),(2,0),(2,1)]
     * OR
     * [(0,1),(1,1),(2,1)]
     *
     * @param coordinate coordinate to check
     * @return true if the instance can move to the specified coordinate else false
     * @throws CoordinateOutOfBoundsException - if the coordinate given is out of bounds
     */
    public boolean canMove(Coordinate coordinate) throws CoordinateOutOfBoundsException {

        List<Integer> movesX = movesX(this, coordinate);
        List<Integer> movesY = movesY(this, coordinate);
        List<Coordinate> firstPathX = firstPathX(movesX, movesY, this);
        List<Coordinate> firstPathY = firstPathY(movesX, movesY, this);
        boolean pathResultX;
        boolean pathResultY;
        Coordinate currentCoordinate = this.getCoordinate();

        if (currentCoordinate.equals(coordinate)) {
            return false;
        }
        try {
            pathResultX = checkFirstPathX(firstPathX, this);
        } catch (CoordinateOutOfBoundsException e) {
            pathResultX = false;
        }
        try {
            pathResultY = checkFirstPathY(firstPathY, this);
        } catch (CoordinateOutOfBoundsException e) {
            pathResultY = false;
        }
        return pathResultX || pathResultY;

    }


    /**
     * Returns a List of all the possible coordinates that this user can
     * collect from.
     * A user can only collect from its immediate neighbouring tiles (only 1
     * tile away) that:
     * Are in the bounds of the current scenario
     * Have contents (Tile.hasContents())
     * If the contents of that tile implement the Collectable interface
     * The User can collect from ANY TileType.
     *
     * The order of the returned coordinates does not matter.
     *
     * @return list of possible collections
     */
    public List<Coordinate> getPossibleCollection() {
        List<Coordinate> possibleCollections = new ArrayList<Coordinate>();
        List<Coordinate> collectCoordinates = checkRange(1,
                this.getCoordinate());
        for (Coordinate collectCoordinate : collectCoordinates) {
            Tile coordinateTile =
                    ScenarioManager.getInstance().getScenario().getMapGrid()
                            [collectCoordinate.getIndex()];
            try {
                if (collectCoordinate.isInBounds() && coordinateTile.hasContents()
                        && (coordinateTile.getContents() instanceof Collectable)) {
                    possibleCollections.add(collectCoordinate);
                }
            } catch (NoSuchEntityException e) {
                e.printStackTrace();
            }
        }
        return possibleCollections;
    }


    /**
     * Collects an entity from the specified coordinate.
     * If the entity at the given coordinate does not implement the
     * Collectable interface then no action is taken.
     *
     * This method should collect the entity even if the Coordinate is more
     * than 1 tile away (getPossibleMoves()).
     *
     * @param coordinate - the coordinate we are collecting from
     * @throws NoSuchEntityException - if the given coordinate is empty
     * @throws CoordinateOutOfBoundsException - if the given coordinate is not in the map bounds.
     */
    public void collect(Coordinate coordinate) throws NoSuchEntityException,
            CoordinateOutOfBoundsException {
        Tile coordinateTile =
                ScenarioManager.getInstance().getScenario().getMapGrid()
                        [coordinate.getIndex()];

        if (coordinateTile.getContents() instanceof Collectable) {
            ((Collectable) coordinateTile.getContents()).collect(this);
        }
    }
}
