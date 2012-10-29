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


public class TopQuery{

	public static class CountMapper extends Mapper<Object, Text, Text, IntWritable>{

		private final static IntWritable one = new IntWritable(1);
		private Text query = new Text();
		  
		public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
		  
			String eachline=value.toString();
			String [] eachterm=eachline.split("#");
			//每行以#分隔的第二个字段为具体的query字面值
			query.set(eachterm[1]);
			context.write(query, one);
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
	    
	    IntWritable times = new IntWritable();
		Text query = new Text();
			
		public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
			String eachline=value.toString();
			String[] eachterm =eachline.split("	");

			query.set(eachterm[0]);
			times.set(Integer.parseInt(eachterm[1]));
			context.write(times,query);	
		}
	}

	  
	public static class SortReducer extends Reducer<IntWritable,Text,IntWritable,Text> {
		
		private Text query = new Text();

		public void reduce(IntWritable key,Iterable<Text> values, Context context) throws IOException, InterruptedException {
			//不同的query可能出现相同的次数
			for (Text val : values) {
				query.set(val);
				context.write(key,query);
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
			System.err.println("Usage: query <in> <out>");
			System.exit(2);
		}
	    
		Job job = new Job(conf, "query");
		job.setJarByClass(TopQuery.class);
		job.setMapperClass(CountMapper.class);
		job.setCombinerClass(CountReducer.class);
		job.setReducerClass(CountReducer.class);

		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(IntWritable.class);

		//定义一个临时目录，先将query词频统计任务的输出结果写到临时目录中, 下一个排序任务以临时目录为输入目录。
		FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
		Path tempDir = new Path("query-temp-" + Integer.toString(new Random().nextInt(Integer.MAX_VALUE))); 
		FileOutputFormat.setOutputPath(job, tempDir);

	    
		if(job.waitForCompletion(true))
		{
			Job sortJob = new Job(conf, "querysort");
			sortJob.setJarByClass(TopQuery.class);

			FileInputFormat.addInputPath(sortJob, tempDir);

			sortJob.setMapperClass(SortMapper.class);
			FileOutputFormat.setOutputPath(sortJob, new Path(otherArgs[1]));

			sortJob.setOutputKeyClass(IntWritable.class);
			sortJob.setOutputValueClass(Text.class);

			//Map、Reduce端的排序都以query频数降序为准
			sortJob.setSortComparatorClass(IntDecreasingComparator.class);

			FileSystem.get(conf).deleteOnExit(tempDir);

			System.exit(sortJob.waitForCompletion(true) ? 0 : 1);
		}
	    System.exit(job.waitForCompletion(true) ? 0 : 1);
	}

}
