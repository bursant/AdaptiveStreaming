package pedomock;

import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

public class Feeder {

	public static void main(String[] args) throws IOException, ParseException {
		List<String> input = Files.readAllLines(Paths.get("log4.txt"), Charset.defaultCharset());
		EventDetector detector = new EventDetector();
		SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss.SSS");
		
		for(String line : input){
			String feed = line.split(" ")[3];
			SensorEvent event = new SensorEvent();
			for(int i = 0; i < 3; i++){
				String value = feed.split("A")[i];
				event.values[i] = Float.parseFloat(value);
			}
			String time = line.split(" ")[1];
			time = time.substring(0, time.length()-1);
	        Date date = sdf.parse(time); 
			
			detector.onSensorChanged(event, date.getTime());
		}
	}

}