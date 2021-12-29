#pragma once

#include "nim_chatroom_cpp_wrapper/nim_cpp_chatroom_api.h"

namespace app_sdk
{
	//��ʽ���˺�����ص�
	using OnFormatAccountAndPasswordCallback = std::function<void(bool ret,/*�Ƿ���е�¼*/const std::string& account, const std::string& password)>;
	//ע�����˺Żص�
	using OnRegisterAccountCallback = std::function<void(int code, const std::string& err_msg)>;
	//��ȡ�������б�ص�
	using OnGetChatroomListCallback = std::function<void(int code, const std::vector<nim_chatroom::ChatRoomInfo>& chatroom_list)>;
	//��ȡ�����ҵ�ַ�ص�
	using OnGetChatroomAddressCallback = std::function<void(int code, const std::list<std::string>& address_list)>;
}
