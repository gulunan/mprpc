#include <iostream>
#include <string>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "mprpcprovider.h"

// using namespace gulu;

/*
    UserService原来是一个本地服务，提供了两个进程内的本地方法，Login和GetFriendLists
*/

// 使用在rpc服务发布端（rpc服务提供者）
class UserService : public gulu::UserServiceRpc {
public:
    bool Login(std::string name, std::string pwd) {
        std::cout << "doing local service: Login" << std::endl;
        std::cout << "name:" << name << " pwd:" << pwd << std::endl;
        return false;
    }

    bool Register(uint32_t id, std::string name, std::string pwd) {
        std::cout << "doing local service: Register" << std::endl;
        std::cout << "id:" << id << " name:" << name << " pwd:" << pwd << std::endl;
        return true;
    }

    /*
        重写基类UserServiceRpc的虚函数，下面这些方法都是框架直接调用的
        1、caller  --->  Login(LoginRequest)  --->  muduo  --->  callee
        2、callee  --->  Login(LoginRequest)  --->  交到下面重写的这个Login方法上
    */
    void Login(google::protobuf::RpcController* controller,
               const ::gulu::LoginRequest* request,
               ::gulu::LoginResponse* response,
               ::google::protobuf::Closure* done) {
        // 框架给业务上报了请求参数 LoginRequest，应用业务获取相应的数据做本地业务
        std::string name = request->name();
        std::string pwd = request->pwd();

        // 做本地业务
        bool login_result = Login(name, pwd);

        // 把响应写入 LoginResponse 包括 错误码errcode、错误消息errmsg、返回值success
        gulu::ResultCode* rc = response->mutable_result();
        rc->set_errcode(0);
        rc->set_errmsg("");
        response->set_success(login_result);

        // 执行回调操作，执行响应对象数据的序列化和网络发送，都是由框架来完成的
        // Closure是一个抽象类，调用对象done重写了的从基类Closure继承而来的纯虚函数Run()
        done->Run();
    }

    void Register(google::protobuf::RpcController* controller,
                  const ::gulu::RegisterRequest* request,
                  ::gulu::RegisterResponse* response,
                  ::google::protobuf::Closure* done) {
        uint32_t id = request->id();
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool register_result = Register(id, name, pwd);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        response->set_success(register_result);

        done->Run();
    }

private:

};

int main(int argc, char** argv) {
    // UserService us;
    // us.Login("xxx", "xxx");

    // 调用框架的初始化操作  provider -i config.conf
    MprpcApplication::Init(argc, argv);

    // provider是一个rpc网络服务对象，把UserService对象发布到rpc节点上
    MprpcProvider provider;
    provider.NotifyService(new UserService());

    // 启动一个rpc服务发布节点，Run以后，进程进入阻塞状态，等待远程的rpc的调用请求
    provider.Run();

    return 0;
}