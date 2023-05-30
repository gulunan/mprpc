#include "zookeeperutil.h"
#include "mprpcapplication.h"
#include <semaphore.h>
#include <iostream>

// 全局的watcher观察器   zkserver给zkclient的通知
void global_watcher(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx) {
    // 回调的消息类型是和会话相关的消息类型
    if (type == ZOO_SESSION_EVENT) {
        // zkclient和zkserver连接成功
		if (state == ZOO_CONNECTED_STATE) {
            // 从句柄获取绑定的信号量 +1
			sem_t* sem = (sem_t*)zoo_get_context(zh);
            sem_post(sem);
		}
	}
}

ZkClient::ZkClient() : m_zhandle(nullptr) {

}

ZkClient::~ZkClient() {
    if (m_zhandle != nullptr) {
        // 关闭句柄，释放资源 类似于MySQL_Conn
        zookeeper_close(m_zhandle);
    }
}

// zkclient启动，连接zkserver
void ZkClient::Start() {
    std::string host = MprpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    std::string port = MprpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    std::string connstr = host + ":" + port;

    /*
        zookeeper_my: 多线程版本
        zookeeper的API客户端程序提供了三个线程
        API调用线程
        网络I/O线程 pthread_create poll
        watcher回调线程 pthread_create
    */

   // 调用API接口，异步建立连接
    m_zhandle = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);
    // 返回值不代表连接是否成功，只是代表句柄的内存开辟
    if (nullptr == m_zhandle) {
        std::cout << "zookeeper_init error!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 创建并初始化信号量
    sem_t sem;
    sem_init(&sem, 0, 0);
    // 句柄绑定信号量
    zoo_set_context(m_zhandle, &sem);

    // 阻塞等待zk服务器响应
    sem_wait(&sem);
    std::cout << "zookeeper_init success!" << std::endl;
}

// 在zkserver上根据指定的path创建znode节点
void ZkClient::Create(const char* path, const char* data, int datalen, int state) {
    char path_buffer[128];
    int bufferlen = sizeof(path_buffer);
    int flag;
    // 判断path表示的znode节点是否存在，如果存在不重复创建
    flag = zoo_exists(m_zhandle, path, 0, nullptr);
    if (ZNONODE == flag) {
        // path对应的znode节点不存在，创建指定path的znode节点
        flag = zoo_create(m_zhandle, path, data, datalen, &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, bufferlen);
        if (ZOK == flag) {
            std::cout << "znode create success... path:" << path << std::endl;
        } else {
            std::cout << "flag:" << flag << std::endl;
            std::cout << "znode create error... path:" << path << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

// 传入参数，根据参数指定的znode节点路径path，获取znode存储的值
std::string ZkClient::GetData(const char* path) {
    char buffer[64];
    int bufferlen = sizeof(buffer);
    int flag = zoo_get(m_zhandle, path, 0, buffer, &bufferlen, nullptr);
    if (ZOK != flag) {
        std::cout << "get znode error... path:" << path << std::endl;
        return "";
    } else {
        return buffer;
    }
}