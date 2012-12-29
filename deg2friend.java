import java.io.IOException;
import java.util.Random;
import java.util.Vector;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;



public class deg2friend {
	
	public static class job1Mapper extends Mapper<Object, Text, Text, Text>{
		
		private Text job1map_key = new Text();
		private Text job1map_value = new Text();
	    
	    public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
	      
	        String eachterm[] = value.toString().split(",");
	        
	        if(eachterm[0].compareTo(eachterm[1])<0){
	        	job1map_value.set(eachterm[0]+"\t"+eachterm[1]);
	        }
	        else if(eachterm[0].compareTo(eachterm[1])>0){
	        	job1map_value.set(eachterm[1]+"\t"+eachterm[0]);
	        }
	        
	        job1map_key.set(eachterm[0]);
	        context.write(job1map_key, job1map_value);
	        
	        job1map_key.set(eachterm[1]);
	        context.write(job1map_key, job1map_value);
	        
	    }
	} 
	
	public static class job1Reducer extends Reducer<Text,Text,Text,Text> {
		
		private Text job1reduce_key = new Text();
		private Text job1reduce_value = new Text();
		
		public void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
	
			String someperson = key.toString();
			Vector<String> hisfriends = new Vector<String>();
			
			for (Text val : values) {
				String eachterm[] = val.toString().split("\t");
				if(eachterm[0].equals(someperson)){
					hisfriends.add(eachterm[1]);
					
					job1reduce_value.set("deg1friend");
					context.write(val, job1reduce_value);
				}
				else if(eachterm[1].equals(someperson)){
					hisfriends.add(eachterm[0]);
					
					job1reduce_value.set("deg1friend");
					context.write(val, job1reduce_value);
				}
			}
			
			for(int i = 0; i<hisfriends.size(); i++){
				for(int j = 0; j<hisfriends.size(); j++){
					if (hisfriends.elementAt(i).compareTo(hisfriends.elementAt(j))<0){
						job1reduce_key.set(hisfriends.elementAt(i)+"\t"+hisfriends.elementAt(j));
						job1reduce_value.set("deg2friend");
						context.write(job1reduce_key, job1reduce_value);
					}
//					else if(hisfriends.elementAt(i).compareTo(hisfriends.elementAt(j))>0){
//						job1reduce_key.set(hisfriends.elementAt(j)+"\t"+hisfriends.elementAt(i));
//					}	
				}
		    }
		}	
	}
	
	public static class job2Mapper extends Mapper<Object, Text, Text, Text>{
		
		private Text job2map_key = new Text();
		private Text job2map_value = new Text();
	    
	    public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
	      
	        String lineterms[] = value.toString().split("\t");
	        
	        if(lineterms.length == 3){
	        	job2map_key.set(lineterms[0]+"\t"+lineterms[1]);
	        	job2map_value.set(lineterms[2]);
	    		context.write(job2map_key,job2map_value);
	    	}
	    }
	} 
	
	public static class job2Reducer extends Reducer<Text,Text,Text,Text> {
		
		private Text job2reducer_key = new Text();
		private Text job2reducer_value = new Text();
		
		public void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
			
			Vector<String> relationtags = new Vector<String>();
			
			String deg2friendpair = key.toString();
			
			for (Text val : values) {
				relationtags.add(val.toString());
			}
			
			boolean isadeg1friendpair = false;
			boolean isadeg2friendpair = false;
			int surport = 0;
			
			for(int i = 0; i<relationtags.size(); i++){
				if(relationtags.elementAt(i).equals("deg1friend")){
					isadeg1friendpair = true;
				}else if(relationtags.elementAt(i).equals("deg2friend")){
					isadeg2friendpair = true;
					surport += 1;
				}	
		    }
			
			if ((!isadeg1friendpair) && isadeg2friendpair){
				job2reducer_key.set(String.valueOf(surport));
				job2reducer_value.set(deg2friendpair);
	    		context.write(job2reducer_key,job2reducer_value);
			}
			
		}	
	}
	
	public static void main(String[] args) throws Exception {
		  
	    Configuration conf = new Configuration();
	    String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
	    if (otherArgs.length != 2) {
	      System.err.println("Usage: deg2friend <in> <out>");
	      System.exit(2);
	    }
	    
	    Job job1 = new Job(conf, "deg2friend");
	    job1.setJarByClass(deg2friend.class);
	    job1.setMapperClass(job1Mapper.class);
	    job1.setReducerClass(job1Reducer.class);
	    
	    job1.setOutputKeyClass(Text.class);
	    job1.setOutputValueClass(Text.class);
	    
	    //定义一个临时目录，先将任务的输出结果写到临时目录中, 下一个排序任务以临时目录为输入目录。
	    FileInputFormat.addInputPath(job1, new Path(otherArgs[0]));
		Path tempDir = new Path("deg2friend-temp-" + Integer.toString(new Random().nextInt(Integer.MAX_VALUE))); 
		FileOutputFormat.setOutputPath(job1, tempDir);
		
		if(job1.waitForCompletion(true))
		{
			Job job2 = new Job(conf, "deg2friend");
			job2.setJarByClass(deg2friend.class);
			
			FileInputFormat.addInputPath(job2, tempDir);
			
			job2.setMapperClass(job2Mapper.class);
		    job2.setReducerClass(job2Reducer.class);
			FileOutputFormat.setOutputPath(job2, new Path(otherArgs[1]));
			
			job2.setOutputKeyClass(Text.class);
			job2.setOutputValueClass(Text.class);
			
			FileSystem.get(conf).deleteOnExit(tempDir);
			
			System.exit(job2.waitForCompletion(true) ? 0 : 1);
		}
	    
	    
	    System.exit(job1.waitForCompletion(true) ? 0 : 1);
	    		    
  }

}
