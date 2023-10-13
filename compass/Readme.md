# BPI-BIT 示例代码-板载九轴传感器MPU9250（指南针）

## 实验现象：

通过串口返回板载MPU9250磁力计获得的地磁数据转化来的南北角度。
角度与方向(LED面板向上)：

|方向|角度|
|:--:|:--:|
|东  |+90 |
|南  |±180|
|西  |-90 |
|北  |±0  |


## 相关库安装与下载地址

- 首先需要安装库：[MPU9250库](https://github.com/BPI-STEAM/MPU9250)

- 库安装方法：[安装方法(wiki)](https://github.com/BPI-STEAM/BPI-BIT-Arduino-IDE/wiki/Install_Lib#arduino-ide%E5%AE%89%E8%A3%85%E5%BA%93)

## 硬件版本(Hardware Version)区别：

主要区分为`v1.2`和`v1.4`版本

BPI-BIT板硬件版本号标注与板子背面右下角，金手指上边。如下图中位置所示：

![Hardware version](/extras/hardware_version.png)