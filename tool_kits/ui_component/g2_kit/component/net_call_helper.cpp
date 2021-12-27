#include "net_call_helper.h"
#include "avchat_component_def.h"

#include <atomic>

#include "nim_sdk/include/depend_lib/include/json/json.h"
#include "nim_sdk/include/nim_cpp_tools_api.h"
#include "nim_sdk/include/nim_cpp_api.h"

const static char* kNIMNetCallType = "type";
const static char* kNIMNetCallStatus = "status";
const static char* kNIMNetCallChannelId = "channelId";
const static char* kNIMNetCallDurations = "durations";
const static char* kNIMNetCallDuration = "duration";
const static char* kNIMNetCallAccid = "accid";

/*
G2话单为新增消息类型，话单信息位于msg.attach_,json格式，样例数据:
{
    "type":2,                            //通话类型，1::音频，2:视频
    "channelId":52601694881751,            
    "status":1,
    "durations":[
        {
            "duration":7,                //通话时长
            "accid":"cqu227hk_1001"        //参与的用户accid
        },
        {
            "duration":7,
            "accid":"cqu227hk_1000"
        }
    ]
}
*/
namespace necall_kit
{
    const std::vector<std::wstring> StatusTipMsgs = {
        L"",
        L"STRID_SESSION_NETCALL_MSG_COMPLETE",
        L"STRID_SESSION_NETCALL_MSG_CANCELED",
        L"STRID_SESSION_NETCALL_MSG_REJECTED",
        L"STRID_SESSION_NETCALL_MSG_TIMEOUT",
        L"STRID_SESSION_NETCALL_MSG_BUSY"
    };

    static std::atomic_bool hasRegMendMsgCb = false;


    void OnSendNetCallMsgCb(const nim::SendMessageArc& arc)
    {
        if (arc.rescode_ != 200)
        {
            YXLOG(Error) << "OnSendNetCallMsgCb error: " << arc.rescode_ << YXLOGEnd;
            return;
        }

        nim::MsgLog::QueryMsgByIDAysnc(arc.msg_id_, [arc](nim::NIMResCode res_code, const std::string& msg_id, const nim::IMMessage& msg) {
            if (res_code != 200)
            {
                YXLOG(Error) << "QueryMsgByIDAysnc error: " << res_code << YXLOGEnd;
                return;
            }
        });
    }
    void SendNetCallMsg(
        const std::string& to,
        const std::string& channelId,
        int type,
        int status,
        std::vector<std::string> members,
        std::vector<int> durations)
    {
        /*if (!hasRegMendMsgCb)
        {
            hasRegMendMsgCb = true;
            nim::Talk::RegSendMsgCb(OnSendNetCallMsgCb);
        }*/

        nim_cpp_wrapper_util::Json::Value values;
        nim_cpp_wrapper_util::Json::FastWriter writer;
        values[kNIMNetCallType] = type;
        values[kNIMNetCallStatus] = status;
        values[kNIMNetCallChannelId] = channelId;
        assert(members.size() == durations.size());

        for (int i = 0; i < members.size(); i++)
        {
            nim_cpp_wrapper_util::Json::Value info;
            info[kNIMNetCallAccid] = members[i];
            info[kNIMNetCallDuration] = durations[i];
            values[kNIMNetCallDurations].append(info);
        }

        auto attach_info = writer.write(values);
        std::string client_msg_id = nim::Tool::GetUuid();
        nim::MessageSetting setting;

        auto json_msg = nim::Talk::CreateG2NetCallMessage(to, nim::kNIMSessionTypeP2P, client_msg_id, attach_info, setting);
        nim::Talk::SendMsg(json_msg);
    }
}

