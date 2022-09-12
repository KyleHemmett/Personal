package researchsim.entities;

import researchsim.map.Coordinate;
import researchsim.util.Encodable;

import java.sql.PreparedStatement;
import java.util.Objects;

/**
 * An entity is an item that can exist as an inhabitant of a tile. <br>
 * They will always have an associated size and coordinate. <br>
 * This governs the key attributes and methods of every entity on the Scenario grid. <br>
 *
 * @ass1_partial
 * @see researchsim.map.Tile
 */
public abstract class Entity {

    /**
     * Size associated with the entity.
     * That is, an entities physical size and attributes.
     */
    private final Size size;

    /**
     * Coordinate associated with the entity.
     * That is, where the entity is located on the map grid.
     */
    private Coordinate coordinate;

    /**
     * Creates an entity with a given size and coordinate.
     *
     * @param size       size associated with the entity
     * @param coordinate coordinate associated with the entity
     * @ass1
     */
    public Entity(Size size, Coordinate coordinate) {
        this.size = size;
        this.coordinate = coordinate;
    }

    /**
     * Returns this entity's size.
     *
     * @return associated size.
     * @ass1
     */
    public Size getSize() {
        return size;
    }

    /**
     * Returns this entity's scenario grid coordinate.
     *
     * @return associated coordinate.
     * @ass1
     */
    public Coordinate getCoordinate() {
        return coordinate;
    }

    /**
     * Updates this entity's scenario grid coordinate.
     *
     * @param coordinate the new coordinate
     * @ass1
     */
    public void setCoordinate(Coordinate coordinate) {
        this.coordinate = coordinate;
    }

    /**
     * Returns the human-readable name of this entity.
     *
     * @return human-readable name
     * @ass1
     */
    public abstract String getName();


    /**
     * Returns the machine-readable string representation of this Entity.
     * The format of the string to return is
     *
     * EntityClass-size-coordinate
     * Where:
     * EntityClass is the entity's instance class name
     * size is the entity's associated size
     * coordinate is the encoding of the entity's associated coordinate
     * For example:
     * Fauna-SMALL-1,3
     * OR
     * Flora-GIANT-5,4
     *
     * @return encoded string representation of this Entity
     */
    public String encode() {
        return String.format("%s-%s-%s",
                this.getClass().getSimpleName(),
                this.size,
                this.coordinate.encode());
    }


    /**
     * Returns the hash code of this entity.
     * Two entities that are equal according to the equals(Object) method
     * should have the same hash code.
     *
     * @return - hash code of this entity.
     */
    @Override
    public int hashCode() {
        return (this.coordinate.hashCode() * 3) + (this.size.hashCode() * 17);
    }


    /**
     * Returns true if and only if this entity is equal to the other given
     * object.
     * For two entities to be equal, they must have the same size and
     * coordinate.
     *
     * @param other - the reference object with which to compare
     * @return true if this entity is the same as the other argument; false otherwise
     */
    @Override
    public boolean equals(Object other) {
        if (!(other instanceof Entity)) {
            return false;
        }
        Entity otherEntity = (Entity) other;
        return this.coordinate.equals(otherEntity.getCoordinate())
                && this.size.equals(otherEntity.getSize());

    }

    /**
     * Returns the human-readable string representation of this Entity.
     * <p>
     * The format of the string to return is:
     * <pre>name [EntityClass] at coordinate</pre>
     * Where:
     * <ul>
     *   <li>{@code EntityClass} is the entity's instance class simple name (see {@link #getClass()}
     *   )</li>
     *   <li>{@code name} is the entity's human-readable name according to the
     *   extended class implementation of {@link #getName()}</li>
     *   <li>{@code coordinate} is the entity's associated coordinate in human-readable form</li>
     * </ul>
     * For example:
     *
     * <pre>Fish [Fauna] at (1,3)</pre>
     * OR
     * <pre>Flower [Flora] at (6,4)</pre>
     *
     * @return human-readable string representation of this Entity
     * @ass1
     * @see Fauna#getName()
     */
    @Override
    public String toString() {
        return String.format("%s [%s] at %s",
            getName(),
            this.getClass().getSimpleName(),
            this.coordinate);
    }
}
