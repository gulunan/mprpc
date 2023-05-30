#include <iostream>
#include "mprpcapplication.h"
#include "user.pb.h"
#include "mprpcchannel.h"


int main(int argc, char** argv) {
    // 整个程序启动以后，想使用mprpc框架来享受rpc服务调用，一定需要先调用框架的初始化函数（只初始化一次）
    MprpcApplication::Init(argc, argv);

    // 演示调用远程发布的rpc方法Login
    gulu::UserServiceRpc_Stub stub(new MprpcChannel());
    // rpc方法的请求参数
    gulu::LoginRequest login_request;
    login_request.set_name("zhang san");
    login_request.set_pwd("123456");
    // rpc方法的响应
    gulu::LoginResponse login_response;
    // rpc方法的控制模块
    MprpcController login_controller;
    // 发起rpc方法的调用，是同步阻塞的rpc调用过程  底层是 MprpcChannel::CallMethod方法的调用
    // RpcChannel->RpcChannel::CallMethod 集中来做所有rpc方法调用的参数序列化和网络发送
    stub.Login(&login_controller, &login_request, &login_response, nullptr);

    // 一次rpc调用完成，读调用的结果
    if (login_controller.Failed()) {
        // 出现错误
        std::cout << login_controller.ErrorText() << std::endl;
    } else {
        if (0 == login_response.result().errcode()) {
            std::cout << "rpc login response success:" << login_response.success() << std::endl;
        } else {
            std::cout << "rpc login response error:" << login_response.result().errmsg() << std::endl;
        }
    }

    // 演示调用远程发布的rpc方法Register
    gulu::RegisterRequest register_request;
    register_request.set_id(2000);
    register_request.set_name("mprpc");
    register_request.set_pwd("666666");
    gulu::RegisterResponse register_response;
    MprpcController register_controller;
    // 以同步的方式发起rpc调用请求，等待返回结果
    stub.Register(&register_controller, &register_request, &register_response, nullptr);

    // 一次rpc调用完成，读调用的结果
    if (register_controller.Failed()) {
        // 出现错误
        std::cout << register_controller.ErrorText() << std::endl;
    } else {
        if (0 == register_response.result().errcode()) {
            std::cout << "rpc register response success:" << register_response.success() << std::endl;
        } else {
            std::cout << "rpc register response error:" << register_response.result().errmsg() << std::endl;
        }
    }

    return 0;
}