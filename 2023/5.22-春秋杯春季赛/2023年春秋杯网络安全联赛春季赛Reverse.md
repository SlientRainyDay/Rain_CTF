# Reverse

## sum

函数逻辑如下

![image-20230607232640441](https://image-1311319331.cos.ap-beijing.myqcloud.com/image/202306072326564.png)

跟进verify语句查看是什么

![image-20230608001718414](https://image-1311319331.cos.ap-beijing.myqcloud.com/image/202306080017513.png)

数独，进matrix里面提取出来

![image-20230608001804525](https://image-1311319331.cos.ap-beijing.myqcloud.com/image/202306080018617.png)

走完数独即可

![image-20230608143517557](https://image-1311319331.cos.ap-beijing.myqcloud.com/image/202306081435638.png)

## BWBA

逻辑部分

![image-20230609160049320](https://image-1311319331.cos.ap-beijing.myqcloud.com/image/202306091600527.png)

进入`encrypt`查看逻辑

像一道数学题，盘问gpt

![image-20230609160148251](https://image-1311319331.cos.ap-beijing.myqcloud.com/image/202306091601558.png)

是离散余弦变换(DCT)

逆离散余弦变换(IDCT)就可

[详解离散余弦变换（DCT）](https://zhuanlan.zhihu.com/p/85299446)



![image-20230609160359287](https://image-1311319331.cos.ap-beijing.myqcloud.com/image/202306091603437.png)

exp(copy别的师傅的)  网上也能搜到IDCT脚本

```
#include <iostream>
#include <vector>
#include <cmath>
 
std::vector<double> decrypt(const std::vector<double>& input)
{
    int v9 = input.size();
    std::vector<double> result(v9, 0.0);
 
    for (int i = 0; i < v9; ++i)
    {
        for (int j = 0; j < v9; ++j)
        {
            double v7 = (j == 0) ? sqrt(1.0 / v9) : sqrt(2.0 / v9);
            double v5 = input[j];
            double v6 = cos((i + 0.5) * (3.141592653589793 * j) / v9) * v5 * v7;
            result[i] += v6;
        }
        // 四舍五入
        result[i] = round(result[i]);
    }
 
    return result;
}
 
int main()
{
    std::vector<double> input = {370.75, 234.362, -58.0834, 59.8212, 88.8221, -30.2406, 21.8316, 49.9781, -33.5259, 2.69675, 43.5386, -30.2925, -28.0754, 27.593, -2.53962, -27.1883, -5.60777, -0.263937, 6.80326, 8.03022, -6.34681, -0.89506, -6.80685, -13.6088, 27.0958, 29.8439, -21.7688, -20.6925, -13.2155, -37.0994, 2.23679, 37.6699, -3.5, 9.85188, 57.2806, 13.5715, -20.7184, 8.6816, 3.59369, -4.5302, 4.22203, -28.8166, -23.695, 31.2268, 6.58823, -39.9966, -20.7877, -19.7624, -22.031, 16.3285, 2.07557, -26.2521, 16.1914, 18.3976, -26.9295, 3.03769, 41.0412, 20.2598, 14.991, 6.99392, -22.3752, -7.24466, 8.96299, -10.4874};
 
    std::vector<double> decrypted = decrypt(input);
 
    for (const auto& value : decrypted)
    {
        std::cout <<(char) value;
    }
    std::cout << std::endl;
 
    return 0;
}
//flag{9ab488a7-5b11-1b15-04f2-c230704ecf72}
```

或者用python的cv2库进行处理

```
import numpy as np
import cv2

x = np.array([370.75,234.362,-58.0834,59.8212,88.8221,-30.2406,21.8316,49.9781,-33.5259,2.69675,43.5386,-30.2925,-28.0754,27.593,-2.53962,-27.1883,-5.60777,-0.263937,6.80326,8.03022,-6.34681,-0.89506,-6.80685,-13.6088,27.0958,29.8439,-21.7688,-20.6925,-13.2155,-37.0994,2.23679,37.6699,-3.5,9.85188,57.2806,13.5715,-20.7184,8.6816,3.59369,-4.5302,4.22203,-28.8166,-23.695,31.2268,6.58823,-39.9966,-20.7877,-19.7624,-22.031,16.3285,2.07557,-26.2521,16.1914,18.3976,-26.9295,3.03769,41.0412,20.2598,14.991,6.99392,-22.3752,-7.24466,8.96299,-10.4874], dtype=np.float64)
x = cv2.idct(x)
x = x.ravel()
flag=''

for i in range(len(x)):
    flag+=chr(round(x[i]))
print(flag)
#flag{9ab488a7-5b11-1b15-04f2-c230704ecf72}
```

