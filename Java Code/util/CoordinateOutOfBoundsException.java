package researchsim.util;

/**
 * Exception thrown when the program attempts to access a Tile using an invalid Coordinate / index.
 *
 * @ass2
 */
public class CoordinateOutOfBoundsException extends Exception {

    /**
     * Constructs a CoordinateOutOfBoundsException with no detail message.
     */
    public CoordinateOutOfBoundsException() {
        super();
    }

    /**
     * Constructs a CoordinateOutOfBoundsException that contains a helpful
     * detail message explaining why the exception occurred.
     * Important: do not write JUnit tests that expect a valid implementation
     * of the assignment to have a certain error message, as the official
     * solution will use different messages to those you are expecting, if
     * any at all.
     *
     * @param message - detail message
     */
    public CoordinateOutOfBoundsException(String message) {
        super(message);
    }

    /**
     *Constructs a CoordinateOutOfBoundsException that contains a helpful
     * detail message explaining why the exception occurred and the
     * underlying cause of the exception.
     * Important: do not write JUnit tests that expect a valid implementation
     * of the assignment to have a certain error message, as the official
     * solution will use different messages to those you are expecting, if
     * any at all.
     *
     * @param message - detail message
     * @param cause - throwable that caused this exception
     */
    public CoordinateOutOfBoundsException(String message, Throwable cause) {
        super(message, cause);
    }

    /**
     *Constructs a CoordinateOutOfBoundsException that stores the underlying
     * cause of the exception.
     * Parameters:
     *
     * @param cause - throwable that caused this exception
     */
    public CoordinateOutOfBoundsException(Throwable cause) {
        super(cause);
    }

}
