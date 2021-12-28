#include "net_call_helper.h"

const static char* kNIMNetCallType = "type";
const static char* kNIMNetCallStatus = "status";
const static char* kNIMNetCallChannelId = "channelId";
const static char* kNIMNetCallDurations = "durations";
const static char* kNIMNetCallDuration = "duration";
const static char* kNIMNetCallAccid = "accid";

namespace necall_kit {

void OnSendNetCallMsgCb(const nim::SendMessageArc& arc) {
    if (arc.rescode_ != 200) {
        YXLOG(Error) << "OnSendNetCallMsgCb error: " << arc.rescode_ << YXLOGEnd;
        return;
    }

    nim::MsgLog::QueryMsgByIDAysnc(arc.msg_id_, [arc](nim::NIMResCode res_code, const std::string& msg_id, const nim::IMMessage& msg) {
        if (res_code != 200) {
            YXLOG(Error) << "QueryMsgByIDAysnc error: " << res_code << YXLOGEnd;
            return;
        }
    });
}
void SendNetCallMsg(const std::string& to, const std::string& channelId, int type, int status, std::vector<std::string> members,
                    std::vector<int> durations) {
    nim_cpp_wrapper_util::Json::Value values;
    nim_cpp_wrapper_util::Json::FastWriter writer;
    values[kNIMNetCallType] = type;
    values[kNIMNetCallStatus] = status;
    values[kNIMNetCallChannelId] = channelId;
    assert(members.size() == durations.size());

    for (int i = 0; i < members.size(); i++) {
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
}  // namespace necall_kit
