
![QQ20250522-214441.png](https://image.lceda.cn/oshwhub/pullImage/12c2f05854434ef9a72cda32e4d1c8d8.png)

## 🌈简介

<div style="background-color:#e6f7ff;padding:20px;border-radius:8px">

**🌟是一个FOC开发板也是一个EtherCAT从站开发板，体积小巧成本超低**
PocketServo 是一个基于 STM32G474RET6 微控制器的高性能FOC伺服驱动项目。该项目采用模块化设计，实现了完整的电机控制系统，包括电机参数配置、自动校准、多种控制模式、实时监控及调试功能。
    
[📺 B站视频](https://www.bilibili.com/video/BV1PZJpzrERm/) | [💻 源码仓库](https://oshwhub.com/flose/pocketservo) | [🐧 QQ交流群](https://qm.qq.com/q/nPpUbjPaXm) (群号：195646621)    
    
</div>

<img src="https://image.lceda.cn/oshwhub/pullImage/768016556b2e497498cdf81a306bdae9.png" width="70%">

<img src="https://image.lceda.cn/oshwhub/pullImage/559848290bf94fb4a4774954b60e1936.png" width="70%">

<div style="background-color:#e6f7ff;padding:20px;border-radius:8px">
    <ul>
        <li>✅丰富的总线接口：支持EtherCAT和CAN总线通信，能够实现高速、稳定的数据传输，适用于工业自动化和汽车电子等复杂环境。</li>
        <li>✅显示直观：配备1.14寸SPI屏幕，显示清晰，操作界面友好。用户可以通过按键轻松修改参数和切换模式，实现直观的人机交互。</li>
        <li>✅交互高效：支持通过串口终端修改参数和切换模式，响应迅速，能够及时处理用户的操作指令，提高工作效率。</li>
        <li>✅监测全面：支持温度采集功能，支持通过USB上报数据，方便与上位机或其他设备进行数据交互，便于实时监控和数据记录。</li>
        <li>✅已适配有感FOC（Field-Oriented Control）算法，能够实现高精度的电机控制，提高电机的效率和性能，适用于各种复杂的电机驱动场景。</li>
    </ul>
</div>

![QQ20250522-213425.png](https://image.lceda.cn/oshwhub/pullImage/cb5cea0ef0204b09b225ccff4c4090b7.png)

## 🧩硬件介绍

![QQ20250522-213425.png](https://image.lceda.cn/oshwhub/pullImage/570d2b9e7fb34e70a287f087555df132.png)

<div style="background-color:#ADD8E6;padding:10px;border-radius:5px">

- **主控芯片**：STM32G474RE（LQFP64封装）
- **主频**：170MHz
- **PWM更新频率**：20kHz
- **编码器**：MT6816磁编码器，SPI接口
- **显示**：ST7789 LCD控制器，SPI接口
- **EtherCAT**：周立功Dport-ECT，SPI接口
- **CAN收发器**：TJA1051TK
- **电流采样**：三相电流采样，ADC注入转换
</div>

### 1.采样电路
<div style="background-color:#e6f7ff;padding:10px;border-radius:5px">
该电路系统采用RS724XQ四运放芯片。一路运放构建电压跟随器利用输出阻抗低带负载能力强的特性来输出稳定的偏置电压，其余三路用于构建差分运算放大器，对采样到的电流信号进行放大，通过恰当的电路设计，可保证放大之后的信号在需要的范围内。
</div>

![图片1.png](https://image.lceda.cn/oshwhub/pullImage/06ed86bb92b14fc28e16ed4006f0434d.png)

<div style="background-color:#e6f7ff;padding:10px;border-radius:5px">
如果你不了解差分运算放大器可直接看下面结论公式(虚短虚断就能快速求出)
</div>


![image.png](https://image.lceda.cn/oshwhub/pullImage/9d0c9ec826cb48ba96c08341fd83802f.png)

### 2.驱动电路
<div style="background-color:#e6f7ff;padding:10px;border-radius:5px">
六个WSD4070DN构成三相半桥逆变电路，可承受40V的漏源电压，并具有68A的连续漏极电流（在25°C环境下），脉冲漏极电流甚至可达144A。
<br />下桥臂使用采样电阻进行电流采样。
<br />门级串联小电阻，减小振荡。
</div>

![image.png](https://image.lceda.cn/oshwhub/pullImage/9a9f9e2f662640eb9b430b20d6dc703e.png)

### 3.电源电路
<div style="background-color:#FFC0CB;padding:10px;border-radius:5px">
LGS5148最大输入 48V，所以母线电压不要超过48V！！！<br />
有两路LDO，专门一路用于ADC参考电压

</div>

![image.png](https://image.lceda.cn/oshwhub/pullImage/98f75185614c4fdbaa87674e1357038b.png)

## 💻软件介绍

![QQ20250522-213425.png](https://image.lceda.cn/oshwhub/pullImage/e0c6b1cb4d94457ca562caa0395a5202.png)

## 主要特性
<div style="background-color:#ADD8E6;padding:10px;border-radius:5px">
    
- **FOC控制算法**：实现了完整的磁场定向控制算法，具有高性能的转矩控制能力
- **多种控制模式**：
  - 转矩控制模式（CONTROL_MODE_TORQUE_RAMP）
  - 速度控制模式（CONTROL_MODE_VELOCITY_RAMP）
  - 位置控制模式（CONTROL_MODE_POSITION_RAMP）
  - 位置-速度-电流级联控制模式（CONTROL_MODE_POSITION_PROFILE）
- **参数配置系统**：支持电机参数、控制器参数、保护参数等配置与存储
- **自动校准**：提供电机参数识别和编码器零点校准功能
- **实时监控**：支持通过LCD显示和串口输出实时运行参数
- **命令行接口**：基于Letter-Shell实现强大的命令行交互功能
- **丰富的外设支持**：LCD显示、USB通信、EtherCAT通信等
- **FreeRTOS支持**：采用实时操作系统，提高系统稳定性和响应速度
</div>

## 文件结构

```
PocketServo_V0.1/
├── Core/                      # STM32核心文件
├── Drivers/                   # STM32 HAL驱动和CMSIS
├── MDK-ARM/                   # Keil工程文件
├── Middlewares/               # 中间件（FreeRTOS等）
├── my_foc/                    # FOC算法核心
│   ├── inc/                   # 头文件
│   │   ├── calibration.h      # 校准模块
│   │   ├── encoder.h          # 编码器模块
│   │   ├── foc.h              # FOC算法
│   │   ├── mc_fsm.h           # 电机控制状态机
│   │   ├── pid.h              # PID控制器
│   │   ├── pwm_curr.h         # PWM和电流采样
│   │   ├── usr_config.h       # 用户配置
│   │   └── utils.h            # 工具函数
│   ├── src/                   # 源文件
│   ├── my_foc.c               # FOC项目管理
│   └── my_foc.h               # FOC项目头文件
├── bsp/                       # 板级支持包
│   ├── inc/                   # 头文件
│   ├── src/                   # 源文件
│   ├── bsp.c                  # BSP初始化
│   └── bsp.h                  # BSP头文件
├── LCD/                       # LCD显示模块
├── USB_Device/                # USB设备
├── letter-shell-3.1.2/        # 命令行接口
├── EcatPort/                  # EtherCAT通信移植
├── SSC/                       # EtherCAT从站协议栈
├── Template.ioc               # STM32CubeMX配置文件
└── README.md                  # 说明文件
```

## 核心模块

<div style="background-color:#e6f7ff;padding:20px;border-radius:8px">

- **FOC算法模块**（`my_foc/src/foc.c`）
  - 实现电流环、速度环、位置环控制算法
  - Clark/Park/逆Park变换
  - SVPWM（空间矢量脉宽调制）

- **编码器模块**（`my_foc/src/encoder.c`）
  - 角度读取与处理
  - 速度估计
  - 多圈位置计算

- **参数配置模块**（`my_foc/src/usr_config.c`）
  - 用户配置参数管理
  - 默认参数设置

- **校准模块**（`my_foc/src/calibration.c`）
  - 电机参数识别
  - 编码器零点校准
  - 编码器方向检测

- **PID控制器**（`my_foc/src/pid.c`）
  - 通用PID控制器实现
  - 积分限幅和输出限幅

- **状态机**（`my_foc/src/mc_fsm.c`）
  - 电机控制状态管理
  - 错误处理和保护
</div>

## 开发环境
<div style="background-color:#ADD8E6;padding:10px;border-radius:5px">
    
- **IDE**：Keil MDK-ARM
- **STM32CubeMX**：用于初始化配置和代码生成
- **编译器**：ARM Compiler 5
- **调试器**：DAP-Link
</div>

## 快速开始
<div style="background-color:#ADD8E6;padding:10px;border-radius:5px">
   
- **1.环境准备**
   - 安装Keil MDK-ARM
   - 安装STM32CubeMX（用于修改配置）
- **2.修改母线电压**

![image.png](https://image.lceda.cn/oshwhub/pullImage/0f8d168ef8424ff6a9e01a7c3500d1a5.png)
- **3.编译与下载**
   - 打开`MDK-ARM/Template.uvprojx`项目
   - 编译项目
   - 通过DAP-Link下载到目标板

- **4.控制电机**
   - 设置目标控制模式（转矩/速度/位置）
   - 设置控制参数
</div>

## 有感FOC

![image.png](https://image.lceda.cn/oshwhub/pullImage/11c9ce2e5ea8475bb697e41ea6599e4d.png)

![image.png](https://image.lceda.cn/oshwhub/pullImage/a8261e3f62a0477cb0096f37fe69e3f4.png)


## 命令行终端


![图片2.png](https://image.lceda.cn/oshwhub/pullImage/443516a0e87245e9b901452dce4cd370.png)

# 命令行终端支持以下命令，按功能分类：

<div style="background-color:#ADD8E6;padding:10px;border-radius:5px">

- **1.电机控制命令**
   - `set_ctrl_mode [mode]` - 设置电机控制模式，支持模式：
     - `TORQUE_RAMP` - 转矩控制模式
     - `VELOCITY_RAMP` - 速度控制模式
     - `POSITION_RAMP` - 位置控制模式
     - `POSITION_PROFILE` - 位置-速度-电流级联控制模式
   - `set_cur [value]` - 设置电流限制或目标电流
   - `set_vel [value]` - 设置目标速度或速度限制
   - `set_pos [value]` - 设置目标位置
- **2.信息查询命令**
   - `motinfo` - 显示电机配置信息
   - `focinfo` - 显示FOC控制器信息和状态
   - `help` - 显示命令帮助信息
   - `sysinfo` - 显示系统任务信息和运行统计
- **3.变量设置**
   - `setVar` - 设置变量值
</div>

## EtherCAT

<div style="background-color:#ADD8E6;padding:10px;border-radius:5px">
右侧两个按钮同时按下进入EtherCAT通信模式<br />
由于EtherCAT模块和LCD屏幕通用一个SPI接口，为了保证速度同时只运行一个设备进行运行
</div>    
    
![image.png](https://image.lceda.cn/oshwhub/pullImage/8c652128c2a245a38ad056a31b1aba80.png)

<div style="background-color:#ADD8E6;padding:10px;border-radius:5px">
切换到EtherCAT模式后屏幕会显示[EtherCAT]
</div>

![image.png](https://image.lceda.cn/oshwhub/pullImage/6ec9b8cb82a44857929f2026e81513de.png)

<div style="background-color:#ADD8E6;padding:10px;border-radius:5px">
通过TwinCAT3扫描到从站后就可以进行通信
</div>

![image.png](https://image.lceda.cn/oshwhub/pullImage/cde78a71b6584d58927cf062b818a22f.png)


![image.png](https://image.lceda.cn/oshwhub/pullImage/08b08972b2ec45228659009b2757bf2b.png)

## 📱功能演示

1. [【星火计划】PocketServo硬件介绍](https://www.bilibili.com/video/BV1p2Jxz1Eyn/?share_source=copy_web&vd_source=4ee7262585fd8c76ac279a555c7b3054)
2. [【星火计划】PocketServo电机校准功能介绍](https://www.bilibili.com/video/BV1p2Jxz1E7a/?share_source=copy_web&vd_source=4ee7262585fd8c76ac279a555c7b3054)
3. [【星火计划】PocketServo按键交互功能介绍](https://www.bilibili.com/video/BV14zJxznEp6/?share_source=copy_web&vd_source=4ee7262585fd8c76ac279a555c7b3054)
4. [【星火计划】PocketServo上位机USB数据上报介绍](https://www.bilibili.com/video/BV1xzJxznEXr/?share_source=copy_web&vd_source=4ee7262585fd8c76ac279a555c7b3054)
5. [【星火计划】PocketServo命令行交互功能介绍](https://www.bilibili.com/video/BV1skJxzEE3D/?share_source=copy_web&vd_source=4ee7262585fd8c76ac279a555c7b3054)
6. [【星火计划】PocketServo EtherCAT总线功能介绍](https://www.bilibili.com/video/BV11kJxzEEJc/?share_source=copy_web&vd_source=4ee7262585fd8c76ac279a555c7b3054)

## 💎 致谢名单
硬件设计参考：[基于STM32G4的FOC电机驱动学习板](https://oshwhub.com/lylssy/foc_driver) 感谢苏工开源！
感谢[有幸遇到娇宝宝](https://space.bilibili.com/687076795) 大佬的FOC视频教程
感谢 电磁诡力★FOC交流群 群友们的耐心解答
感谢 [立创开源硬件平台-星火计划](https://oshwhub.com/activities/spark2025?tab=2) 提供的元件和PCB打印券