# ✨Custom ctrl Server

| Supported Targets | ESP32-C3 | ESP32-S3 |
| ----------------- | -------- | -------- |
本分支使用ESP-IDF 5.3.1 进行编译烧录

### 💻How to Use 
1. 单击打开
2. 双击关闭

### 🎯Hardware Required
七轴机械臂和一个同构的示教器

### 📌Pin Assignment
编码器实体请在main.cpp中修改,可以选择硬件/软件i2c的as5600/mt6701,构造实体的时候填入真实的引脚即可。由于esp32s3只有两路硬件i2c,只够驱动4个编码器,所以只能选择软件i2c,如果要驱动多个编码器,请使用软件，本代码与硬件高度耦合，仅供参考。

### ❗NOTE
先让server上电,再让client上电,等待client连接即可传输



### Referee and Thanks
[MiaoUI](https://github.com/JFeng-Z/MiaoUI)




