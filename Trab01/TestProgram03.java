import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class TestProgram03 {
    public static void main(String[] args) {

        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));

        try {
            double y = Double.parseDouble(reader.readLine());
            double x = Double.parseDouble(reader.readLine());
	    } catch (IOException e) {
            e.printStackTrace();  
        }
    }
    }
