#pragma once
#include "pch.h"
#include "avchat_business_wrapper.h"
#include "shared/business_action_gateway/business_manager/business_manager.h"

namespace nim_comp
{
    std::shared_ptr<AvChatComponentEventHandler> AvChatBusinessWrapper::eventHandler_;
    std::shared_ptr<AvChatComponentNERtcEventHandler> AvChatBusinessWrapper::nertcEventHandler_;
    void AvChatComponentEventHandler::onInvited(const std::string& invitor, std::vector<std::string> userIDs, bool isFromGroup, const std::string& groupID, AVCHAT_CALL_TYPE type, const std::string& attachment)
    {
        QLOG_APP(L"AvChatComponentEventHandler onInvited, invitor: {0}") << invitor;

        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnInvite;
        AvChatParams params;

        params.invitorAccid = invitor;
        params.members = userIDs;
        params.isFromGroup = isFromGroup;
        params.callType = type;

        bp.body_.param_ = params;
        nbase::BusinessManager::GetInstance()->Notify(bp);
    }

    void AvChatComponentEventHandler::onUserAccept(const std::string& userId)
    {
        QLOG_APP(L"AvChatComponentEventHandler onUserAccept, userId: {0}") << userId;

        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnUserAccept;
        AvChatParams params;

        params.userId = userId;
        bp.body_.param_ = params;
        nbase::BusinessManager::GetInstance()->Notify(bp);
    }
    void AvChatComponentEventHandler::onUserReject(const std::string& userId) 
    {
        QLOG_APP(L"AvChatComponentEventHandler onUserReject, userId: {0}") << userId;

        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnUserReject;
        AvChatParams params;

        params.userId = userId;
        bp.body_.param_ = params;
        nbase::BusinessManager::GetInstance()->Notify(bp);
    }
    void AvChatComponentEventHandler::onUserEnter(const std::string& userId)
    {
        QLOG_APP(L"AvChatComponentEventHandler onUserEnter, userId: {0}") << userId;

        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnUserEnter;
        AvChatParams params;

        params.userId = userId;
        bp.body_.param_ = params;
        nbase::BusinessManager::GetInstance()->Notify(bp);
    }
    void AvChatComponentEventHandler::onUserLeave(const std::string& userId)
    {
        QLOG_APP(L"AvChatComponentEventHandler onUserLeave, userId: {0}") << userId;

        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnUserLeave;
        AvChatParams params;

        params.userId = userId;
        bp.body_.param_ = params;
        nbase::BusinessManager::GetInstance()->Notify(bp);
    }
    void AvChatComponentEventHandler::onUserDisconnect(const std::string& userId)
    {
        QLOG_APP(L"AvChatComponentEventHandler onUserDisconnect, userId: {0}") << userId;

        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnUserDisconnect;
        AvChatParams params;

        params.userId = userId;
        bp.body_.param_ = params;
        nbase::BusinessManager::GetInstance()->Notify(bp);
    }
    void AvChatComponentEventHandler::onUserBusy(const std::string& userId)
    {
        QLOG_APP(L"AvChatComponentEventHandler onUserBusy, userId: {0}") << userId;

        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnUserBusy;
        AvChatParams params;

        params.userId = userId;
        bp.body_.param_ = params;
        nbase::BusinessManager::GetInstance()->Notify(bp);
    }
    void AvChatComponentEventHandler::onUserCancel(const std::string& userId)
    {
        QLOG_APP(L"AvChatComponentEventHandler onUserCanale, userId: {0}") << userId;

        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnUserCancel;
        AvChatParams params;

        params.userId = userId;
        bp.body_.param_ = params;
        nbase::BusinessManager::GetInstance()->Notify(bp);
    }

    void AvChatComponentEventHandler::onCallingTimeOut()
    {
        QLOG_APP(L"AvChatComponentEventHandler onCallingTimeOut");

        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnCallingTimeOut;
        nbase::BusinessManager::GetInstance()->Notify(bp);
    }

    void AvChatComponentEventHandler::onDisconnect(int reason)
    {
        QLOG_APP(L"AvChatComponentEventHandler onDisconnect");

        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnDisconnect;
        AvChatParams params;

        params.reason = reason;
        bp.body_.param_ = params;
        nbase::BusinessManager::GetInstance()->Notify(bp);
    }

    void AvChatComponentEventHandler::OnVideoToAudio()
    {
        QLOG_APP(L"AvChatComponentEventHandler OnVideoToAudio");

        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnVideoToAudio;
        nbase::BusinessManager::GetInstance()->Notify(bp);
    }
    void AvChatComponentEventHandler::onCallEnd()
    {
        QLOG_APP(L"AvChatComponentEventHandler onCallEnd");

        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnCallEnd;

        nbase::BusinessManager::GetInstance()->Notify(bp);
    }
    void AvChatComponentEventHandler::onError(int errCode, const std::string& errMsg)
    {
        QLOG_APP(L"AvChatComponentEventHandler onError: {0}")<<errCode;

        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnError;
        AvChatParams params;
        params.errCode = errCode;
        bp.body_.param_ = params;
        nbase::BusinessManager::GetInstance()->Notify(bp);
    }
    void AvChatComponentEventHandler::onOtherClientAccept()
    {
        QLOG_APP(L"AvChatComponentEventHandler onOtherClientAccept: ");

        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnOtherClientAccept;
        nbase::BusinessManager::GetInstance()->Notify(bp);
    }
    void AvChatComponentEventHandler::onOtherClientReject()
    {
        QLOG_APP(L"AvChatComponentEventHandler onOtherClientReject: ");

        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnOtherClientReject;
        nbase::BusinessManager::GetInstance()->Notify(bp);
    }
    void AvChatComponentEventHandler::onUserNetworkQuality(const std::map<std::string, nertc::NERtcNetworkQualityInfo>& network_quality)
    {
        QLOG_APP(L"AvChatComponentEventHandler onUserNetworkQuality: ");

        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnUserNetworkQuality;
        AvChatParams params;
        params.network_quality = network_quality;
        bp.body_.param_ = params;
        nbase::BusinessManager::GetInstance()->Notify(bp);
    }
    void AvChatComponentEventHandler::onCameraAvailable(const std::string& userId, bool available)
    {
        QLOG_APP(L"AvChatComponentEventHandler onCameraAvailable, userId: {0}, available: {1}") << userId << available;

        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnCameraAvailable;
        AvChatParams params;
        params.userId = userId;
        params.cameraAvailable = available;
        bp.body_.param_ = params;

        nbase::BusinessManager::GetInstance()->Notify(bp);
    }
    void AvChatComponentEventHandler::onAudioAvailable(const std::string& userId, bool available)
    {
        QLOG_APP(L"AvChatComponentEventHandler onAudioAvailable, userId: {0}, available: {1}") << userId << available;

        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnAudioAvailable;
        AvChatParams params;
        params.userId = userId;
        params.audioAvailable = available;
        bp.body_.param_ = params;

        nbase::BusinessManager::GetInstance()->Notify(bp);
    }
    void AvChatComponentEventHandler::onLocalCameraStateChanged(bool available)
    {
        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnLocalCameraStateChanged;
        AvChatParams params;
        params.cameraAvailable = available;
        bp.body_.param_ = params;

        nbase::BusinessManager::GetInstance()->Notify(bp);
    }
    void AvChatComponentEventHandler::onAudioVolumeChanged(unsigned char value, bool isRecord)
    {
        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnAudioVolumChanged;
        AvChatParams params;
        params.volume = value;
        params.isRecordDevice = isRecord;
        bp.body_.param_ = params;

        nbase::BusinessManager::GetInstance()->Notify(bp);
    }
    void AvChatComponentEventHandler::onJoinChannel(const std::string& accid, nertc::uid_t uid, nertc::channel_id_t cid, const std::string& cname, int errCode)
    {
        nbase::BatpPack bp;
        bp.head_.action_name_ = kAvChatOnJoinChannel;
        AvChatParams params;
        params.userId = accid;
        params.uid = uid;
        params.cid = cid;
        params.cname = cname;
        bp.body_.param_ = params;

        nbase::BusinessManager::GetInstance()->Notify(bp);
    }
    void AvChatBusinessWrapper::release(const nbase::BatpPack& request)
    {
        QLOG_APP(L"Release component resource");
        createChatComponent()->release();

        nbase::BusinessManager::GetInstance()->Response(request);
    }
    void AvChatBusinessWrapper::setupAppKey(const nbase::BatpPack& request)
    {
        nertcEventHandler_.reset();
        nertcEventHandler_ = std::make_shared<AvChatComponentNERtcEventHandler>();

        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
        std::string key = params.appKey;
        bool bUseRtcSafeMode = params.useRtcSafeMode;
        createChatComponent()->setupAppKey(key, bUseRtcSafeMode, nertcEventHandler_);

        eventHandler_.reset();
        eventHandler_ = std::make_shared<AvChatComponentEventHandler>();
        createChatComponent()->regEventHandler(eventHandler_);
    }

    //avchat 组件并未定义onCalling事件，为了通知界面更新状态，在AvChatBusinessWrapper::call中发一个onCalling事件
    void AvChatBusinessWrapper::call(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
        std::string userId = params.userId;
        AVCHAT_CALL_TYPE callType = AVCHAT_CALL_TYPE(params.callType);

        nim_cpp_wrapper_util::Json::Value values;
        nim_cpp_wrapper_util::Json::FastWriter writer;
        values["key"] = "call";
        values["value"] = "testvalue";
		createChatComponent()->call(userId, callType, writer.write(values), [params](int errorCode) {
			//成功
			if (errorCode == 10201 || errorCode == 10202 || errorCode == 200) {
				nbase::BatpPack bp;
				bp.head_.action_name_ = kAvChatOnCalling;
				bp.body_.param_ = params;
				nbase::BusinessManager::GetInstance()->Notify(bp);
			}
			else {
				//失败
				QLOG_APP(L"call failed: ", errorCode);
			}

			if (params.optCb) {
				params.optCb(errorCode);
			}
		});
    }

    void AvChatBusinessWrapper::login(const nbase::BatpPack& request)
    {
        assert(false);
    }
    void AvChatBusinessWrapper::logout(const nbase::BatpPack& request)
    {
        assert(false);
    }

    void AvChatBusinessWrapper::accept(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
		createChatComponent()->accept(params.optCb);
    }
    void AvChatBusinessWrapper::reject(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
		createChatComponent()->reject(params.optCb);
    }
    void AvChatBusinessWrapper::hangup(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
		createChatComponent()->hangup(params.optCb);
    }
    void AvChatBusinessWrapper::switchCallType(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
		createChatComponent()->switchCallType(params.sessionId, (AVCHAT_CALL_TYPE)params.callType, params.optCb);
    }
    void AvChatBusinessWrapper::setAudioMute(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
		createChatComponent()->setAudioMute(params.sessionId, params.muteAudio);
    }
    void AvChatBusinessWrapper::cancel(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
		createChatComponent()->cancel(params.optCb);
    }
    void AvChatBusinessWrapper::leave(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
		createChatComponent()->leave(params.optCb);
    }
    void AvChatBusinessWrapper::setupLocalView(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
        nertc::NERtcVideoCanvas canvas;
        canvas.window = params.window;
        canvas.cb = params.dataCb;
        canvas.user_data = nullptr;
        canvas.scaling_mode = nertc::kNERtcVideoScaleFit;
		createChatComponent()->setupLocalView(&canvas);
        nbase::BatpPackResponse response(request.head_, BatpTrailInfoFromHere);
        nbase::BusinessManager::GetInstance()->Response(response);
    }

    void AvChatBusinessWrapper::setupRemoteView(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
        nertc::NERtcVideoCanvas canvas;
        canvas.window = params.window;
        canvas.cb = params.dataCb;
        canvas.user_data = nullptr;
        canvas.scaling_mode = nertc::kNERtcVideoScaleFit;
		createChatComponent()->setupRemoteView(&canvas, params.userId);
    }
    void AvChatBusinessWrapper::switchCamera(const nbase::BatpPack& request)
    {
		createChatComponent()->switchCamera();
    }
    void AvChatBusinessWrapper::getLocalDeviceList(
        std::vector<std::wstring>* recordDevicesNames,
        std::vector<std::wstring>* recordDevicesIds,
        std::vector<std::wstring>* playoutDevicesNames,
        std::vector<std::wstring>* playoutDevicesIds,
        std::vector<std::wstring>* videoDeviceNames,
        std::vector<std::wstring>* videoDeviceIds)
    {
        std::vector<std::string> recordDevicesNamesTmp;
        std::vector<std::string> recordDevicesIdsTmp;
        std::vector<std::string> playoutDevicesNamesTmp;
        std::vector<std::string> playoutDevicesIdsTmp;
        std::vector<std::string> videoDeviceNamesTmp;
        std::vector<std::string> videoDeviceIdsTmp;

		createChatComponent()->getLocalDeviceList(
            recordDevicesNames ? &recordDevicesNamesTmp : nullptr, recordDevicesIds ? &recordDevicesIdsTmp : nullptr,
            playoutDevicesNames ? &playoutDevicesNamesTmp : nullptr, playoutDevicesIds ? &playoutDevicesIdsTmp : nullptr,
            videoDeviceNames ? &videoDeviceNamesTmp : nullptr, videoDeviceIds ? &videoDeviceIdsTmp : nullptr);

        if (recordDevicesNames) {
            for (auto& it : recordDevicesNamesTmp) {
                recordDevicesNames->push_back(nbase::UTF8ToUTF16(it));
            }
        }

        if (recordDevicesIds) {
            for (auto& it : recordDevicesIdsTmp) {
                recordDevicesIds->push_back(nbase::UTF8ToUTF16(it));
            }
        }

        if (playoutDevicesNames) {
            for (auto& it : playoutDevicesNamesTmp) {
                playoutDevicesNames->push_back(nbase::UTF8ToUTF16(it));
            }
        }

        if (playoutDevicesIds) {
            for (auto& it : playoutDevicesIdsTmp) {
                playoutDevicesIds->push_back(nbase::UTF8ToUTF16(it));
            }
        }

        if (videoDeviceNames) {
            for (auto& it : videoDeviceNamesTmp) {
                videoDeviceNames->push_back(nbase::UTF8ToUTF16(it));
            }
        }

        if (videoDeviceIds) {
            for (auto& it : videoDeviceIdsTmp) {
                videoDeviceIds->push_back(nbase::UTF8ToUTF16(it));
            }
        }
    }
    unsigned char AvChatBusinessWrapper::getAudioVolumn(bool isRecord)
    {
        auto value = createChatComponent()->getAudioVolumn(isRecord);
        return value <= 255 ? value : value % 255;
    }
    std::wstring AvChatBusinessWrapper::getVideoDevice()
    {
        return nbase::UTF8ToUTF16(createChatComponent()->getVideoDevice());
    }
    std::wstring AvChatBusinessWrapper::getAudioDevice(bool isRecord)
    {
        return nbase::UTF8ToUTF16(createChatComponent()->getAudioDevice(isRecord));
    }
//     bool AvChatBusinessWrapper::isAudioEnable(bool isRecord)
//     {
//         return component.isAudioEnable(isRecord);
//     }
//     bool AvChatBusinessWrapper::isVideoEnable()
//     {
//         return component.isVideoEnable();
//     }
    void AvChatBusinessWrapper::setRecordDeviceVolume(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
		createChatComponent()->setRecordDeviceVolume(params.volume);
    }
    void AvChatBusinessWrapper::setPlayoutDeviceVolume(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
		createChatComponent()->setPlayoutDeviceVolume(params.volume);
    }

    void AvChatBusinessWrapper::setVideoDevice(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
		createChatComponent()->setVideoDevice(nbase::UTF16ToUTF8(params.deviceId));
    }
    void AvChatBusinessWrapper::setAudioDevice(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
		createChatComponent()->setAudioDevice(nbase::UTF16ToUTF8(params.deviceId), params.isRecordDevice);
    }
    void AvChatBusinessWrapper::enableLocalVideo(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
		createChatComponent()->enableLocalVideo(params.cameraAvailable);
    }
    void AvChatBusinessWrapper::muteLocalAudio(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
		createChatComponent()->muteLocalAudio(params.muteAudio);
    }
    void AvChatBusinessWrapper::enableAudioPlayout(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
		createChatComponent()->enableAudioPlayout(params.audioAvailable);
    }
    void AvChatBusinessWrapper::startVideoPreview(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
		createChatComponent()->startVideoPreview(params.startPreview);
    }

    void AvChatBusinessWrapper::startAudioDeviceLoopbackTest(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
		createChatComponent()->startAudioDeviceLoopbackTest(params.interval);
    }
    void AvChatBusinessWrapper::stopAudioDeviceLoopbackTest(const nbase::BatpPack& request)
    {
		createChatComponent()->stopAudioDeviceLoopbackTest();
    }
    void AvChatBusinessWrapper::setVideoQuality(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
		createChatComponent()->setVideoQuality((nertc::NERtcVideoProfileType)params.videoQuality);
    }
    void AvChatBusinessWrapper::getTokenService(const nbase::BatpPack& request)
    {
        AvChatParams params = nbase::BatpParamCast<AvChatParams>(request.body_.param_);
		createChatComponent()->setTokenService((GetTokenServiceFunc)params.tockenServiceFunc);
    }
    void AvChatBusinessWrapper::startScreenSharing() {
        QLOG_APP(L"AvChatBusinessWrapper startScreenSharing");
        auto rtc = createChatComponent()->getRtcEngine();
        if (rtc) {
            nertc::NERtcVideoCanvas canvas;
            canvas.window = nullptr;
            canvas.cb = nullptr;
            canvas.user_data = nullptr;
            canvas.scaling_mode = nertc::kNERtcVideoScaleFit;
            int ret = rtc->setupLocalSubStreamVideoCanvas(&canvas);
            if (0 != ret) {
                QLOG_APP(L"setupLocalSubStreamVideoCanvas, ret: {0}", ret);
            }

            nertc::NERtcRectangle source = {};
            source.x = 0;
            source.y = 0;
            source.width = 1920;
            source.height = 1080;

            nertc::NERtcRectangle region = {};

            nertc::NERtcScreenCaptureParameters params = {};
            params.bitrate = 0;
            params.frame_rate = 25;
            params.profile = nertc::kNERtcScreenProfileCustom;
            params.capture_mouse_cursor = true;
            params.dimensions.width = source.width;
            params.dimensions.height = source.height;
            params.excluded_window_list = nullptr;
            params.excluded_window_count = 0;
            params.prefer = nertc::kNERtcSubStreamContentPreferMotion;
            ret = rtc->startScreenCaptureByScreenRect(source, region, params);
            if (0 != ret) {
                QLOG_APP(L"startScreenCaptureByScreenRect, ret: {0}", ret);
            }
        }
    }

    void AvChatBusinessWrapper::stopScreenSharing() {
        QLOG_APP(L"AvChatBusinessWrapper stopScreenSharing");
        auto rtc = createChatComponent()->getRtcEngine();
        if (rtc) {
            rtc->stopScreenCapture();
        }
    }

    std::string AvChatBusinessWrapper::getChannelId()
    {
        auto info = createChatComponent()->getCreatedChannelInfo();
        return info.channel_info_.channel_id_;
    }
}