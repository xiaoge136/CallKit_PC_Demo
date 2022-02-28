/**
 * @file necallkit_interface.h
 * @brief 呼叫组件头文件
 * @copyright (c) 2014-2022, NetEase Inc. All rights reserved
 * @date 2021/12/30
 */

#ifndef NECALLKIT_INTERFACE_H_
#define NECALLKIT_INTERFACE_H_

#include <string>
#include <vector>
#include <memory>

// NeRtc
#include "nertc_engine_ex.h"
#include "nertc_video_device_manager.h"
#include "nertc_audio_device_manager.h"

// NIM
#include "nim_cpp_wrapper/nim_cpp_api.h"
#include "nim_tools_cpp_wrapper/nim_tools_http_cpp.h"

namespace necall_kit {

/**
 * 呼叫类型，与nim::NIMSignalingType同值
 */
enum AVCHAT_CALL_TYPE {
    kAvChatAudio = 1,  /**< 音频类型 */
    kAvChatVideo = 2,  /**< 视频类型 */
    kAvChatCustom = 3, /**< 自定义 */
};

/**
 * @brief 错误码
 */
enum AVCHAT_ERROR_CODE {
    kAvChatNoError = 200,                   /**< 无错误 */
    kAvChatErrorCallWhenCalling = 20002,    /**< 已在呼叫中 */
    kAvChatErrorCallWhenCalled = 20003,     /**< 正在被呼叫 */
    kAvChatErrorCallWhenInCall = 20004,     /**< 已在通话中 */
    kAvChatErrorHangupWhenIdle = 20005,     /**< 挂断时，不在通话中 */
    kAvChatErrorLeaveWhenIdle = 20009,      /**< idle 状态时调用leave，不在通话中 */
    kAvChatErrorLeaveWhenCalling = 20010,   /**< Calling 状态时调用leave，不在通话中 */
    kAvChatErrorLeaveWhenCalled = 20011,    /**< Called 状态时调用leave，不在通话中 */
    kAvChatErrorCancelWhenIdle = 20013,     /**< idle 状态时调用cancel，未发起通话 */
    kAvChatErrorCancelWhenCalled = 20015,   /**< Called 状态时调用cancel，未发起通话 */
    kAvChatErrorCancelWhenInCall = 20016,   /**< Called 状态时调用cancel，不能取消 */
    kAvChatErrorAcceptWhenIdle = 20017,     /**< idle状态时调用Accept，不存在需要接通的呼叫 */
    kAvChatErrorAcceptWhenCalling = 20018,  /**< Calling状态时调用Accept，不存在需要接通的呼叫 */
    kAvChatErrorAcceptWhenInCall = 20020,   /**< InCall状态时调用Accept，不存在需要接通的呼叫 */
    kAvChatErrorRejectWhenIdle = 20021,     /**< idle状态时调用Reject，不存在需要拒绝的呼叫 */
    kAvChatErrorRejectWhenCalling = 20022,  /**< Calling状态时调用Reject，不存在需要拒绝的呼叫 */
    kAvChatErrorRejectWhenInCall = 20024,   /**< InCall状态时调用Reject，不存在需要拒绝的呼叫 */
    kAvChatErrorSwitchCallTypeWhenIdle = 20025,     /**< Idle状态时调用SwitchCallType，只能在呼叫中进行切换 */
    kAvChatErrorSwitchCallTypeWhenCalling = 20026,  /**< Calling状态时调用SwitchCallTypet，只能在呼叫中进行切换 */
    kAvChatErrorSwitchCallTypeWhenCalled = 20027,   /**< Called状态时调用SwitchCallType，只能在呼叫中进行切换 */
};

/**
 * @brief 组件事件回调接口类（通话过程发生状态变化时回调）
 */
class IAvChatComponentEventHandler {
public:
    /**
     * @brief 收到邀请
     * @param invitor 邀请者的用户id
     * @param userIds 通话里的用户id列表
     * @param isFromGroup 是否为多人通话
     * @param groupId 透传groupCall传入的groupId，1to1则为空
     * @param type 呼叫类型 {@see AVCHAT_CALL_TYPE}
     * @param attachment 附件信息
     * @return void
     */
    virtual void onInvited(const std::string& invitor, std::vector<std::string> userIds, bool isFromGroup, const std::string& groupId,
                           AVCHAT_CALL_TYPE type, const std::string& attachment){};

    /**
     * @brief 收到用户接受
     * @param userId 用户id
     * @return void
     */
    virtual void onUserAccept(const std::string& userId) {}

    /**
     * @brief 收到用户拒绝
     * @param userId  用户id
     * @return void
     */
    virtual void onUserReject(const std::string& userId) {}

    /**
     * @brief 收到用户加入
     * @param userId 用户id
     * @return void
     */
    virtual void onUserEnter(const std::string& userId) {}

    /**
     * @brief 收到用户离开
     * @param userId 用户id
     * @return void
     */
    virtual void onUserLeave(const std::string& userId) {}

    /**
     * @brief 收到用户断开连接
     * @param userId 用户id
     * @return void
     */
    virtual void onUserDisconnect(const std::string& userId) {}

    /**
     * @brief 收到用户忙碌
     * @param userId 用户id
     * @return void
     */
    virtual void onUserBusy(const std::string& userId) {}

    /**
     * @brief 收到用户取消
     * @param userId 用户id
     * @return void
     */
    virtual void onUserCancel(const std::string& userId) {}

    /**
     * @brief 收到NeRtc断开连接
     * @param reason 断开的原因 {@see nertc::NERtcErrorCode}
     * @return void
     */
    virtual void onDisconnect(int reason) {}

    /**
     * @brief 收到呼叫超时
     * @return void
     */
    virtual void onCallingTimeOut() {}

    /**
     * @brief 收到视频切换成音频
     * @return void
     */
    virtual void OnVideoToAudio() {}

    /**
     * @brief 收到呼叫结束
     * @return void
     */
    virtual void onCallEnd() {}

    /**
     * @brief 收到NeRtc错误
     * @param errCode 错误码 {@see nertc::NERtcErrorCode}
     * @param errMsg  错误信息
     * @return void
     */
    virtual void onError(int errCode, const std::string& errMsg) {}

    /**
     * @brief 收到其他端接受
     * @return void
     */
    virtual void onOtherClientAccept() {}

    /**
     * @brief 收到其他端拒绝
     * @return void
     */
    virtual void onOtherClientReject() {}

    /**
     * @brief 收到用户网络质量
     * @param mapNetworkQuality 用户的网络质量列表
     * @return void
     */
    virtual void onUserNetworkQuality(const std::map<std::string, nertc::NERtcNetworkQualityInfo>& mapNetworkQuality) {}

    /**
     * @brief 收到用户视频开关状态
     * @param userId 用户id
     * @param available 视频是开关状态，ture开，false关
     * @return void
     */
    virtual void onCameraAvailable(const std::string& userId, bool available) {}

    /**
     * @brief 收到用户音频开关状态
     * @param userId 用户id
     * @param available 视频是开关状态，ture开，false关
     * @return void
     */
    virtual void onAudioAvailable(const std::string& userId, bool available) {}

    /**
     * @brief 本地视频状态改变
     * @param isEnable 视频状态，ture开，false关
     * @return void
     */
    virtual void onLocalCameraStateChanged(bool isEnable) {}

    /**
     * @brief 音频音量改变
     * @param value 音量值
     * @param isRecord 是否为麦克风，true麦克风，false扬声器
     * @return void
     */
    virtual void onAudioVolumeChanged(unsigned char value, bool isRecord) {}

    /**
     * @brief 当前用户加入音视频的回调
     * @param accid 用户id，即userId
     * @param uid 音视频里的用户id
     * @param cid 音视频通道id
     * @param cname 音视频通道名称
     * @param errCode 错误码 {@see nertc::NERtcErrorCode}
     * @return void
     */
    virtual void onJoinChannel(const std::string& accid, nertc::uid_t uid, nertc::channel_id_t cid, const std::string& cname, int errCode) {}

    /**
     * @brief 提示房间内谁正在说话及说话者瞬时音量的回调
     * 该回调默认为关闭状态。可以通过 getRtcEngine() 的 enableAudioVolumeIndication 方法开启。开启后，无论房间内是否有人说话，SDK 都会按 enableAudioVolumeIndication 方法中设置的时间间隔触发该回调。
     * 在返回的 speakers 数组中:
     * - 如果有 uid 出现在上次返回的数组中，但不在本次返回的数组中，则默认该 uid 对应的远端用户没有说话。
     * - 如果volume 为 0，表示该用户没有说话。
     * - 如果speakers 数组为空，则表示此时远端没有人说话
     @param listAudioVolumeInfo 每个说话者的用户 ID 和音量信息
     @param totalVolume （混音后的）总音量，取值范围为 [0,100]。
     */
    virtual void onRemoteAudioVolumeIndication(const std::list<nertc::NERtcAudioVolumeInfo>& listAudioVolumeInfo, int totalVolume) {}

    /**
     * @brief 提示频道内本地用户瞬时音量的回调
     * 该回调默认禁用。可以通过 \ref IRtcEngineEx::enableAudioVolumeIndication "enableAudioVolumeIndication" 方法开启。
     * 开启后，本地用户说话，SDK 会按  \ref IRtcEngineEx::enableAudioVolumeIndication "enableAudioVolumeIndication" 方法中设置的时间间隔触发该回调。
     * 如果本地用户将自己静音（调用了 \ref IRtcEngineEx::muteLocalAudioStream "muteLocalAudioStream"），SDK 将音量设置为 0 后回调给应用层。
     * @param volume （混音后的）音量，取值范围为 [0,100]。
     */
    virtual void onLocalAudioVolumeIndication(int volume) {}
};

/**
 * 组件操作回调（返回调用组件接口的错误码）
 */
using AvChatComponentOptCb = std::function<void(int errCode)>;

/**
 * 获取token函数
 */
using GetTokenServiceFunc = std::function<void(int64_t uid, std::function<void(const std::string& token)> onGetToken)>;

/**
 * @brief 组件接口类
 */
class IAvChatComponent {
public:
    virtual ~IAvChatComponent() = default;
    /**
     * @brief 创建内部资源
     * @param key appkey
     * @param useRtcSafeMode 是否使用安全模式，默认true使用，false不使用
     * @return void
     */
    virtual void setupAppKey(const std::string& key, bool useRtcSafeMode = true) = 0;

    /**
     * @brief 释放内部资源
     * @return void
     */
    virtual void release() = 0;

    /**
     * @brief 设置本地视频画布
     * @param canvas 画布
     * @return void
     */
    virtual void setupLocalView(nertc::NERtcVideoCanvas* canvas) = 0;

    /**
     * @brief 设置远端视频画布
     * @param canvas 画布
     * @param userId 用户id
     * @return void
     */
    virtual void setupRemoteView(nertc::NERtcVideoCanvas* canvas, const std::string& userId) = 0;

    /**
     * @brief 切换视频
     * @note PC暂没实现摄像头切换
     * @return void
     */
    virtual void switchCamera() = 0;

    /**
     * @brief 打开本地视频
     * @param enable 是否打开本地视频，true打开，false关闭
     * @return void
     */
    virtual void enableLocalVideo(bool enable) = 0;

    /**
     * @brief 打开本地音频
     * @param mute 是否打开本地音频，false打开，true关闭
     * @return void
     */
    virtual void muteLocalAudio(bool mute) = 0;

    /**
     * @brief 静音本地音频
     * @param enable 是否静音本地音频，false静音，true不静音
     * @return void
     */
    virtual void enableAudioPlayout(bool enable) = 0;

    /**
     * @brief 呼叫
     * @param userId 对方用户id
     * @param type 呼叫类型 {@see AVCHAT_CALL_TYPE}
     * @param attachment 附加信息
     * @param cb 结果回调
     * @return void
     */
    virtual void call(const std::string& userId, AVCHAT_CALL_TYPE type, const std::string& attachment, AvChatComponentOptCb cb) = 0;

    /**
     * @brief 接受
     * @param cb 结果回调
     * @return void
     */
    virtual void accept(AvChatComponentOptCb cb) = 0;

    /**
     * @brief 拒绝
     * @param cb 结果回调
     * @return void
     */
    virtual void reject(AvChatComponentOptCb cb) = 0;

    /**
     * @brief 挂断
     * @param cb 结果回调
     * @return void
     */
    virtual void hangup(AvChatComponentOptCb cb) = 0;

    /**
     * @brief 取消
     * @param cb 结果回调
     * @return void
     */
    virtual void cancel(AvChatComponentOptCb cb) = 0;

    /**
     * @brief 离开
     * @param cb 结果回调
     * @return void
     */
    virtual void leave(AvChatComponentOptCb cb) = 0;

    /**
     * @brief 注册事件监听器
     * @param compEventHandler 事件监听器
     * @return void
     */
    virtual void regEventHandler(std::shared_ptr<IAvChatComponentEventHandler> compEventHandler) = 0;

    /**
     * @brief 本地预览
     * @param start 是否开始本地预览，true开始，false停止
     * @return void
     */
    virtual void startVideoPreview(bool start = true) = 0;

    /**
     * @brief 切换呼叫类型
     * @note 目前只支持视频切换到音频
     * @param userId 用户id
     * @param type 呼叫类型 {@see AVCHAT_CALL_TYPE}
     * @return void
     */
    virtual void switchCallType(std::string userId, AVCHAT_CALL_TYPE type, AvChatComponentOptCb cb) = 0;

    /**
     * @brief 开始音频设备回路测试
     * @param interval 回调时间间隔，单位为毫秒
     * @return void
     */
    virtual void startAudioDeviceLoopbackTest(int interval) = 0;

    /**
     * @brief 停止音频设备回路测试
     * @return void
     */
    virtual void stopAudioDeviceLoopbackTest() = 0;

    /**
     * @brief 请求token
     * @param uid 用户id
     * @return void
     */
    virtual void requestTokenValue(int64_t uid) = 0;

    /**
     * @brief 设置视频画质质量
     * @param type 画质类型
     * @return void
     */
    virtual void setVideoQuality(nertc::NERtcVideoProfileType type) = 0;

    /**
     * @brief 打开用户音频
     * @param userId 用户id
     * @param bOpen 是否打开用户音频，true打开，false关闭
     * @return void
     */
    virtual void setAudioMute(std::string userId, bool bOpen) = 0;

    /**
     * @brief 获取频道信息
     * @return nim::SignalingCreateResParam
     */
    virtual nim::SignalingCreateResParam getCreatedChannelInfo() = 0;

    /**
     * @brief 呼叫等待超时
     * @return void
     */
    //virtual void onWaitingTimeout() = 0;

    /**
     * @brief 设置麦克风音量
     * @param value 音量，取值范围为[0, 400]
     * @return int
     * @retval 0 成功
     * @retval 非0 失败
     */
    virtual int setRecordDeviceVolume(int value) = 0;

    /**
     * @brief 设置扬声器音量
     * @param value 音量，取值范围为[0, 400]
     * @return int
     * @retval 0 成功
     * @retval 非0 失败
     */
    virtual int setPlayoutDeviceVolume(int value) = 0;

    /**
     * @brief 获取音量
     * @param isRecord 是否为麦克风，true是麦克风，false是扬声器
     * @return uint32_t 范围为[0, 255]
     */
    virtual uint32_t getAudioVolumn(bool isRecord) = 0;

    /**
     * @brief 获取本地设备列表
     * @param recordDevicesNames 麦克风设备名列表
     * @param recordDevicesIds 麦克风设备id列表
     * @param playoutDevicesNames 扬声器设备名列表
     * @param playoutDevicesIds 扬声器设备id列表
     * @param videoDeviceNames 视频设备名列表
     * @param videoDeviceIds 视频设备id列表
     * @return void
     */
    virtual void getLocalDeviceList(std::vector<std::string>* recordDevicesNames, std::vector<std::string>* recordDevicesIds,
                            std::vector<std::string>* playoutDevicesNames, std::vector<std::string>* playoutDevicesIds,
                            std::vector<std::string>* videoDeviceNames, std::vector<std::string>* videoDeviceIds) = 0;

    /**
     * @brief 设置当前视频设备
     * @param deviceId 视频设备id
     * @return void
     */
    virtual void setVideoDevice(const std::string& deviceId) = 0;

    /**
     * @brief 获取当前视频设备id
     * @return std::string
     */
    virtual std::string getVideoDevice() = 0;

    /**
     * @brief 设置音频设备
     * @param id 音频设备id
     * @param isRecord 是否为麦克风，true是麦克风，false是扬声器
     * @return void
     */
    virtual void setAudioDevice(const std::string& id, bool isRecord) = 0;

    /**
     * @brief 获取音频设备
     * @param isRecord 是否为麦克风，true是麦克风，false是扬声器
     * @return std::string
     */
    virtual std::string getAudioDevice(bool isRecord) = 0;

    /**
     * @brief 设置呼叫/接听超时时间
     * @param period 超时时间，单位毫秒
     * @return void
     */
    virtual void setTimeout(uint32_t period) = 0;

    /**
     * @brief 获取NeRtc引擎
     * @return nertc::IRtcEngineEx*
     */
    virtual nertc::IRtcEngineEx* getRtcEngine() = 0;

    /**
     * @brief 设置token服务
     * @attention 在线上环境中，token的获取需要放到您的应用服务端完成，然后由服务器通过安全通道把token传递给客户端，
     * Demo中使用的URL仅仅是demoserver，不要在您的应用中使用，详细请参考: http://dev.netease.im/docs?doc=server
     * @param getTokenService token 服务
     * @return void
     */
    virtual void setTokenService(GetTokenServiceFunc getTokenService) = 0;
};

#define NECALLKIT_VER "1.3.1"   /**< 呼叫组件版本 */

/**
 * @brief 创建组件实例
 * @return IAvChatComponent* 组件对象
 */
IAvChatComponent* createChatComponent();

/**
 * @brief 销毁组件实例
 * @return void
 */
void destroyChatComponent();

}  // namespace necall_kit

#endif  // !NECALLKIT_INTERFACE_H_