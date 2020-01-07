# TFastDict
TFastDict是COM版本的字典。参考微软.NET Frameworks源码(C#)，
微软的字典算法采用空间换时间的方法，速度很快，处理大量数据时非常有用。 
This source code is the COM version of the dictionary. Microsoft's dictionary algorithm USES space for time. 
It's fast. Very useful when working with large amounts of data.

测试增删100万数据结果：   
![image](https://github.com/bzmework/TFastDict/blob/master/test.jpg)   
由于封装成了COM DLL，性能有所损耗，在可接受的范围之内。
