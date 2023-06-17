#define N 200  // 数组长度
#define K 10   // 移动平均滤波窗口大小
#define THRESHOLD 100  // 突变值的阈值

int samples[N];        // 输入数组
int samples_mutation[N]; // 突变值处理后的数组
int moving_average_filtered[N];  // 移动平均滤波后的数组

void processing_mutation()
{
  int i;
  for(i = 0; i < N; i++)
  {
    samples_mutation[i] = samples[i];
  }
  // 处理突变值 消除数据突变，使数据更加平滑和稳定
  for (i = 1; i < N-1; i++) 
  {
    int diff1 = abs(samples_mutation[i] - samples_mutation[i-1]);
    int diff2 = abs(samples_mutation[i] - samples_mutation[i+1]);
    if (diff1 > THRESHOLD || diff2 > THRESHOLD) {
      samples_mutation[i] = (samples_mutation[i-1] + samples_mutation[i+1]) / 2;
    }
  }
}

void average_filtering()
{
  // 移动平均滤波
  for (int i = 0; i < N; i++) {
    int sum = 0;
    int count = 0;
    for (int j = max(0, i-K+1); j <= min(N-1, i+K-1); j++) {
      sum += samples_mutation[j];
      count++;
    }
    moving_average_filtered[i] = sum / count;
  }
}

void setup() {
  Serial.begin(9600);   // 串口初始化
  for (int i = 0; i < N; i++) {
    samples[i] = random(0, 1024);  // 初始化输入数组
  }
}

void loop() {
  processing_mutation();
  average_filtering();
  // 输出滤波后的数组到串口
  for (int i = 0; i < N; i++) 
  {
    Serial.print(samples[i]);
    Serial.print("\t");
    Serial.print(samples_mutation[i]);
    Serial.print("\t");
    Serial.println(moving_average_filtered[i]);
  }

  delay(100000);   // 延时 1 秒
}