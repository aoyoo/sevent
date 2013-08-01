##sevent##
##muduo##
拙劣的[muduo](https://github.com/chenshuo/muduo)仿制品

主要修改：
* 去掉定时器
* 去掉onConnection回调函数，用户看不到对Connection的操作
* onMessage回调函数参数由Buffer变成Message，即收到一个（多个）完整的Message后才会调用用户的onMessage函数
* 
* 每个EventLoopThread直接持有一个EventLoop
* Server额外持有一个EventLoopThread用于Accept，Server的主线程空闲
