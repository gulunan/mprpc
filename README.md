# mprpc
mprpc分布式网络通信框架

基于muduo高性能网络库
Protobuf数据序列化和反序列化协议
ZooKeeper分布式一致性协调服务
异步日志
conf配置文件读取
CMake构建集成编译环境

编译方式
./autobuild.sh一键编译
或
rm -rf `pwd`/build/*
cd `pwd`/build &&
	cmake .. &&
	make
cd ..
cp -r `pwd`/src/include `pwd`/lib

需要安装配置muduo网络库
需要安装配置Protobuf
需要安装配置ZooKeeper及其原生开发API的C/C++接口