/**
 * @file "stable.h"
 * @brief 呼叫组件的一些定义
 * @copyright (c) 2014-2021, NetEase Inc. All rights reserved
 * @date 2020/11/03
 */

#ifndef STABLE_H_
#define STABLE_H_

#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <atomic>
#include <future>

/**< nertc日志目录 */
#define AVCHAT_LOG_DIR L"Netease\\CallKit\\NeRtc"

#include "third_party/alog/include/alog.h"
#define YXLOGEnd ALOGEnd
#define YXLOG(level) ALOG_DIY("callkit", LogNormal, level)
#define YXLOG_API(level) ALOG_DIY("callkit", LogApi, level)

#endif