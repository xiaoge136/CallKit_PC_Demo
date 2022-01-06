## 接入流程

 - 从[官网](https://yunxin.163.com/im-sdk-demo)下载 NeRtc SDK（音视频通话2.0）和 NIM SDK（即时通讯）
 - 引入这两个SDK的到工程，包含头文件和库
 - 引入呼叫组件（CallKit），包含头文件 `necallkit_interface.h`，关注头文件中的 NeRtc 和 NIM 的头文件路径

## 接口说明

 - `IAvChatComponentEventHandler` 为呼叫组件回调接口类（通话过程发生状态变化等的回调），需要继承自它，然后调用 `createChatComponent` 创建的实例的 `regEventHandler` 接口注册
 - `createChatComponent` 创建呼叫组件的单例
 - `destroyChatComponent`销毁呼叫组件的单例
 - `setupAppKey` 设置appkey，创建内部资源
 - `release` 销毁内部资源
 - `call` 呼叫
 - `accept` 接听
 - `reject` 拒接
 - `hangup` 挂断
 - `cancel` 取消