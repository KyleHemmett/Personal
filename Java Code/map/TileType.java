package researchsim.map;

import researchsim.util.BadSaveException;
import researchsim.util.Encodable;

import java.io.Serializable;

/**
 * Represents the type of terrain that is at a tile. <br>
 * The terrain type restricts the entities that are allowed in inhabit a tile.
 * <p>
 * A characteristic of a tile type is its encoding.
 * <table border="1">
 *      <caption>Enum definitions</caption>
 *     <tr>
 *         <th>TileType</td>
 *         <th>Encoding</td>
 *     </tr>
 *     <tr>
 *         <td>LAND</td>
 *         <td>L</td>
 *     </tr>
 *     <tr>
 *         <td>OCEAN</td>
 *         <td>O</td>
 *     </tr>
 *     <tr>
 *         <td>SAND</td>
 *         <td>S</td>
 *     </tr>
 *     <tr>
 *         <td>MOUNTAIN</td>
 *         <td>X</td>
 *     </tr>
 * </table>
 * <p>
 * For this assignment you will need to add some values to the Java enum class, see the following
 * links.<br>
 * <a href="https://docs.oracle.com/javase/tutorial/java/javaOO/enum.html">
 * Java documentation - Recommended</a>
 * <br> OR <br>
 * <a href="https://www.baeldung.com/java-enum-values">Baeldung - Java enum values (section 1-4)</a>
 * <p>
 * <span style="font-size: 1.2em;">
 * <b>NOTE:</b> You do <b>not</b> need to implement the {@code values()} or
 * {@code valueOf(String)} methods as part of the assignment.
 * Their implementations are generated automatically by the compiler.
 * Also, you do <b>not</b> need to implement the {@code Serializable} or {@code Comparable}
 * interfaces, or extend {@code Enum}.
 * </span>
 *
 * @ass1_partial
 */
public enum TileType implements Serializable, Comparable<TileType>, Encodable {
    /**
     * Generic land area for ground based entities
     *
     * @ass1_partial
     */
    LAND,
    /**
     * Generic sea area for water based entities
     *
     * @ass1_partial
     */
    OCEAN,
    /**
     * Sandy terrain to transition between LAND and SEA or represent a desert.
     *
     * @ass1_partial
     */
    SAND,
    /**
     * A mountain, a rough terrain to inhabit, a good boundary tile.
     *
     * @ass1_partial
     */
    MOUNTAIN;

    /**
     * Returns the decoded instance of the corresponding encoded string.
     *
     * @param encoded - the encoded string containing the TileType
     * @return decoded object
     * @throws BadSaveException - if the given encoding does not match any
     * enum elements encoding attribute
     */
    public static TileType decode(String encoded) throws BadSaveException {
        switch (encoded) {
            case "L":
                return LAND;
            case "O":
                return OCEAN;
            case "S":
                return SAND;
            case "X":
                return MOUNTAIN;
            default:
                throw new BadSaveException();
        }
    }

    /**
     * Returns the machine-readable encoding representation of this TileType.
     *
     * @return encoded String representation
     */
    @Override
    public String encode() {
        switch (this) {
            case LAND:
                return "L";
            case OCEAN:
                return "O";
            case SAND:
                return "S";
            case MOUNTAIN:
                return "X";
            default:
                return null;
        }
    }
}
