//Raiz digital
import java.util.Scanner;

public class Main {
public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        while (true) {
            int n = scanner.nextInt();
            if (n == 0) break;
            while (n >= 10) {
                int suma = 0;
                while (n > 0) {
                    suma += n % 10; 
                    n /= 10; 
                }
                n = suma;
            }
            System.out.println(n);
        }
    }
}