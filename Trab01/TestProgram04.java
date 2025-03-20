import java.util.Scanner;
public class TestProgram04{

    public static void main(String[] args) {
        
        if (args.length < 2) {
            System.out.println("ingrese los numeros");
            System.out.println("Ejemplo de uso: java ArgsExample 2 4");
            return;
        }
                double x = Double.parseDouble(args[0]);
		double y = Double.parseDouble(args[1]);	
		
    }
}
