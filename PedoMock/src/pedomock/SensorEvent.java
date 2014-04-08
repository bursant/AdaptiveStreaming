package pedomock;

public class SensorEvent {
	public float[] values;
	
	public SensorEvent(){
		values = new float[3];
	}
	
	public String toString(){
		return values[0] + " " + values[0] + " " + values[0];
	}
}
