# 【中国电信2023巅峰极客Reverse】

## g0Re

改回UPX特征，脱壳

![image-20230721231554501](https://image-1311319331.cos.ap-beijing.myqcloud.com/image/202307212315537.png)

![image-20230721231449127](https://image-1311319331.cos.ap-beijing.myqcloud.com/image/202307212314295.png)

![image-20230721231800075](https://image-1311319331.cos.ap-beijing.myqcloud.com/image/202307212318126.png)

主要逻辑如下

![image-20230721233622354](https://image-1311319331.cos.ap-beijing.myqcloud.com/image/202307212336439.png)

`v32:`

![image-20230721234009368](https://image-1311319331.cos.ap-beijing.myqcloud.com/image/202307212340413.png)



![image-20230721233643960](https://image-1311319331.cos.ap-beijing.myqcloud.com/image/202307212336052.png)

`base表：`

![image-20230722000631512](https://image-1311319331.cos.ap-beijing.myqcloud.com/image/202307220006554.png)

```
456789}#IJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123ABCDEFGH
```

动调拿到aes秘钥`wvgitbygwbk2b46d`

求出加密前的base字符串：

```
enc=[0xE6, 0xCE, 0x89, 0xC8, 0xCF, 0xC5, 0xF5, 0xC9, 0xD2, 0xD9,
  0xC0, 0x91, 0xCE, 0x7F, 0xAC, 0xCC, 0xE9, 0xCF, 0xB7, 0xC0, 
  0x96, 0xD4, 0xEA, 0x92, 0xE2, 0xD7, 0xDF, 0x84, 0xCB, 0xA5, 
  0xAE, 0x93, 0xA6, 0xCA, 0xBE, 0x97, 0xDF, 0xCE, 0xF0, 0xC9, 
  0xB7, 0xE1, 0xAE, 0x6B, 0xC4, 0xB1, 0x65, 0xDB, 0xCE, 0xED, 
  0x92, 0x93, 0xD6, 0x8C, 0xED, 0xC3, 0xA3, 0xDA, 0x94, 0xA5, 
  0xAA, 0xB2, 0xB5, 0xA7, 0x55]
key=b"wvgitbygwbk2b46d"

base = ""
for i in range(len(enc)):
    base+=chr(((enc[i]-key[i%16])^0x1a) &0xff)
print(base)
```

![image-20230722000928551](https://image-1311319331.cos.ap-beijing.myqcloud.com/image/202307220009665.png)

```
flag{g0_1s_th3_b3st_1anguage_1n_the_wOrld!_xxx}
```

