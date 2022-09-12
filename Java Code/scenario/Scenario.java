package researchsim.scenario;

import researchsim.entities.Fauna;
import researchsim.entities.Flora;
import researchsim.entities.Size;
import researchsim.entities.User;
import researchsim.logging.Logger;
import researchsim.map.Coordinate;
import researchsim.map.Tile;
import researchsim.map.TileType;
import researchsim.util.BadSaveException;
import researchsim.util.CoordinateOutOfBoundsException;
import researchsim.util.NoSuchEntityException;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.Reader;
import java.util.*;

import static java.lang.Integer.parseInt;


/**
 * The scenario is the overriding class of the simulation.
 * It is similar to a level in a video game.
 * <p>
 * NOTE: Some methods in this class require interaction with the {@link ScenarioManager}. Only
 * interact with it when you need it.
 *
 * @ass1_partial
 */
public class Scenario {

    /**
     * The minimum dimensions of the map grid.
     * The value of this constant is {@value}
     *
     * @ass1
     */
    public static final int MIN_SIZE = 5;
    /**
     * The maximum dimensions of the map grid.
     * The value of this constant is {@value}
     *
     * @ass1
     */
    public static final int MAX_SIZE = 15;
    /**
     * Maximum number of tiles that the grid contains.
     * The value of this constant is {@value}
     *
     * @ass1
     */
    public static final int MAX_TILES = MAX_SIZE * MAX_SIZE;
    /**
     * The name of this scenario.
     */
    private final String name;
    /**
     * The width of the map in the scenario.
     */
    private final int width;
    /**
     * The height of the map in the scenario.
     */
    private final int height;
    /**
     * The tile grid for this scenario.
     */
    private Tile[] mapGrid;

    /**
     * The animal Controller.
     */
    private AnimalController animalController;

    /**
     * The logger for the scenario.
     */
    private Logger logger;

    /**
     * random seed for the scenario.
     */
    private Random random;

    /**
     * the seed of the Scenario
     */
    private int seed;

    /**
     * Creates a new Scenario with a given name, width, height and random seed. <br>
     * A one dimensional (1D) array of tiles is created as the board with the given width and
     * height. <br>
     * An empty Animal Controller and logger is also initialised. <br>
     * An instance of the {@link Random} class in initialised with the given seed.
     *
     * @param name   scenario name
     * @param width  width of the board
     * @param height height of the board
     * @param seed   the random seed for this scenario
     * @throws IllegalArgumentException if width &lt; {@value Scenario#MIN_SIZE} or width &gt;
     *                                  {@value Scenario#MAX_SIZE} or height
     *                                  &lt; {@value Scenario#MIN_SIZE} or height &gt;
     *                                  {@value Scenario#MAX_SIZE} or seed &lt; 0 or name is {@code
     *                                  null}
     * @ass1_partial
     * @see Random (<a href="https://docs.oracle.com/en/java/javase/11/docs/api/java.base/java/util/Random.html">Link</a>)
     */
    public Scenario(String name, int width, int height, int seed)
        throws IllegalArgumentException {
        if (width > MAX_SIZE || width < MIN_SIZE) {
            throw new IllegalArgumentException("The given width does not conform to the "
                + "requirement: " + MIN_SIZE + " <= width <= " + MAX_SIZE + ".");
        }
        if (height > MAX_SIZE || height < MIN_SIZE) {
            throw new IllegalArgumentException("The given height does not conform to the "
                + "requirement: " + MIN_SIZE + " <= height <= " + MAX_SIZE + ".");
        }
        if (name == null) {
            throw new IllegalArgumentException("The given name does not conform to the "
                + "requirement: name != null.");
        }
        if (seed < 0) {
            throw new IllegalArgumentException();
        }
        this.name = name;
        this.width = width;
        this.height = height;
        this.mapGrid = new Tile[width * height];

        animalController = new AnimalController();

        logger = new Logger();

        random = new Random(seed);
        this.seed = seed;
    }

    /**
     * Returns the name of the scenario.
     *
     * @return scenario name
     * @ass1
     */
    public String getName() {
        return name;
    }

    /**
     * Returns the map grid for this scenario.
     * <p>
     * Adding or removing elements from the returned array should not affect the original array.
     *
     * @return map grid
     * @ass1
     */
    public Tile[] getMapGrid() {
        return Arrays.copyOf(mapGrid, getSize());
    }

    /**
     * Updates the map grid for this scenario.
     * <p>
     * Adding or removing elements from the array that was passed should not affect the class
     * instance array.
     *
     * @param map the new map
     * @throws CoordinateOutOfBoundsException (param) map length != size of current scenario map
     * @ass1_partial
     */
    public void setMapGrid(Tile[] map) throws CoordinateOutOfBoundsException {
        if (map.length != getMapGrid().length) {
            throw new CoordinateOutOfBoundsException();
        }
        mapGrid = Arrays.copyOf(map, getSize());
    }

    /**
     * Returns the width of the map for this scenario.
     *
     * @return map width
     * @ass1
     */
    public int getWidth() {
        return width;
    }

    /**
     * Returns the height of the map for this scenario.
     *
     * @return map height
     * @ass1
     */
    public int getHeight() {
        return height;
    }

    /**
     * Returns the size of the map in the scenario.<br>
     * The size of a map is the total number of tiles in the Tile array.
     *
     * @return map size
     * @ass1
     */
    public int getSize() {
        return width * height;
    }

    /**
     * Returns the scenarios random instance.
     *
     * @return random
     */
    public Random getRandom() {
        return random;
    }

    /**
     * Returns the scenario's activity log.
     *
     * @return game log
     */
    public Logger getLog() {
        return logger;
    }


    /**
     * Returns the scenarios enemy manager.
     *
     * @return enemy manager
     */
    public AnimalController getController() {
        return animalController;
    }


    /**
     *Creates a Scenario instance by reading information from the given reader.
     * The provided reader should contain data in the format:
     *
     *  {ScenarioName}
     *  Width:{Width}
     *  Height:{Height}
     *  Seed:{Seed}
     *  {Separator}
     *  {map}
     *  {Separator}
     *  {entity}
     *  {entity...}
     *
     * (As specified by encode())
     * The reader is invalid if any of the following conditions are true:
     *
     * The given reader is empty
     * The reader hits EOF (end of file) before all of the required
     * information is present. The required information is:
     * ScenarioName
     * Width
     * Height
     * Seed
     * map
     * It is not required to have an entity. But a Separator must exist after
     * the map.
     * The required information does NOT appear in the order specified above.
     * If the Width, Height or Seed lines do not contain exactly one (1)
     * colon (":")
     * If any of the Width, Height and Seed keys (before colon) are not
     * "Width", "Height" and "Seed" respectively.
     * If any of the Width, Height and Seed values (after colon) are not a
     * valid integer (i.e. cannot be parsed by Integer.parseInt(String))
     * If any of the ScenarioName, Width, Height and Seed values cause an
     * IllegalArgumentException when used to create a Scenario with the
     * constructor Scenario(String, int, int, int).
     * Scenario(ScenarioName, Width, Height, Seed)
     * A separator does not have exactly the Width value (or its default, see
     * below) number of equals characters ("=").
     * i.e. Width == 5 -> separator == "=====".
     * A separator must appear on the line immediately after the Seed and the
     * last line of the map.
     * If any of the following hold true for a map line:
     * The number of characters on that line is not exactly the Width value
     * (or its default, see below).
     * NOTE: This includes trailing whitespace such as tabs and spaces ( "\t"
     * and " ").
     * If the number of characters provided is not equal to the size of the
     * scenario created with the Width and Height values
     * If ANY character provided can not be parsed by TileType.decode(String)
     * If any of the following hold true for an entity line:
     * The line does not contain the correct number of hyphen ("-")
     * characters for its respective encoding (Fauna.encode(), Entity.encode
     * (), User.encode())
     * The line does not start with "Fauna" or "Flora" or "User"
     * If the Coordinate component can not be decoded by Coordinate.decode
     * (String)
     * If the Coordinate specified already has an Entity assigned.
     * i.e. You can not have multiple entities request the same Coordiante in
     * the reader
     * If line starts with "Fauna" or "Flora" AND the Size component can not
     * be decoded by Size.valueOf(String)
     * If line starts with "Fauna" AND the Habitat component can not be
     * decoded by TileType.valueOf(String)
     * If line starts with "Fauna" AND the Habitat value causes an
     * IllegalArgumentException to be thrown
     * If line starts with "Fauna" AND the Habitat at the Tile specified by
     * the Coordinate is not suitable
     * That is, if Habitat is TileType.OCEAN the Tile's type must be TileType
     * .OCEAN. If the Habitat is TileType.LAND the Tile's type must NOT be
     * TileType.OCEAN.
     * If line starts with "Flora" AND the Tile specified by the Coordinate
     * is not suitable
     * That is, if Tile's type is TileType.OCEAN then it is INVALID.
     * If line starts with "User" AND the Tile specified by the Coordinate is
     * not suitable
     * That is, if Tile's type is TileType.OCEAN or TileType.MOUNTAIN then it
     * is INVALID.
     * If the Width, Height and Seed values are -1 they should be assigned a
     * default value of MIN_SIZE (5).
     *
     * The created Scenario should be added to the ScenarioManager class by
     * calling ScenarioManager.addScenario(Scenario).
     *
     * The created Scenario map should be set to the map as descriped in the
     * Reader.
     *
     * The created entities should be inhabiting the Tiles at the Coordinate
     * specified.
     * (HINT: Make sure that you add the Scenario before this step so that
     * you can utilise the index of the Coordinate by Coordinate.getIndex()
     * or Coordinate.convert(int, int).)
     *
     * For example, the reader could contain:
     *
     *  Example File
     *  Width:-1
     *  Height:6
     *  Seed:20
     *  =====
     *  LLLLS
     *  LLSSO
     *  LLSOO
     *  LLSSS
     *  LLLLL
     *  LLLLL
     *  =====
     *  Fauna-SMALL-1,1-LAND
     *  Flora-LARGE-2,5-LAND
     *
     * Noting that the Width is set to a default value of MIN_SIZE (5) and as
     * such the Map is valid
     * The simplest file would be:
     *
     *  Example File
     *  Width:5
     *  Height:5
     *  Seed:5
     *  =====
     *  LLLLL
     *  LLLLL
     *  LLLLL
     *  LLLLL
     *  LLLLL
     *  =====
     *
     * @param reader - reader from which to load all info (will not be null)
     * @return scenario created by reading from the given reader
     * @throws IOException - if an IOException is encountered when reading
     * from the reader
     * @throws BadSaveException - if the reader contains a line that does not
     * adhere to the rules above (thus indicating that the
     * contents of the reader are invalid)
     */
    public static Scenario load(Reader reader) throws IOException,
            BadSaveException {
        BufferedReader br = new BufferedReader(reader);
        int width;
        int height;
        int seed;
        Scenario scenario;
        String name = br.readLine();
        String next = br.readLine();
        try {
            parseInt(next.substring(6));
        } catch (NumberFormatException | StringIndexOutOfBoundsException e) {
            throw new BadSaveException();
        }
        String[] widthLine = next.split(":");
        if (widthLine.length > 2 || !widthLine[0].equals("Width")) {
            throw new BadSaveException();
        }
        try {
            width = parseInt(widthLine[1]);
        } catch (NumberFormatException ex) {
            throw new BadSaveException();
        }
        if (width == -1) {
            width = MIN_SIZE;
        }
        next = br.readLine();
        try {
            parseInt(next.substring(7));
        } catch (NumberFormatException | StringIndexOutOfBoundsException e) {
            throw new BadSaveException();
        }
        String[] heightLine = next.split(":");
        if (heightLine.length > 2 || !heightLine[0].equals("Height")) {
            throw new BadSaveException();
        }
        try {
            height = parseInt(heightLine[1]);
        } catch (NumberFormatException ex) {
            throw new BadSaveException();
        }
        if (height == -1) {
            height = MIN_SIZE;
        }
        next = br.readLine();
        try {
            parseInt(next.substring(5));
        } catch (NumberFormatException | StringIndexOutOfBoundsException e) {
            throw new BadSaveException();
        }
        String[] seedLine = next.split(":");
        if (seedLine.length > 2 || !seedLine[0].equals("Seed")) {
            throw new BadSaveException();
        }
        try {
            seed = parseInt(seedLine[1]);
        } catch (NumberFormatException ex) {
            throw new BadSaveException();
        }
        if (seed == -1) {
            seed = MIN_SIZE;
        }
        try {
            scenario  = new Scenario(name, width, height, seed);
            ScenarioManager.getInstance().addScenario(scenario);
        } catch (IllegalArgumentException ex) {
            throw new BadSaveException();
        }
        String[] sep = br.readLine().split("");
        if (sep.length != width) {
            throw new BadSaveException();
        }
        String mapLine = br.readLine();
        List<Tile> tiles = new ArrayList<Tile>();
        while (mapLine.charAt(0) != '=') {
            String[] token = mapLine.split("");
            for (String type : token) {
                Tile tmp = new Tile(TileType.decode(type));
                tiles.add(tmp);
            }
            mapLine = br.readLine();
        }
        String[] sep2 = mapLine.split("");
        if (sep2.length != width) {
            throw new BadSaveException();
        }

        String entityLine = br.readLine();
        while (entityLine != null) {
            String[] token = entityLine.split("-");
            switch (token[0]) {
                case "Fauna": {
                    if (token.length != 4) {
                        throw new BadSaveException();
                    }
                    Size size = Size.valueOf(token[1]);
                    Coordinate coordinate = Coordinate.decode(token[2]);
                    TileType habitat = TileType.valueOf(token[3]);
                    int index = coordinate.getIndex();
                    Tile currentTile = tiles.get(index);
                    if (habitat == TileType.OCEAN && currentTile.getType() != TileType.OCEAN) {
                        throw new BadSaveException();
                    }
                    if (habitat == TileType.LAND && currentTile.getType() == TileType.OCEAN) {
                        throw new BadSaveException();
                    }
                    Fauna fauna = new Fauna(size, coordinate, habitat);
                    scenario.getController().addAnimal(fauna);
                    currentTile.setContents(fauna);
                    tiles.set(index, currentTile);
                    break;
                }
                case "Flora": {
                    if (token.length != 3) {
                        throw new BadSaveException();
                    }
                    Size size = Size.valueOf(token[1]);
                    Coordinate coordinate = Coordinate.decode(token[2]);
                    int index = coordinate.getIndex();
                    Tile currentTile = tiles.get(index);
                    if (currentTile.getType() == TileType.OCEAN) {
                        throw new BadSaveException();
                    }
                    Flora flora = new Flora(size, coordinate);
                    currentTile.setContents(flora);
                    tiles.set(index, currentTile);
                    break;
                }
                case "User": {
                    if (token.length != 3) {
                        throw new BadSaveException();
                    }
                    Coordinate coordinate = Coordinate.decode(token[1]);
                    String userName = (token[2]);
                    int index = coordinate.getIndex();
                    Tile currentTile = tiles.get(index);
                    if (currentTile.getType() == TileType.OCEAN
                            || currentTile.getType() == TileType.MOUNTAIN) {
                        throw new BadSaveException();
                    }
                    User user = new User(coordinate, userName);
                    currentTile.setContents(user);
                    tiles.set(index, currentTile);
                    break;
                }
            }
            entityLine = br.readLine();
        }

        Tile[] tileArray = tiles.toArray(new Tile[0]);
        try {
            scenario.setMapGrid(tileArray);
            ScenarioManager.getInstance().addScenario(scenario);
        } catch (CoordinateOutOfBoundsException e) {
            e.printStackTrace();
        }
        return scenario;
    }

    /**
     * Returns true if and only if this scenario is equal to the other given
     * object.
     * For two scenarios to be equal, they must have the same:
     *
     * name
     * width
     * height
     * map contents (The tile array)
     *
     * @param other - the reference object with which to compare
     * @return true if this scenario is the same as the other argument; false otherwise
     */
    @Override
    public boolean equals(Object other) {
        if (!(other instanceof Scenario)) {
            return false;
        }
        Scenario otherScenario = (Scenario) other;

        return name.equals(otherScenario.getName())
                && width == (otherScenario.getWidth())
                && height == (otherScenario.getHeight())
                && Arrays.equals(mapGrid, otherScenario.getMapGrid());
    }

    /**
     * Returns the hash code of this scenario.
     * Two scenarios that are equal according to the equals(Object) method
     * should have the same hash code.
     *
     * @return hash code of this scenario.
     */
    @Override
    public int hashCode() {
        return (this.name.hashCode() * 3) + (this.width * 17) + (this.height * 11)
               + (Arrays.hashCode(this.mapGrid) * 13);
    }


    /**
     * Returns the machine-readable string representation of this Scenario.
     * The format of the string to return is
     *
     *  {ScenarioName}
     *  Width:{Width}
     *  Height:{Height}
     *  Seed:{Seed}
     *  {Separator}
     *  {map}
     *  {Separator}
     *  {entity}
     *  {entity...}
     *
     * Where:
     * {ScenarioName} is the name of the scenario
     * {Width} is the width of the scenario
     * {Height} is the Height of the scenario
     * {Seed} is the seed of the scenario
     * NOTE: There is no whitespace between the ':' and value for the above
     * conditions
     * {Separator} is a string of repeated equals "=" characters where the
     * number of characters is equal to the width of the scenario
     * i.e. width == 5 -> separator == "====="
     * {map} is the tile map grid where:
     * Each tile is represented by its TileType encoding (TileType.encode())
     * A system-dependent line separator is added after Width characters are
     * written
     * (See example below)
     * {entity} is the Entity.encode() of each entity found in the map where:
     * Each entity is added in the order it appears in the array by index (i
     * .e. an entity inhabiting a tile with index 1 appears before an entity
     * inhabiting a tile with index 4
     * A system-dependent line separator is added after entity EXCEPT the
     * last entity
     * For example, a simple scenario with the following attributes:
     * Name - Scenario X
     * Width - 5
     * Height - 5
     * Seed - 0
     * A Mouse located at Coordinate (1,1)
     * See Fauna.getName()
     * The map is as shown in the save
     * Each Tile is represented by its TileType.encode() value of its type
     * would be return the following string
     *  Scenario X
     *  Width:5
     *  Height:5
     *  Seed:0
     *  =====
     *  LLLLS
     *  LLSSO
     *  LLSOO
     *  LLSSS
     *  LLLLL
     *  =====
     *  Fauna-SMALL-1,1-LAND
     *
     * @return encoded string representation of this Scenario
     */
    public String encode() {
        StringBuilder mapString = new StringBuilder();
        StringBuilder entityString = new StringBuilder();
        int count = 0;
        for (Tile tile : mapGrid) {
            mapString.append(tile.getType().encode());

            if ((mapString.length() - count) % (width) == 0) {
                mapString.append('\n');
                count++;
            }
            if (tile.hasContents()) {
                try {
                    entityString.append(tile.getContents().encode());
                    entityString.append('\n');
                } catch (NoSuchEntityException e) {
                    e.printStackTrace();
                }
            }
        }
        if (entityString.length() != 0) {
            return String.format("%s" + System.lineSeparator()
                    + "Width:%s" + System.lineSeparator()
                    + "Height:%s" + System.lineSeparator()
                    + "Seed:%s" + System.lineSeparator()
                    + "=".repeat(width)
                    + System.lineSeparator()
                    + "%s"
                    + "=".repeat(width) + System.lineSeparator()
                    + "%s", name, width, height, seed, mapString, entityString);
        } else {
            return String.format("%s" + System.lineSeparator()
                    + "Width:%s" + System.lineSeparator()
                    + "Height:%s" + System.lineSeparator()
                    + "Seed:%s" + System.lineSeparator()
                    + "=".repeat(width)
                    + System.lineSeparator()
                    + "%s"
                    + "=".repeat(width), name, width, height, seed, mapString);
        }

    }

    /**
     * Returns the human-readable string representation of this scenario.
     * <p>
     * The format of the string to return is:
     * <pre>
     *     (name)
     *     Width: (width), Height: (height)
     *     Entities: (entities)
     * </pre>
     * Where:
     * <ul>
     *   <li>{@code (name)} is the scenario's name</li>
     *   <li>{@code (width)} is the scenario's width</li>
     *   <li>{@code (height)} is the scenario's height</li>
     *   <li>{@code (entities)} is the number of entities currently on the map in the scenario</li>
     * </ul>
     * For example:
     *
     * <pre>
     *     Beach retreat
     *     Width: 6, Height: 5
     *     Entities: 4
     * </pre>
     * <p>
     * Each line should be separated by a system-dependent line separator.
     *
     * @return human-readable string representation of this scenario
     * @ass1
     */
    @Override
    public String toString() {
        StringJoiner result = new StringJoiner(System.lineSeparator());
        result.add(name);
        result.add(String.format("Width: %d, Height: %d", width, height));
        result.add(String.format("Entities: %d",
            Arrays.stream(mapGrid).filter(Objects::nonNull).filter(Tile::hasContents).count()));
        return result.toString();
    }
}
