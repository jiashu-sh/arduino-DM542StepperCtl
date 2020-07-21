//信号灯输出口
const int LED_MSG_PIN = 13;

//定义了板上的4个控制端，12一组，34一组
const int DIRECTION_PIN = 8;

//定义了PWM引脚，需要将这两个脚接入PUL端
const int SPEED_PUL_PIN = 9;

//定义串口接受数据的全局变量
String comdata = "";

//直流电机的速度参数
int moto_val = 0;
//步进电机的转动方向
boolean stepper_direction = false;
//步进电机速度,wait ms（间隔时间越小,速度越快）
int stepperSpeed = 0;

//来回走动运行的次数
int i_run_times = 65;

boolean is_control_stepper = false;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600); //设置

  pinMode(LED_MSG_PIN, OUTPUT);
  pinMode(DIRECTION_PIN, OUTPUT);

  pinMode(SPEED_PUL_PIN, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  char val;
  while (Serial.available() > 0)
  {
    //comdata += char(Serial.read());
    val = char(Serial.read());
    if ((val != '\r') && (val != '\n'))
    {
      comdata += val;
    }
    delay(2);
  }

  if (comdata.length() > 0)
  {
    Serial.println(comdata + " rcv ok");

    Serial.println(comdata);
    if (comdata == "lighton")
    {
      digitalWrite(LED_MSG_PIN, HIGH);
      delay(300);
    }
    else if (comdata == "lightoff")
    {
      digitalWrite(LED_MSG_PIN, LOW);
      delay(300);
    }
    else if (comdata == "left")
    {
      stepper_direction = true; //步进电机正转
      digitalWrite(DIRECTION_PIN, HIGH);
      Serial.println("go forward.");
      delay(300);
    }
    else if (comdata == "right")
    {
      stepper_direction = false; //步进电机反转
      digitalWrite(DIRECTION_PIN, LOW);
      Serial.println("go backward.");
      delay(300);
    }
    else if (comdata == "stop")
    {
      digitalWrite(LED_MSG_PIN, LOW);
      //根据设置控制直流电机或者步进电机停止
      //控制步进电机
      analogWrite(SPEED_PUL_PIN, 0);

      delay(300);
    }
    else if (comdata == "pick")
    {
      //int i = 0;
      //开始向前
      digitalWrite(DIRECTION_PIN, HIGH);
      analogWrite(SPEED_PUL_PIN, 1);

      for (int i = 0; i < i_run_times; i++)
      {
        delay(100);
      }
      analogWrite(SPEED_PUL_PIN, 0);

      //开始向后
      digitalWrite(DIRECTION_PIN, LOW);
      analogWrite(SPEED_PUL_PIN, 1);

      for (int i = 0; i < i_run_times; i++)
      {
        delay(100);
      }
      analogWrite(SPEED_PUL_PIN, 0);
    }

    else
    {
      //开始读取输入的数字
      boolean is_number = true;
      //逐个字符读入检查是否数字
      for (int i = 0; i < comdata.length(); i++)
      {
        int inChar = comdata[i];
        if (!(isDigit(inChar)))
        {
          is_number = false;
        }
      }

      //检查是否为数字（速度值）
      if (is_number)
      {
        //设置步进/直流 电机的速度，直流电机速度范围1~1024 PWM值，步进电机越小速度越快
        moto_val = comdata.toInt();     //直流电机速度范围1~1024 PWM值,越大速度越快
        stepperSpeed = comdata.toInt(); //步进电机越小速度越快
        //开始控制电机
        is_control_stepper = true;

        analogWrite(SPEED_PUL_PIN, stepperSpeed);

        Serial.println("stepperSpeed:" + comdata + ".");
      }
    }
  }

  //是否控制步进电机
  if (is_control_stepper)
  {
    //控制步进电机
    analogWrite(SPEED_PUL_PIN, stepperSpeed);
  }

  comdata = "";
}
