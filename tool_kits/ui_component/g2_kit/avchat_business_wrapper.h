/**
 * @file avchat_business_wrapper.h
 * @brief 呼叫组件（AvChatComponent）针对BusinessGateway的封装，负责将调用参数、通知参数打包成nbase::BatpPack
 * @copyright (c) 2014-2021, NetEase Inc. All rights reserved
 * @author Martin
 * @date 2021/05/28
 */

#ifndef AVCHAT_BUSINESS_WRAPPER_H_
#define AVCHAT_BUSINESS_WRAPPER_H_

#include "shared/business_action_gateway/batp/batp.h"
#include "call_kit/include/necallkit_interface.h"
#include "shared/business_action_gateway/business_manager/business_manager.h"

namespace nim_comp
{
    using namespace necall_kit;

    static const char* kAvChatRelease = "release";
    static const char* kAvChatSetupAppKey = "setupAppKey";
    static const char* kAvChatLogin = "login";
    static const char* kAvChatLogout = "logout";
    static const char* kAvChatCall = "call";
    static const char* kAvChatAccept = "accept";
    static const char* kAvChatReject = "reject";
    static const char* kAvChatHangup = "hangup";
    static const char* kAvChatCancel = "cancel";
    static const char* kAvChatLeave = "leave";

    static const char* kAvChatSetupLocalView = "setupLocalView";
    static const char* kAvChatSetupRemoteView = "setupRemoteView";
    static const char* kAvChatEnableLocalVideo = "enableLocalVideo";
    static const char* kAvChatMuteLocalAudio = "muteLocalAudio";
    static const char* kAvChatEnableAudioPlayout = "enableAudioPlayout";
    static const char* kAvChatSwitchCamera = "switchCamera";
    static const char* kAvChatSetRecordVolume = "setRecordDeviceVolume";
    static const char* kAvChatSetPlayoutVolume = "setPlayoutDeviceVolume";
    static const char* kAvChatSetAudioDevice = "setAudioDevice";
    static const char* kAvChatSetVideoDevice = "setVideoDevice";
    static const char* kAvChatStartVideoPreview = "startVideoPreview";
    static const char* kAvChatSwitchCallType = "switchCallType";
    static const char* kAvChatSetAudioMute = "setAudioMute";
    static const char* kAvChatGetTockenService = "getTockenService";
    static const char* kAvChatStartAudioDeviceLoopbackTest = "startAudioDeviceLoopbackTest";
    static const char* kAvChatStopAudioDeviceLoopbackTest = "stopAudioDeviceLoopbackTest";
    static const char* kAvChatSetVideoQuality = "setVideoQuality";

    static const char* kAvChatOnCalling = "onCalling";
    static const char* kAvChatOnInvite = "onInvite";
    static const char* kAvChatOnUserAccept = "onUserAccept";
    static const char* kAvChatOnUserReject = "onUserReject";
    static const char* kAvChatOnUserEnter = "onUserEnter";
    static const char* kAvChatOnUserLeave = "onUserLeave";
    static const char* kAvChatOnUserDisconnect = "onUserDisconnect";
    static const char* kAvChatOnUserBusy = "onUserBusy";
    static const char* kAvChatOnUserCancel = "onUserCancel";
    static const char* kAvChatOnCallingTimeOut = "onCallingTimeOut";
    static const char* kAvChatOnDisconnect = "onDisconnect";
    static const char* kAvChatOnVideoToAudio = "onVideoToAudio";
    static const char* kAvChatOnCallEnd = "onCallEnd";
    static const char* kAvChatOnError = "onError";

    static const char* kAvChatOnOtherClientAccept = "onOtherClientAccept";
    static const char* kAvChatOnOtherClientReject = "onOtherClientReject";
    static const char* kAvChatOnUserNetworkQuality = "onUserNetworkQuality";

    static const char* kAvChatOnCameraAvailable = "onCameraAvailable";
    static const char* kAvChatOnAudioAvailable = "onAudioAvailable";
    static const char* kAvChatOnLocalCameraStateChanged = "onLocalCameraStateChanged";
    static const char* kAvChatOnAudioVolumChanged = "onAudioVolumeChanged";
    static const char* kAvChatOnJoinChannel = "onJoinChannel";

    /**
     * @brief wrapper参数
     */
    struct AvChatParams
    {
        std::string     appKey;
        std::string     userId;//即为accid
        AVCHAT_CALL_TYPE     callType;
        AvChatComponentOptCb        optCb;
        nertc::onFrameDataCallback  dataCb;
        std::vector<std::string>    members;
        std::string     invitorAccid;
        bool    isFromGroup;
        bool    cameraAvailable;
        bool    audioAvailable;
        bool    muteAudio;
        int     errCode;
        void*   userData;
        void*   window;
        int     volume;
        std::wstring    deviceId;
        bool    isRecordDevice;
        bool    startPreview;
        int     interval;
        int     videoQuality;
        int     reason;
        std::string     sessionId;
        bool            useRtcSafeMode;
        nertc::uid_t    uid;
        nertc::channel_id_t cid;
        std::string         cname;
        GetTokenServiceFunc tockenServiceFunc;
        std::map<uint64_t, nertc::NERtcNetworkQualityType>  network_quality;
        AvChatParams() :userData(nullptr), window(nullptr), errCode(0)
        {}
    };

    /**
     * @brief 将组件回调传来的参数，组装成 nbase::BatpPack 通知出去
     */
    class AvChatComponentEventHandler : public IAvChatComponentEventHandler
    {
    public:
        virtual void onInvited(const std::string& invitor, std::vector<std::string> userIDs, bool isFromGroup, const std::string& groupID, AVCHAT_CALL_TYPE type, const std::string& attachment) override;
        virtual void onUserAccept(const std::string& userId) override;
        virtual void onUserReject(const std::string& userId) override;
        virtual void onUserEnter(const std::string& userId) override;
        virtual void onUserLeave(const std::string& userId) override;
        virtual void onUserDisconnect(const std::string& userId) override;
        virtual void onUserBusy(const std::string& userId) override;
        virtual void onUserCancel(const std::string& userId) override;

        virtual void onCallingTimeOut() override;
        virtual void onDisconnect(int reason) override;
        virtual void OnVideoToAudio() override;
        virtual void onCallEnd() override;
        virtual void onError(int errCode) override;

        virtual void onOtherClientAccept() override;
        virtual void onOtherClientReject() override;
        virtual void onUserNetworkQuality(std::map<uint64_t, nertc::NERtcNetworkQualityType>) override;

        virtual void onCameraAvailable(const std::string& userId, bool available) override;
        virtual void onAudioAvailable(const std::string& userId, bool available) override;
        virtual void onLocalCameraStateChanged(bool available) override;
        virtual void onAudioVolumeChanged(unsigned char value, bool isRecord) override;
        virtual void onJoinChannel(const std::string& accid, nertc::uid_t uid, nertc::channel_id_t cid, const std::string& cname) override;
    };

    /**
     * @brief 组件实现类的Wrapper
     * @see AvChatComponent
     */
    class AvChatBusinessWrapper
    {
    public:
        static void release(const nbase::BatpPack& request);
        static void setupAppKey(const nbase::BatpPack& request);
        static void call(const nbase::BatpPack& request);
        static void login(const nbase::BatpPack& request);
        static void logout(const nbase::BatpPack& request);
        static void accept(const nbase::BatpPack& request);
        static void reject(const nbase::BatpPack& request);
        static void hangup(const nbase::BatpPack& request);
        static void cancel(const nbase::BatpPack& request);
        static void leave(const nbase::BatpPack& request);

        static void setupLocalView(const nbase::BatpPack& request);
        static void setupRemoteView(const nbase::BatpPack& request);
        static void switchCamera(const nbase::BatpPack& request);
        static void enableLocalVideo(const nbase::BatpPack& request);
        static void muteLocalAudio(const nbase::BatpPack& request);
        static void enableAudioPlayout(const nbase::BatpPack& request);
        static void setRecordDeviceVolume(const nbase::BatpPack& request);
        static void setPlayoutDeviceVolume(const nbase::BatpPack& request);
        static void setVideoDevice(const nbase::BatpPack& request);
        static void setAudioDevice(const nbase::BatpPack& request);
        static void startVideoPreview(const nbase::BatpPack& request);
        static void switchCallType(const nbase::BatpPack& request);
        static void setAudioMute(const nbase::BatpPack& request);
        static void startAudioDeviceLoopbackTest(const nbase::BatpPack& request);
        static void stopAudioDeviceLoopbackTest(const nbase::BatpPack& request);
        static void setVideoQuality(const nbase::BatpPack& request);
        static void getTokenService(const nbase::BatpPack& request);


        static std::string getChannelId();
        static void getLocalDeviceList(
            std::vector<std::wstring>* recordDevicesNames,
            std::vector<std::wstring>* recordDevicesIds,
            std::vector<std::wstring>* playoutDevicesNames,
            std::vector<std::wstring>* playoutDevicesIds,
            std::vector<std::wstring>* videoDeviceNames,
            std::vector<std::wstring>* videoDeviceIds);
        static unsigned char getAudioVolumn(bool isRecord);
        static std::wstring getVideoDevice();
        static std::wstring getAudioDevice(bool isRecord);
//         static bool isAudioEnable(bool isRecord);
//         static bool isVideoEnable();

    private:
        static std::shared_ptr<AvChatComponentEventHandler> eventHandler_;
    };
}

#endif  // !AVCHAT_BUSINESS_WRAPPER_H_