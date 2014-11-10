const int accel_pin[] = { 0, 1, 2 };

int accel_val[] = { 0, 0, 0 };//mV出力を格納
double accel_mg[] = { 0, 0, 0 };//mVからmG（ミリガル）を換算し格納
double acc = 0;//合成加速度
double maxacc = 0;//合成加速度の最大値
double minacc = 0;//合成加速度の最小値
int sampling = 0;//サンプリング数をカウント
int mv = 660;//1gあたりの出力振れ幅（感度）

int steps = 0;
int threshold = 800;// 閾値（基準値）
int minthreshold = threshold - 5;// 閾値(下限)
int maxthreshold = threshold + 5;// 閾値(上限)
boolean stepflag = false;



void setup() {
  Serial.begin( 9600 );
}

void loop() {
  for ( int i = 0; i < 3; i++ ) {
    accel_val[i] = analogRead( accel_pin[i]);
    accel_mg[i] = (double)accel_val[i] / mv * 1000;//出力値の単位mVをmGに変換
  }
  acc = sqrt(pow(accel_mg[0], 2) + pow(accel_mg[1], 2) + pow(accel_mg[2], 2));//合成加速度を計算
  if(sampling == 0){
    maxacc = acc;
    minacc = acc;
  }
  else if(maxacc < acc){
    maxacc = acc;
  }
  else if(minacc > acc){
    minacc = acc;
  }
  Serial.print( "X:" );
  Serial.print( accel_mg[0] );
  Serial.print( " Y:" );
  Serial.print( accel_mg[1] );
  Serial.print( " Z:" );
  Serial.print( accel_mg[2] );
  Serial.print( " Step:" );
  Serial.print( steps );
  Serial.print( " Threshold:" );
  Serial.print( threshold );
  Serial.print( "\n" );

  
  sampling++;
  if (sampling >= 50){//50パターン（5秒）ごとに、閾値を合成加速度の最大値と最小値の平均に更新する
    threshold = (maxacc + minacc)/2;
    minthreshold = threshold - 5;
    maxthreshold = threshold + 5;
    sampling = 0;
  }
  
  counter(acc);//合成加速度と閾値を照らし合わせる

  delay( 100 );
}

void counter( double acc){
    // もし、歩数カウント用のフラッグmStepFlagがTRUEの場合、閾値(上限)を超えている状態
    // そのため、閾値(下限)を超えたら１歩とカウントして、歩数カウント用フラグをFALSEに戻す。
    // mStepFlagがFALSEの場合は、閾値(上限)をこえたら
    // 歩数カウント用のフラッグmStepFlagをTRUEにしておく。
    if ( stepflag == true ){
        if ( acc < minthreshold ){
            steps++;
            stepflag = false;
        }
    } else {
        if ( acc > maxthreshold ){
            stepflag = true;
        }
    }
}
