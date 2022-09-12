package researchsim.map;

import researchsim.entities.Entity;
import researchsim.scenario.Scenario;
import researchsim.scenario.ScenarioManager;
import researchsim.util.BadSaveException;

/**
 * A coordinate is a representation of the  X and Y positions on a graphical map.<br>
 * This X, Y position can be used to calculate the index of a Tile in the scenario tile map
 * depending on the currently active scenario. <br>
 * The X and Y positions will not change but the index will depending on the current scenario.
 * <p>
 * A coordinate is similar to a point on the cartesian plane.
 * <p>
 * NOTE: Some methods in this class require interaction with the {@link ScenarioManager}. Only
 * interact with it when you need it.
 *
 * @ass1_partial
 * @ass1_test_partial
 */
public class Coordinate {

    /**
     * The position in the Horizontal plane (Left-Right).
     */
    private final int xcoord;

    /**
     * The position in the Vertical plane (Up-Down).
     */
    private final int ycoord;

    /**
     * Creates a new coordinate at the top left position (0,0), index 0 (zero).
     *
     * @ass1
     */
    public Coordinate() {
        this(0, 0);
    }

    /**
     * Creates a new coordinate at the specified (x,y) position.
     *
     * @param xcoord horizontal position
     * @param ycoord vertical position
     * @ass1
     */
    public Coordinate(int xcoord, int ycoord) {
        this.xcoord = xcoord;
        this.ycoord = ycoord;
    }

    /**
     * Creates a new coordinate at the specified index.
     *
     * @param index index in the tile grid
     * @ass1
     */
    public Coordinate(int index) {
        int width = ScenarioManager.getInstance().getScenario().getWidth();
        this.xcoord = index % width;
        this.ycoord = index / width;
    }

    /**
     * The position in the Horizontal plane (Left-Right)
     *
     * @return the horizontal position
     * @ass1
     */
    public int getX() {
        return xcoord;
    }

    /**
     * The position in the Horizontal plane (Left-Right) absolute value.
     *
     * @return the absolute horizontal position
     */
    public int getAbsX() {
        return java.lang.Math.abs(xcoord);
    }

    /**
     * The position in the Vertical plane (Up-Down)
     *
     * @return the vertical position
     * @ass1
     */
    public int getY() {
        return ycoord;
    }


    /**
     *The position in the Vertical plane (Up-Down) absolute value.
     *
     * @return the absolute vertical position
     */
    public int getAbsY() {
        return java.lang.Math.abs(ycoord);
    }

    /**
     * The index in the tile grid of this coordinate.
     *
     * @return the grid index
     * @ass1
     */
    public int getIndex() {
        return Coordinate.convert(xcoord, ycoord);
    }

    /**
     * Determines if the coordinate in the bounds of the current scenario map
     *
     * @return true, if 0 &le; coordinate's x position &lt; current scenarios' width AND 0 &le;
     * coordinate's y position &lt; current scenarios' height
     * else, false
     * @ass1
     */
    public boolean isInBounds() {
        Scenario scenario = ScenarioManager.getInstance().getScenario();
        return xcoord < scenario.getWidth() && xcoord >= 0
            && ycoord < scenario.getHeight() && ycoord >= 0;
    }

    /**
     * Utility method to convert an (x,y) integer pair to an array index location.
     *
     * @param xcoord the x portion of a coordinate
     * @param ycoord the y portion of a coordinate
     * @return the converted index
     * @ass1
     */
    public static int convert(int xcoord, int ycoord) {
        return xcoord + ycoord * ScenarioManager.getInstance().getScenario().getWidth();
    }


    /**
     *Returns a new Coordinate from the given encoded string.
     * The format of the string should match the encoded representation of a
     * Coordinate, as described in encode().
     *
     * The encoded string is invalid if any of the following conditions are
     * true:
     *
     * The number of commas (,) detected was more/fewer than expected
     * The x component of the Coordinate can NOT be parsed as an Integer
     * The y component of the Coordinate can NOT be parsed as an Integer
     *
     * @param encoded - the encoded coordinate string
     * @return decoded Coordinate
     * @throws BadSaveException - if the format of the given string is
     * invalid according to the rules above
     */
    public static Coordinate decode(String encoded) throws BadSaveException {
        String[] tokens = encoded.split(",");
        int newX;
        int newY;
        if (tokens.length < 2) {
            throw new BadSaveException();
        }
        try {
            newX = Integer.parseInt(tokens[0]);
        } catch (NumberFormatException ex) {
            throw new BadSaveException();
        }

        try {
            newY = Integer.parseInt(tokens[1]);
        } catch (NumberFormatException ex) {
            throw new BadSaveException();
        }

        return new Coordinate(newX, newY);
    }

    /**
     * Returns a special Coordinate pair showing the difference between the
     * current instance and the other coordinate.
     * If the current coordinate was: (5,10) and the other coordinate was:
     * (3,2) the resultant coordinate is: (-2, -8)
     *
     * This can be explicitly defined as:
     *      result X = other X - this X
     *      result Y = other Y - this Y
     *
     *
     * @param other - coordinate to compare
     * @return special difference Coordinate pair
     */
    public Coordinate distance(Coordinate other) {
        return new Coordinate(other.getX() - this.xcoord,
                other.getY() - this.ycoord);
    }

    /**
     * Translate the coordinate the given amount of tiles in the x and y
     * direction.
     * For example:
     * new Coordinate(0,-1).translate(1,3) == new Coordinate(1,2)
     *
     * @param x - translation in the x- axis
     * @param y - translation in the y- axis
     * @return new coordinate location
     */
    public Coordinate translate(int x, int y) {
        return new Coordinate(this.xcoord + x, this.ycoord + y);
    }

    /**
     * Returns the machine-readable string representation of this Coordinate.
     * The format of the string to return is
     *
     * x,y
     * Where:
     * x is the position in the Horizontal plane (Left-Right)
     * y is the position in the Vertical plane (Up-Down)
     * For example:
     * 1,3
     *
     * @return encoded string representation of this Coordinate.
     */
    public String encode() {
        return String.format("%s,%s",
                xcoord,
                ycoord);
    }

    /**
     * Returns the hash code of this coordinate.
     * Two coordinates that are equal according to the equals(Object) method
     * should have the same hash code.
     *
     * @return hash code of this coordinate.
     */
    @Override
    public int hashCode() {
        return (this.xcoord * 3) + (this.ycoord * 17);
    }

    /**
     * Returns true if and only if this coordinate is equal to the other
     * given coordinate.
     * For two coordinates to be equal, they must have the same x and y
     * position.
     *
     * @param other - the reference object with which to compare
     * @return true if this coordinate is the same as the other argument; false otherwise
     */
    @Override
    public boolean equals(Object other) {
        if (!(other instanceof Coordinate)) {
            return false;
        }
        Coordinate otherCoordinate = (Coordinate) other;
        return this.xcoord == (otherCoordinate.getX())
                && this.ycoord == (otherCoordinate.getY());
    }

    /**
     * Returns the human-readable string representation of this Coordinate.
     * <p>
     * The format of the string to return is:
     * <pre>(x,y)</pre>
     * Where:
     * <ul>
     *   <li>{@code x} is the position in the Horizontal plane (Left-Right)</li>
     *   <li>{@code y} is the position in the Vertical plane (Up-Down)</li>
     * </ul>
     * For example:
     *
     * <pre>(1,3)</pre>
     *
     * @return human-readable string representation of this Coordinate.
     * @ass1
     */
    @Override
    public String toString() {
        return String.format("(%d,%d)",
            this.xcoord, this.ycoord);
    }
}
