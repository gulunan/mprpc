# set(SRC_LIST calluserservice.cc ../user.pb.cc)
# set(SRC_LIST callfriendservice.cc ../friend.pb.cc)
set(SRC_LIST calluserandfriendservice.cc ../userandfriend.pb.cc)

add_executable(consumer ${SRC_LIST})
target_link_libraries(consumer mprpc protobuf)