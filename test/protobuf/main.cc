#include "test.pb.h"
#include <iostream>
#include <string>
using namespace gulu;

int main() {
    // 封装了login响应对象的数据
    LoginResponse rsp;
    // 在定义的一个message类中 对于一个成员变量本身 又是另外一个message类对象 都会提供一个 mutable_xxx
    ResultCode* rc = rsp.mutable_result(); // mutable_result 返回了一个 ResultCode* 指针
    rc->set_errcode(1);
    rc->set_errmsg("login failed");
    rsp.set_success(false);

    // 对象数据序列化 -> char*
    std::string send_str2;
    if (rsp.SerializeToString(&send_str2)) {
        std::cout << send_str2.c_str() << std::endl;
    }

    // 从send_str2反序列化一个login响应对象
    LoginResponse rspB;
    if (rspB.ParseFromString(send_str2)) {
        std::cout << rspB.result().errcode() << std::endl;
        std::cout << rspB.result().errmsg() << std::endl;
        std::cout << rspB.success() << std::endl;
    }

    GetFriendListsResponse flrsp;
    ResultCode* rc2 = flrsp.mutable_result();
    rc2->set_errcode(0);
    rc2->set_errmsg("no error");

    // 列表 add_xxx 返回一个 User* 指针
    User* user1 = flrsp.add_friend_list();
    user1->set_name("zhang san");
    user1->set_age(20);
    user1->set_sex(User::MAN);

    User* user2 = flrsp.add_friend_list();
    user2->set_name("li si");
    user2->set_age(22);
    user2->set_sex(User::MAN);

    std::cout << flrsp.result().errcode() << std::endl;
    std::cout << flrsp.result().errmsg() << std::endl;
    std::cout << flrsp.friend_list_size() << std::endl;
    std::cout << flrsp.friend_list(0).name() << std::endl;
    std::cout << flrsp.friend_list(0).age() << std::endl;
    std::cout << flrsp.friend_list(0).sex() << std::endl;
    std::cout << flrsp.friend_list(1).name() << std::endl;
    std::cout << flrsp.friend_list(1).age() << std::endl;
    std::cout << flrsp.friend_list(1).sex() << std::endl;

    return 0;
}


int main1() {
    // 封装了login请求对象的数据
    LoginRequest req;
    req.set_name("zhang san");
    req.set_pwd("123456");

    // 对象数据序列化 -> char*
    std::string send_str;
    if (req.SerializeToString(&send_str)) {
        std::cout << send_str.c_str() << std::endl;
    }

    // 从send_str反序列化一个login请求对象
    LoginRequest reqB;
    if (reqB.ParseFromString(send_str)) {
        std::cout << reqB.name() << std::endl;
        std::cout << reqB.pwd() << std::endl;
    }

    return 0;
}