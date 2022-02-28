/**
 * @file necallkit.h
 * @brief 呼叫组件头文件
 * @copyright (c) 2014-2022, NetEase Inc. All rights reserved
 * @author Martin
 * @date 2021/05/28
 */

#ifndef AVCHAT_COMPONENT_H_
#define AVCHAT_COMPONENT_H_

#include "include/necallkit_interface.h"
#include "stable.h"

namespace necall_kit {

/**
 * 呼叫模式类型
 */
enum AVCHAT_MODULE_TYPE {
    kAvChatP2P = 0, /**< 单人 */
    kAvChatMulti    /**< 多人 */
};

enum NIMNetCallStatus {
    kNIMNetCallStatusComplete = 1, /**< 1:通话完成 */
    kNIMNetCallStatusCanceled,     /**< 2:通话取消 */
    kNIMNetCallStatusRejected,     /**< 3:通话拒绝 */
    kNIMNetCallStatusTimeout,      /**< 4:超时未接听 */
    kNIMNetCallStatusBusy,         /**< 5:对方忙线 */
};

class Timer;

/**
 * @brief 组件实现类
 */
class AvChatComponent : public IAvChatComponent, public nertc::IRtcEngineEventHandlerEx, public nertc::IRtcMediaStatsObserver {
    /**
     * 呼叫的状态
     */
    enum ComponentStatus {
        idle = 0, /**< 空闲 */
        calling,  /**< 呼叫中 */
        called,   /**< 被邀请 */
        inCall,   /**< 在通道 */
    };
    
private:
    AvChatComponent();
    AvChatComponent(const AvChatComponent &other) = delete;
    AvChatComponent& operator=(const AvChatComponent &other) = delete;

public:
    SINGLETONG(AvChatComponent);
    
public:
    /**
     * @brief 析构函数
     */
    ~AvChatComponent();

    /**
     * @brief 创建内部资源
     * @param key appkey
     * @param useRtcSafeMode 是否使用安全模式，默认true使用，false不使用
     * @return void
     */
    virtual void setupAppKey(const std::string& key, bool useRtcSafeMode = true) override;

    /**
     * @brief 释放内部资源
     * @return void
     */
    virtual void release() override;

    /**
     * @brief 登录
     * @param account 账号
     * @param token token
     * @param cb 登录结果回调
     * @return void
     */
    void login(const std::string& account, const std::string& token, AvChatComponentOptCb cb);

    /**
     * @brief 登出
     * @param cb 登出结构回调
     * @return void
     */
    void logout(AvChatComponentOptCb cb);

    /**
     * @brief 设置本地视频画布
     * @param canvas 画布
     * @return void
     */
    virtual void setupLocalView(nertc::NERtcVideoCanvas* canvas) override;

    /**
     * @brief 设置远端视频画布
     * @param canvas 画布
     * @param userId 用户id
     * @return void
     */
    virtual void setupRemoteView(nertc::NERtcVideoCanvas* canvas, const std::string& userId) override;

    /**
     * @brief 切换视频
     * @note PC暂没实现摄像头切换
     * @return void
     */
	virtual void switchCamera() override;

    /**
     * @brief 打开本地视频
     * @param enable 是否打开本地视频，true打开，false关闭
     * @return void
     */
    virtual void enableLocalVideo(bool enable) override;

    /**
     * @brief 打开本地音频
     * @param mute 是否打开本地音频，false打开，true关闭
     * @return void
     */
    virtual void muteLocalAudio(bool mute) override;

    /**
     * @brief 静音本地音频
     * @param enable 是否静音本地音频，false静音，true不静音
     * @return void
     */
    virtual void enableAudioPlayout(bool enable) override;

    /**
     * @brief 呼叫
     * @param userId 对方用户id
     * @param type 呼叫类型 {@see AVCHAT_CALL_TYPE}
     * @param attachment 附加信息
     * @param cb 结果回调
     * @return void
     */
    virtual void call(const std::string& userId, AVCHAT_CALL_TYPE type, const std::string& attachment, AvChatComponentOptCb cb) override;

    /**
     * @brief 接受
     * @param cb 结果回调
     * @return void
     */
    virtual void accept(AvChatComponentOptCb cb) override;

    /**
     * @brief 拒绝
     * @param cb 结果回调
     * @return void
     */
    virtual void reject(AvChatComponentOptCb cb) override;

    /**
     * @brief 挂断
     * @param cb 结果回调
     * @return void
     */
    virtual void hangup(AvChatComponentOptCb cb) override;

    /**
     * @brief 取消
     * @param cb 结果回调
     * @return void
     */
    virtual void cancel(AvChatComponentOptCb cb) override;

    /**
     * @brief 离开
     * @param cb 结果回调
     * @return void
     */
    virtual void leave(AvChatComponentOptCb cb) override;

    /**
     * @brief 注册事件监听器
     * @param compEventHandler 事件监听器
     * @return void
     */
    virtual void regEventHandler(std::shared_ptr<IAvChatComponentEventHandler> compEventHandler) override;

    /**
     * @brief 本地预览
     * @param start 是否开始本地预览，true开始，false停止
     * @return void
     */
    virtual void startVideoPreview(bool start = true) override;

    /**
     * @brief 切换呼叫类型
     * @note 目前只支持视频切换到音频
     * @param user_id 用户id
     * @param type 呼叫类型 {@see AVCHAT_CALL_TYPE}
     * @return void
     */
    virtual void switchCallType(std::string user_id, AVCHAT_CALL_TYPE type, AvChatComponentOptCb cb) override;

    /**
     * @brief 开始音频设备回路测试
     * @param interval 回调时间间隔，单位为毫秒
     * @return void
     */
    virtual void startAudioDeviceLoopbackTest(int interval) override;

    /**
     * @brief 停止音频设备回路测试
     * @return void
     */
    virtual void stopAudioDeviceLoopbackTest() override;

    /**
     * @brief 请求token
     * @param uid 用户id
     * @return void
     */
    virtual void requestTokenValue(int64_t uid) override;

    /**
     * @brief 设置视频画质质量
     * @param type 画质类型
     * @return void
     */
    virtual void setVideoQuality(nertc::NERtcVideoProfileType type) override;

    /**
     * @brief 打开用户音频
     * @param user_id 用户id
     * @param bOpen 是否打开用户音频，true打开，false关闭
     * @return void
     */
    virtual void setAudioMute(std::string user_id, bool bOpen) override;

    /**
     * @brief 获取频道信息
     * @return nim::SignalingCreateResParam
     */
	virtual nim::SignalingCreateResParam getCreatedChannelInfo() override;

    /**
     * @brief 呼叫等待超时
     * @return void
     */
    //virtual void onWaitingTimeout() override;

    /**
     * @brief 设置麦克风音量
     * @param value 音量，取值范围为[0, 400]
     * @return int
     * @retval 0 成功
     * @retval 非0 失败
     */
    virtual int setRecordDeviceVolume(int value) override;

    /**
     * @brief 设置扬声器音量
     * @param value 音量，取值范围为[0, 400]
     * @return int
     * @retval 0 成功
     * @retval 非0 失败
     */
    virtual int setPlayoutDeviceVolume(int value) override;

    /**
     * @brief 获取音量
     * @param isRecord 是否为麦克风，true是麦克风，false是扬声器
     * @return uint32_t 范围为[0, 255]
     */
    virtual uint32_t getAudioVolumn(bool isRecord) override;

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
                            std::vector<std::string>* videoDeviceNames, std::vector<std::string>* videoDeviceIds) override;

    /**
     * @brief 设置当前视频设备
     * @param id 视频设备id
     * @return void
     */
    virtual void setVideoDevice(const std::string& id) override;

    /**
     * @brief 获取当前视频设备id
     * @return std::string
     */
    virtual std::string getVideoDevice() override;

    /**
     * @brief 设置音频设备
     * @param id 音频设备id
     * @param isRecord 是否为麦克风，true是麦克风，false是扬声器
     * @return void
     */
    virtual void setAudioDevice(const std::string& id, bool isRecord) override;

    /**
     * @brief 获取音频设备
     * @param isRecord 是否为麦克风，true是麦克风，false是扬声器
     * @return std::string
     */
    virtual std::string getAudioDevice(bool isRecord) override;

    /**
     * @brief 设置呼叫/接听超时时间
     * @param period 超时时间，单位毫秒
     * @return void
     */
    virtual void setTimeout(uint32_t period) override;

    /**
     * @brief 获取G2引擎
     * @return nertc::IRtcEngineEx*
     */
	virtual nertc::IRtcEngineEx* getRtcEngine() override;

    /**
     * @brief 设置token服务
     * @attention 在线上环境中，token的获取需要放到您的应用服务端完成，然后由服务器通过安全通道把token传递给客户端，
     * Demo中使用的URL仅仅是demoserver，不要在您的应用中使用，详细请参考: http://dev.netease.im/docs?doc=server
     * @param getTokenService token服务
     * @return void
     */
	virtual void setTokenService(GetTokenServiceFunc getTokenService);

protected:
    void signalingCreateCb(int errCode, std::shared_ptr<nim::SignalingResParam> res_param, AvChatComponentOptCb cb);
    void signalingJoinCb(int errCode, std::shared_ptr<nim::SignalingResParam> res_param, AvChatComponentOptCb cb, const std::string& channelId);
    void signalingInviteCb(int errCode, std::shared_ptr<nim::SignalingResParam> res_param, AvChatComponentOptCb cb);
    // 被叫方调用accept的结果的回调
    void signalingAcceptCb(int errCode, std::shared_ptr<nim::SignalingResParam> res_param, AvChatComponentOptCb cb);
    void signalingRejectCb(int errCode, std::shared_ptr<nim::SignalingResParam> res_param, AvChatComponentOptCb cb);
    void signalingCloseCb(int errCode, std::shared_ptr<nim::SignalingResParam> res_param, AvChatComponentOptCb cb);
    void signalingLeaveCb(int errCode, std::shared_ptr<nim::SignalingResParam> res_param, AvChatComponentOptCb cb);
    void signalingControlCb(int errCode, std::shared_ptr<nim::SignalingResParam> res_param);

    void handleInvited(std::shared_ptr<nim::SignalingNotifyInfo> notifyInfo);
    void handleControl(std::shared_ptr<nim::SignalingNotifyInfo> notifyInfo);
    void handleAccepted(std::shared_ptr<nim::SignalingNotifyInfo> notifyInfo);
    void handleOtherClientAccepted(std::shared_ptr<nim::SignalingNotifyInfo> notifyInfo);
    void handleRejected(std::shared_ptr<nim::SignalingNotifyInfo> notifyInfo);
    void handleOtherClientRejected(std::shared_ptr<nim::SignalingNotifyInfo> notifyInfo);
    void handleJoin(std::shared_ptr<nim::SignalingNotifyInfo> notifyInfo);
    void handleLeave(std::shared_ptr<nim::SignalingNotifyInfo> notifyInfo);
    void handleClose(std::shared_ptr<nim::SignalingNotifyInfo> notifyInfo);
    void handleCancelInvite(std::shared_ptr<nim::SignalingNotifyInfo> notifyInfo);
    void signalingNotifyCb(std::shared_ptr<nim::SignalingNotifyInfo> notifyInfo);
    void signalingMutilClientSyncCb(std::shared_ptr<nim::SignalingNotifyInfo> notifyInfo);
    void signalingOfflineNotifyCb(std::list<std::shared_ptr<nim::SignalingNotifyInfo>> notifyInfo);

    // G2事件回调
    virtual void onJoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed) override;
    virtual void onUserJoined(nertc::uid_t uid, const char* user_name) override;
    virtual void onUserLeft(nertc::uid_t uid, nertc::NERtcSessionLeaveReason reason) override;
    virtual void onUserAudioStart(nertc::uid_t uid) override;
    virtual void onUserAudioStop(nertc::uid_t uid) override;
    virtual void onUserVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile) override;
    virtual void onUserVideoStop(nertc::uid_t uid) override;
    virtual void onDisconnect(nertc::NERtcErrorCode reason) override;
    virtual void onLocalAudioVolumeIndication(int volume) override;
    virtual void onRemoteAudioVolumeIndication(const nertc::NERtcAudioVolumeInfo *speakers, unsigned int speaker_number, int total_volume) override;
    virtual void onError(int error_code, const char* msg) override;

    // G2 MediaStatsObserver回调
    //该回调描述每个用户在通话中的网络状态，每 2 秒触发一次，只上报状态有变更的成员。
    virtual void onNetworkQuality(const nertc::NERtcNetworkQualityInfo* infos, unsigned int user_count) override;

private:
    void startDialWaitingTimer();
    void closeChannelInternal(const std::string& channelId, AvChatComponentOptCb cb);
    void updateChannelMembers(const nim::SignalingJoinResParam* res);
    void handleNetCallMsg(necall_kit::NIMNetCallStatus why);
    void regSignalingCb(bool reg = true);

    GetTokenServiceFunc getTokenService_;
    std::string getAccid(int64_t uid);
    std::string appKey_;
    nertc::IRtcEngineEx* rtcEngine_ = nullptr;
    // std::string currentChannelId;
    std::weak_ptr<IAvChatComponentEventHandler> compEventHandler_;
    AvChatComponentOptCb optCb_;
    std::string senderAccid;
    std::string toAccid;
    std::map<std::string, int64_t> channelMembers_;
    nim::SignalingNotifyInfoInvite invitedInfo_;
    nim::SignalingInviteParam invitingInfo_;
    nim::SignalingCreateResParam createdChannelInfo_;
    necall_kit::Timer* calling_timeout_timer_ = nullptr;
    ComponentStatus status_ = idle;
    std::string joined_channel_id_;
    int64_t to_account_id_ = 0;
    std::string from_account_id_;
    std::string stoken_;
    std::string version_;     /**< 对方版本 */
    std::string channelName_; /**< 房间频道名称 */
    std::string attachment_;  /**< 附件信息 */
    int callType = kAvChatAudio;
    bool isCameraOpen;
    bool timeOutHurryUp;
    bool isMasterInvited; /**< 主叫方标记 */
    bool isUseRtcSafeMode;
    std::atomic_bool m_bRegSignalingCb{ false };
};
}  // namespace necall_kit

#endif  // !AVCHAT_COMPONENT_H_