# arduino-DM542StepperCtl
# 通过arduino程序控制DM542驱动57步进电机

用arduino的单片机接DM542驱动器来控制57步进电机。采用共阴极接法，即ENA不接，PUL-与DIR-串接单片机GND,PUL+接单片机p3^1口给脉冲，DIR+接P3^2口给高低电平（本来应该高电平正转，低电平反转）,驱动器输出端A+接红，A-接绿，B+接黄，B-接蓝，供电直流24v。

定义引脚8作为方向控制，定义了9号引脚作为PWM引脚，需要将这引脚接入PUL端。
程序定义方向，stepper_direction = true，步进电机正转；stepper_direction = false反转。

通过串口输入控制指令，left/right控制前进/后退方向；直接输入数字运行，输入stop/0表示停止运行。
输入pick进行一次往复来回运行。
