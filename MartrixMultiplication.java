import java.io.IOException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;


public class MartrixMultiplication{

  public static class MartrixMapper extends Mapper<Object, Text, Text, Text>{
    
    private Text map_key = new Text();
    private Text map_value = new Text();
    
    int rNumber = 300;
    int cNumber = 500;
    String fileTarget;
    String i, j, k, ij, jk;
    
      
    public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
      
        String eachterm[] = value.toString().split("#");
        fileTarget = eachterm[0];
        
        if(fileTarget.equals("M")){
          i = eachterm[1];
          j = eachterm[2];
          ij = eachterm[3];
            
          for(int c = 1; c<=cNumber; c++){
              map_key.set(i + "#" + String.valueOf(c));
              map_value.set("M" + "#" + j + "#" + ij);
              context.write(map_key, map_value);
          }
            
        }else if(fileTarget.equals("N")){
          j = eachterm[1];
          k = eachterm[2];
          jk = eachterm[3];

          for(int r = 1; r<=rNumber; r++){
              map_key.set(String.valueOf(r) + "#" +k);
              map_value.set("N" + "#" + j + "#" + jk);
              context.write(map_key, map_value);
          }
        }
    }

  } 
  
  
  public static class MartrixReducer extends Reducer<Text,Text,Text,Text> {
    
    private Text reduce_value = new Text();
    
    int jNumber = 150;

    int M_ij[] = new int[jNumber+1];
    int N_jk[] = new int[jNumber+1];

    int j, ij, jk;

    String fileTarget;
    int jsum = 0;
    
    public void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
      
      jsum = 0; 

      for (Text val : values) {
        String eachterm[] = val.toString().split("#");
        
        fileTarget = eachterm[0];
        j = Integer.parseInt(eachterm[1]);
        
        if(fileTarget.equals("M")){
      	  ij = Integer.parseInt(eachterm[2]);
      	  M_ij[j] = ij;
        }else if(fileTarget.equals("N")){
      	  jk = Integer.parseInt(eachterm[2]);
      	  N_jk[j] = jk;
        }
        
      }
      
      for(int d = 1; d<=jNumber; d++){
    	 jsum +=  M_ij[d] * N_jk[d];
      }
      
      reduce_value.set(String.valueOf(jsum));
      context.write(key, reduce_value); 
    }

  }
  

  public static void main(String[] args) throws Exception {

      Configuration conf = new Configuration();
      String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
      if (otherArgs.length != 2) {
	  System.err.println("Usage: MartrixMultiplication <in> <out>");
	  System.exit(2);
      }
	    
      Job job = new Job(conf, "martrixmultiplication");
      job.setJarByClass(MartrixMultiplication.class);
      job.setMapperClass(MartrixMapper.class);
      job.setReducerClass(MartrixReducer.class);
	    
      job.setOutputKeyClass(Text.class);
      job.setOutputValueClass(Text.class);
	    
      FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
      FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));
	    
      System.exit(job.waitForCompletion(true) ? 0 : 1);
	    		    
  }
  
}