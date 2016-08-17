HADOOP=/software/servers/hadoop-2.7.1/bin/hadoop

INPUT_LEFT="/user/zhaoyihua/left_matrix" 
INPUT_RIGHT="/user/zhaoyihua/right_matrix"
OUTPUT_TEMP="/user/zhaoyihua/output_temp"
OUTPUT="/user/zhaoyihua/output"


$HADOOP fs -rmr ${OUTPUT1}
$HADOOP jar /software/servers/hadoop-2.7.1/share/hadoop/tools/lib/hadoop-streaming-2.7.1.jar \
-D mapreduce.job.reduce.slowstart.completedmaps=1 \
-D mapred.reduce.tasks=100 \
-D stream.memory.limit=1000 \
-D mapreduce.map.memory.mb=1000 \
-input ${INPUT_LEFT},${INPUT_RIGHT} \
-output ${OUTPUT_TEMP} \
-mapper "python ./sparseMatrixMultiply.py mapper1" \
-reducer "python ./sparseMatrixMultiply.py reducer1" \
-file ./sparseMatrixMultiply.py \

$HADOOP fs -rmr ${OUTPUT2}
$HADOOP jar /software/servers/hadoop-2.7.1/share/hadoop/tools/lib/hadoop-streaming-2.7.1.jar \
-D mapreduce.job.reduce.slowstart.completedmaps=1 \
-D mapred.reduce.tasks=100 \
-D stream.memory.limit=1000 \
-D mapreduce.map.memory.mb=1000 \
-input ${OUTPUT_TEMP} \
-output ${OUTPUT} \
-mapper "python ./sparseMatrixMultiply.py mapper2" \
-reducer "python ./sparseMatrixMultiply.py reducer2" \
-file ./sparseMatrixMultiply.py \

