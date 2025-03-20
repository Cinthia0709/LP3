import javax.swing.JOptionPane;

public class TestProgram05 {
    public static void main(String[] args) {
        


        
        String numxtext = JOptionPane.showInputDialog("Ingrese el numero x:");
        double x;
        try {
            x = Double.parseDouble(numxtext); 
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(null, "Error: El numero debe ser real");
            return;
       

}

        String numytext = JOptionPane.showInputDialog("Ingresa el numero y:");
        double y;
        try {
            y = Double.parseDouble(numytext);
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(null, "Error: El numero debe ser real");
            return;
	}

    }
}

