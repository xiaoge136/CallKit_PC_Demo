/**
 * @file net_call_helper.h
 * @brief 话单头文件
 * @copyright (c) 2014-2021, NetEase Inc. All rights reserved
 * @author Martin
 * @date 2021/05/28
 */

#ifndef NET_CALL_HELPER_H_
#define NET_CALL_HELPER_H_

#include <string>
#include <vector>

#include "src/cpp_sdk/nim/helper/nim_talk_helper.h"

/*
 *G2 话单解析模块
 *
 */
namespace necall_kit
{
    enum NIMNetCallStatus
    {
        kNIMNetCallStatusComplete = 1,    /**< 1:通话完成 */
        kNIMNetCallStatusCanceled,        /**< 2:通话取消 */
        kNIMNetCallStatusRejected,        /**< 3:通话拒绝 */
        kNIMNetCallStatusTimeout,         /**< 4:超时未接听 */
        kNIMNetCallStatusBusy,            /**< 5:对方忙线 */
    };

    enum NIMNetCallType
    {
        kNIMNetCallTypeAudio = 1,
        kNIMNetCallTypeVideo = 2,
    };

    std::wstring ParseNetCallMsg(const nim::SessionData& msg);
    std::wstring ParseNetCallMsg(const nim::IMMessage& msg);

    void SendNetCallMsg(const std::string& to, const std::string& channelId, int type, int status, std::vector<std::string> members, std::vector<int> durations);
}

#endif  // !NET_CALL_HELPER_H_
