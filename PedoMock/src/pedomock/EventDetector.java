package pedomock;

public class EventDetector implements SensorEventListener {
	
	private ExponentialMovingAverage avg;
	private long LastStepDetection;
	private long StepDetectionDelta;
	private double DifferenceDelta;

	/*
	 * alpha - factor for moving average
	 * cadence - time slot between steps
	 */
	public EventDetector(){
		avg = new ExponentialMovingAverage();
		LastStepDetection = 0;
		StepDetectionDelta = 500;
		DifferenceDelta = 0.8;
	}
	
	@Override
	public void onSensorChanged(SensorEvent event, long time) {
		detectStep(event, time);
		detectFall(event, time);
	}
	
	private void detectStep(SensorEvent event, long time){
		double vector = Math.sqrt(event.values[0]*event.values[0] + 
				event.values[1]*event.values[1] + 
				event.values[2]*event.values[2]);
		
		double average = avg.average(vector);
		
		if(vector - average > DifferenceDelta && time-LastStepDetection > StepDetectionDelta){
			LastStepDetection = time;
			System.out.println(time-70000000 + "\t" + vector + "\t" + average + "\t10");
		}
		else
			System.out.println(time-70000000 + "\t" + vector + "\t" + average + "\t12");
	}
	
	private void detectFall(SensorEvent event, long time){
		
	}
}
