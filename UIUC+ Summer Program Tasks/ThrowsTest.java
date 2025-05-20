package jabha;

public class ThrowsTest {

    // Method declares that it throws IOException, but doesn't actually throw
    // anything
    public static void methodWithThrows() throws RuntimeException {
        System.out.println("This method declares a throws clause but doesn't throw an exception.");
    }

    public static void main(String[] args) {
        methodWithThrows();
    }
}
