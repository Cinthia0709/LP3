public class Program{

	double x;
	double y;
	double result;

	public Program(double a, double b){
		this.x = a;
		this.y = b;

	}

	public void  setX(double a) {
		this.x = a;
	}
	
	public void  setY(double b) {
		this.y = b;
	}

	public double  getResult(){
		return this.result;
	}

	public double  getAdd(){
		return this.x+this.y;
	}
}
