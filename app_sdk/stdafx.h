// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ

//c++ header
#include <ctime>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <map>
#include <memory>
#include <functional>
//base header
#include "base/callback/callback.h"
#include "base/util/string_util.h"
#include "base/util/unicode.h"
#include "base/util/string_number_conversions.h"
#include "base/memory/deleter.h"
#include "base/memory/singleton.h"
#include "base/win32/platform_string_util.h"
#include "base/win32/path_util.h"
#include "base/win32/win_util.h"
#include "base/thread/thread_manager.h"
#include "base/macros.h"
#include "base/base_types.h"
#include "base/file/file_util.h"
#include "base/time/time.h"
#include "base/framework/task.h"
#include "base/memory/singleton.h"

//shared project
#include "shared/utf8_file_util.h"
#include "shared/tool.h"

//json
#include "jsoncpp/include/json/json.h"

//xml
#include "tinyxml/tinyxml.h"

//window header
#include <atlbase.h>
#include <WTypes.h>
#include <shellapi.h>

//log
#include "shared/log.h"

//util
#include "shared/util.h"

//sdk
#include "nim_cpp_wrapper/nim_cpp_api.h"
#include "nim_chatroom_cpp_wrapper/nim_cpp_chatroom_api.h"
#include "nim_tools_cpp_wrapper/nim_tools_http_cpp.h"

//helper
#include "shared/closure.h"


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
