package pedomock;

public class ExponentialMovingAverage {
    private double alpha;
    private Double oldValue;
    
    public ExponentialMovingAverage() {
        this.alpha = 0.1;
    }

    public double average(double value) {
        if (oldValue == null) {
            oldValue = value;
            return value;
        }
        double newValue = oldValue + alpha * (value - oldValue);
        oldValue = newValue;
        return newValue;
    }
}
