import java.io.IOException;
import java.util.Random;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;


public class csdn{

	public static class CountMapper extends Mapper<Object, Text, Text, IntWritable>{
    
	    private final static IntWritable one = new IntWritable(1);
	    private Text password = new Text();
	      
	    public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
	      
	        String eachline=value.toString();
	        String [] eachterm=eachline.split("#");
	        
	        password.set(eachterm[1]);
	        
	    	context.write(password, one);  
	    }
  	} 
  
	public static class CountReducer extends Reducer<Text,IntWritable,Text,IntWritable> {
		
		private IntWritable total = new IntWritable();

		public void reduce(Text key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException {
	  
	  	int sum = 0;
	  	for (IntWritable val : values) {
	    	sum += val.get();
	  	}
	  	total.set(sum);
	  	context.write(key,total);
		}
	}
  

	public static class SortMapper extends Mapper<Object, Text, IntWritable,Text>{
	    
	    public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
	      
	    	IntWritable times = new IntWritable(1);
	 	    Text password = new Text();
	 	   
	 	    
	    	String eachline=value.toString();
	    	String[] eachterm =eachline.split("	");
	    	
	    	if(eachterm.length==2){
	    		password.set(eachterm[0]);
	    		times.set(Integer.parseInt(eachterm[1]));
	    		context.write(times,password);
	    	}else{
	    		password.set("errorpassword");
	    		context.write(times,password);
	    	}
	    }
	} 
	  
	public static class SortReducer extends Reducer<IntWritable,Text,IntWritable,Text> {
		private Text password = new Text();

		public void reduce(IntWritable key,Iterable<Text> values, Context context) throws IOException, InterruptedException {

			//不同的密码可能出现相同的次数
			for (Text val : values) {
				password.set(val);
				context.write(key,password);
			}
		}
	}
  
  
	private static class IntDecreasingComparator extends IntWritable.Comparator {
		public int compare(WritableComparable a, WritableComparable b) {
			return -super.compare(a, b);
		}

		public int compare(byte[] b1, int s1, int l1, byte[] b2, int s2, int l2) {
		 	return -super.compare(b1, s1, l1, b2, s2, l2);
		}
	}
  
  

	public static void main(String[] args) throws Exception {
	  
	    Configuration conf = new Configuration();
	    String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
	    if (otherArgs.length != 2) {
	      System.err.println("Usage: csdn <in> <out>");
	      System.exit(2);
	    }
	    
	    Job job = new Job(conf, "csdn");
	    job.setJarByClass(csdn.class);
	    job.setMapperClass(CountMapper.class);
	    job.setCombinerClass(CountReducer.class);
	    job.setReducerClass(CountReducer.class);
	    
	    job.setOutputKeyClass(Text.class);
	    job.setOutputValueClass(IntWritable.class);
	    
	    //定义一个临时目录，先将词频统计任务的输出结果写到临时目录中, 下一个排序任务以临时目录为输入目录。
		FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
	    Path tempDir = new Path("csdn-temp-" + Integer.toString(new Random().nextInt(Integer.MAX_VALUE))); 
	    FileOutputFormat.setOutputPath(job, tempDir);
		
	    
	    if(job.waitForCompletion(true))
		{
			Job sortJob = new Job(conf, "csdnsort");
			sortJob.setJarByClass(csdn.class);

			FileInputFormat.addInputPath(sortJob, tempDir);

			sortJob.setMapperClass(SortMapper.class);
			FileOutputFormat.setOutputPath(sortJob, new Path(otherArgs[1]));

			sortJob.setOutputKeyClass(IntWritable.class);
			sortJob.setOutputValueClass(Text.class);

			sortJob.setSortComparatorClass(IntDecreasingComparator.class);

			FileSystem.get(conf).deleteOnExit(tempDir);

			System.exit(sortJob.waitForCompletion(true) ? 0 : 1);
		}
	    
	    System.exit(job.waitForCompletion(true) ? 0 : 1);  
	}
}
