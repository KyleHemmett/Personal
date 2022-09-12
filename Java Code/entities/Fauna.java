package researchsim.entities;

import researchsim.logging.CollectEvent;
import researchsim.logging.MoveEvent;
import researchsim.map.Coordinate;
import researchsim.map.Tile;
import researchsim.map.TileType;
import researchsim.scenario.AnimalController;
import researchsim.scenario.ScenarioManager;
import researchsim.util.Collectable;
import researchsim.util.CoordinateOutOfBoundsException;
import researchsim.util.Movable;
import researchsim.util.NoSuchEntityException;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;


/**
 * Fauna is all the animal life present in a particular region or time.
 * Fauna can move around the scenario and be collected by the {@link User}.
 * <p>
 * NOTE: Some methods in this class require interaction with the {@link ScenarioManager}. Only
 * interact with it when you need it.
 *
 * @ass1_partial
 * @ass1_test
 */
public class Fauna extends Entity implements Movable, Collectable {

    /**
     * The habitat associated with the animal.
     * That is, what tiles an animal can exist in.
     */
    private final TileType habitat;

    /**
     * Creates a fauna (Animal) with a given size, coordinate and habitat.
     *
     * @param size       size associated with the animal
     * @param coordinate coordinate associated with the animal
     * @param habitat    habitat tiles associated with the animal
     * @throws IllegalArgumentException if habitat is not {@link TileType#LAND} or
     *                                  {@link TileType#OCEAN}
     * @ass1
     */
    public Fauna(Size size, Coordinate coordinate, TileType habitat)
        throws IllegalArgumentException {
        super(size, coordinate);
        if (habitat != TileType.LAND && habitat != TileType.OCEAN) {
            throw new IllegalArgumentException("Animal was created with a bad habitat: " + habitat);
        }
        this.habitat = habitat;
    }

    /**
     * Returns the animal's habitat.
     *
     * @return animal's habitat
     * @ass1
     */
    public TileType getHabitat() {
        return habitat;
    }

    /**
     * Returns the human-readable name of this animal.
     * The name is determined by the following table.
     * <p>
     * <table border="1">
     *     <caption>Human-readable names</caption>
     *     <tr>
     *         <td rowspan="2" colspan="2" style="background-color:#808080">&nbsp;</td>
     *         <td colspan="3">Habitat</td>
     *     </tr>
     *     <tr>
     *         <td>LAND</td>
     *         <td>OCEAN</td>
     *     </tr>
     *     <tr>
     *         <td rowspan="4">Size</td>
     *         <td>SMALL</td>
     *         <td>Mouse</td>
     *         <td>Crab</td>
     *     </tr>
     *     <tr>
     *         <td>MEDIUM</td>
     *         <td>Dog</td>
     *         <td>Fish</td>
     *     </tr>
     *     <tr>
     *         <td>LARGE</td>
     *         <td>Horse</td>
     *         <td>Shark</td>
     *     </tr>
     *     <tr>
     *         <td>GIANT</td>
     *         <td>Elephant</td>
     *         <td>Whale</td>
     *     </tr>
     * </table>
     * <p>
     * e.g. if this animal is {@code MEDIUM} in size and has a habitat of {@code LAND} then its
     * name would be {@code "Dog"}
     *
     * @return human-readable name
     * @ass1
     */
    @Override
    public String getName() {
        String name;
        switch (getSize()) {
            case SMALL:
                name = habitat == TileType.LAND ? "Mouse" : "Crab";
                break;
            case MEDIUM:
                name = habitat == TileType.LAND ? "Dog" : "Fish";
                break;
            case LARGE:
                name = habitat == TileType.LAND ? "Horse" : "Shark";
                break;
            case GIANT:
            default:
                name = habitat == TileType.LAND ? "Elephant" : "Whale";
        }
        return name;
    }

    /**
     * Returns the machine-readable string representation of this animal.
     * The format of the string to return is
     *
     * Fauna-size-coordinate-habitat
     * Where:
     * size is the animal's associated size
     * coordinate is the encoding of the animal's associated coordinate
     * habitat is the animal's associated habitat
     * For example:
     * Fauna-SMALL-4,6-LAND
     *
     * @return - encoded string representation of this animal
     */
    @Override
    public String encode() {
        return String.format("%s-%s",
            super.encode(),
            this.habitat);
    }

    /**
     *Returns the hash code of this animal.
     * Two animals that are equal according to the equals(Object) method
     * should have the same hash code.
     *
     * @return - hash code of this animal.
     */
    @Override
    public int hashCode() {
        return super.hashCode() + (this.habitat.hashCode() * 7);
    }


    /**
     * Returns true if and only if this animal is equal to the other given
     * object.
     * For two animals to be equal, they must have the same size, coordinate
     * and habitat.
     *
     * @param other - the reference object with which to compare
     * @return - true if this animal is the same as the other argument; false otherwise
     */
    @Override
    public boolean equals(Object other) {
        if (!(other instanceof Fauna)) {
            return false;
        }
        Fauna otherFauna = (Fauna) other;
        return this.getCoordinate().equals(otherFauna.getCoordinate())
                && this.getSize().equals(otherFauna.getSize())
                && this.getHabitat().equals(otherFauna.getHabitat());
    }


    /**
     * Returns the human-readable string representation of this animal.
     * <p>
     * The format of the string to return is:
     * <pre>name [Fauna] at coordinate [habitat]</pre>
     * Where:
     * <ul>
     *   <li>{@code name} is the animal's human-readable name according to {@link #getName()}</li>
     *   <li>{@code coordinate} is the animal's associated coordinate in human-readable form</li>
     *   <li>{@code habitat} is the animal's associated habitat</li>
     *
     * </ul>
     * For example:
     *
     * <pre>Dog [Fauna] at (2,5) [LAND]</pre>
     *
     * @return human-readable string representation of this animal
     * @ass1
     */
    @Override
    public String toString() {
        return String.format("%s [%s]",
            super.toString(),
            this.habitat);
    }


    /**
     * Returns a List of all the possible coordinates that this animal can
     * move to.
     * The possible coordinates that this animal can move to are defined as:
     * Any Coordinate in Movable.checkRange(int, Coordinate) (checkRange(move
     * distance, current coordinate)) that this animal can move to (See
     * canMove(Coordinate)).
     * Any CoordinateOutOfBoundsException's thrown by canMove(Coordinate) are
     * squashed.
     *
     * The order of the returned coordinates does not matter.
     *
     * @return - list of possible movements
     */
    public List<Coordinate> getPossibleMoves() {
        int radius = this.getSize().moveDistance;
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
     * Moves the animal to the new coordinate.
     * The Tile that the animal moves to should now be occupied by this animal.
     *
     * The tile that the animal moves from (the existing coordinate) should
     * now have no occupant.
     *
     * A MoveEvent should be created and added to the current scenario logger.
     *
     * @param coordinate - The new coordinate to move to
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
        newTile.setContents(this);
        this.setCoordinate(coordinate);

    }

    /**
     * Gets all moves in x Axis
     *
     * @param fauna current fauna
     * @param coordinate coordinate to move to
     * @return list of all x moves
     */
    private static List<Integer> movesAxisX(Fauna fauna,
                                            Coordinate coordinate) {
        List<Integer> movesAxisX = new ArrayList<Integer>();
        Coordinate currentCoordinate = fauna.getCoordinate();
        int destinationX = coordinate.getX();
        int startingX =
                currentCoordinate.getX();

        if (destinationX > startingX) {
            for (int i = startingX; i < destinationX; i++) {
                movesAxisX.add(i + 1);
            }
        } else if (destinationX < startingX) {
            for (int i = startingX; i > destinationX; i--) {
                movesAxisX.add(i - 1);
            }
        }
        return movesAxisX;
    }

    /**
     * Gets all moves in y Axis
     *
     * @param fauna current fauna
     * @param coordinate coordinate to move to
     * @return list of all y moves
     */
    private static List<Integer> moveAxisY(Fauna fauna,
                                               Coordinate coordinate) {
        List<Integer> moveAxisY = new ArrayList<Integer>();
        Coordinate currentCoordinate = fauna.getCoordinate();
        int destinationY = coordinate.getY();
        int startingY = currentCoordinate.getY();

        if (destinationY > startingY) {
            for (int i = startingY; i < destinationY; i++) {
                moveAxisY.add(i + 1);
            }
        } else if (destinationY < startingY) {
            for (int i = startingY; i > destinationY; i--) {
                moveAxisY.add(i - 1);
            }
        }

        return moveAxisY;
    }


    /**
     * Gets path of coordinates moving x first
     *
     * @param movesAxisX - list of x moves
     * @param moveAxisY - list of y moves
     * @param fauna - current fauna
     * @return list of coordinates
     */
    private static List<Coordinate> firstPathX(List<Integer> movesAxisX,
                                               List<Integer> moveAxisY,
                                               Fauna fauna) {
        List<Coordinate> path = new ArrayList<Coordinate>();
        if (movesAxisX.size() != 0) {
            for (Integer i : movesAxisX) {
                Coordinate tmp = new Coordinate(i, fauna.getCoordinate().getY());
                path.add(tmp);
            }
            for (Integer j : moveAxisY) {
                Coordinate tmp =
                        new Coordinate((movesAxisX.get(movesAxisX.size() - 1)),
                                j);
                path.add(tmp);
            }
        } else {
            for (Integer k : moveAxisY) {
                Coordinate tmp = new Coordinate(fauna.getCoordinate().getX(), k);
                path.add(tmp);
            }
        }
        return path;
    }

    /**
     * Gets path of coordinates moving y first
     *
     * @param movesAxisX - list of x moves
     * @param moveAxisY - list of y moves
     * @param fauna - current fauna
     * @return list of coordinates
     */
    private static List<Coordinate> firstPathY(List<Integer> movesAxisX,
                                              List<Integer> moveAxisY,
                                              Fauna fauna) {
        List<Coordinate> path = new ArrayList<Coordinate>();
        if (moveAxisY.size() != 0) {
            for (Integer i : moveAxisY) {
                Coordinate tmp = new Coordinate(fauna.getCoordinate().getX(), i);
                path.add(tmp);
            }
            for (Integer j : movesAxisX) {
                Coordinate tmp = new Coordinate(j,
                        (moveAxisY.get(moveAxisY.size() - 1)));
                path.add(tmp);
            }
        } else {
            for (Integer k : movesAxisX) {
                Coordinate tmp = new Coordinate(k, fauna.getCoordinate().getY());
                path.add(tmp);
            }
        }
        return path;
    }

    /**
     * Checks if the x path passes
     *
     * @param firstPathX - x first path
     * @return true if iy passes
     */
    private static boolean checkFirstPathX(List<Coordinate> firstPathX,
                                          Fauna fauna) throws CoordinateOutOfBoundsException {
        Coordinate currentCoordinate = fauna.getCoordinate();
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
            if (fauna.getHabitat() == TileType.OCEAN
                    && (coordinateTile.getType() != TileType.OCEAN)) {
                return false;
            }
            if (fauna.getHabitat() == TileType.LAND
                    && (coordinateTile.getType() == TileType.OCEAN)) {
                return false;
            }
            if (coordinateTile.hasContents()) {
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
                                          Fauna fauna) throws CoordinateOutOfBoundsException {

        Coordinate currentCoordinate = fauna.getCoordinate();
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
            if (fauna.getHabitat() == TileType.OCEAN
                    && (coordinateTile.getType() != TileType.OCEAN)) {
                return false;
            }
            if (fauna.getHabitat() == TileType.LAND
                    && (coordinateTile.getType() == TileType.OCEAN)) {
                return false;
            }
            if (coordinateTile.hasContents()) {
                return false;
            }
        }
        return true;
    }


    /**
     * Determines if the animal can move to the new coordinate.
     * An animal can move to the new coordinate if ALL of the following
     * conditions are satisfied:
     *
     * The new coordinate must be different from the current coordinate.
     * The coordinate given is on the current scenario map (See
     * ScenarioManager).
     * The distance from the given coordinate to the current coordinate is
     * not greater than the distance this animal can move (Size.moveDistance)
     * If the animal's habitat is OCEAN then the tile at the coordinate must
     * be OCEAN
     * If the animal's habitat is LAND then the tile at the coordinate must
     * NOT be OCEAN
     * The tile at the coordinate is not already occupied
     * The animal has an unimpeded path (meaning all the above conditions are
     * true) for each tile it must traverse to reach the destination coordinate
     * The animal can only turn once.
     * i.e. can not move diagonally but rather n tiles in the horizontal
     * plane followed by m tiles in the vertical plane (or vice versa).
     * Similar to how a knight moves in chess.
     *
     * For example:
     * If the animal wants to move from (0,0) to (2,1) on the following
     * encoded map.
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
     * @return if it can move
     * @throws CoordinateOutOfBoundsException - if the coordinate given is out of bounds
     */
    public boolean canMove(Coordinate coordinate) throws CoordinateOutOfBoundsException {
        List<Integer> movesAxisX = movesAxisX(this, coordinate);
        List<Integer> moveAxisY = moveAxisY(this, coordinate);

        List<Coordinate> firstPathX = firstPathX(movesAxisX, moveAxisY, this);
        List<Coordinate> firstPathY = firstPathY(movesAxisX, moveAxisY, this);
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
     * A User interacts and collects this animal.
     * Upon collection the following should occur:
     * A CollectEvent should be created with the animal and coordinate.
     * The Tile that the animal was occupying should now be unoccupied (empty).
     * The animal should be removed from the current scenario's animal
     * controller.
     * The given user will gain a number of points for collecting this animal
     * . This value is determined by the animals size.
     *
     * @param user - the user that collects the entity.
     * @return points earned
     */
    public int collect(User user) {
        CollectEvent collect = new CollectEvent(user, this);
        ScenarioManager.getInstance().getScenario().getLog().add(collect);
        Tile currentTile =
                ScenarioManager.getInstance().getScenario().getMapGrid()
                        [this.getCoordinate().getIndex()];
        currentTile.setContents(null);
        AnimalController animalController =
                ScenarioManager.getInstance().getScenario().getController();

        animalController.removeAnimal(this);

        return this.getSize().points;
    }

}































