#include "chatroom_form.h"
#include "chatroom_frontpage.h"
#include "shared/cpp_wrapper_util.h"
#include "module/session/session_util.h"
#include "gui/login/login_form.h"
#include "module/service/user_service.h"

#define ROOMMSG_R_N _T("\r\n")
namespace nim_chatroom
{

using namespace ui;

void ChatroomForm::RequestEnter(const __int64 room_id)
{
	if (room_id == 0)
	{
		OnRequestRoomError();
		return;
	}

	room_id_ = room_id;
	//��ȡ�����ҵ�¼��Ϣ
	nim::PluginIn::ChatRoomRequestEnterAsync(room_id_, nbase::Bind(&ChatroomForm::OnChatRoomRequestEnterCallback, this, std::placeholders::_1, std::placeholders::_2));
}

void ChatroomForm::SetAnonymity(bool anonymity)
{
	is_anonymity_ = anonymity;
	if (anonymity)
	{
		((Button *)FindControl(L"send"))->SetTextId(L"STRID_LOGIN_FORM_LOGIN");
		((RichEdit *)FindControl(L"input_edit"))->SetPromptTextId(L"STRID_CHATROOM_LOGIN_CHAT");
		((RichEdit *)FindControl(L"input_edit"))->SetEnabled(false);
		btn_face_->SetVisible(false);
		FindControl(L"btn_jsb")->SetVisible(false);
		FindControl(L"logout")->SetVisible(false);
		auto form = (ChatroomFrontpage*)nim_comp::WindowsManager::GetInstance()->GetWindow(ChatroomFrontpage::kClassName, ChatroomFrontpage::kClassName);
		form->SetAnonymity(true);
	}
	else
	{
		((Button *)FindControl(L"send"))->SetTextId(L"STRING_SEND");
		((RichEdit *)FindControl(L"input_edit"))->SetPromptTextId(L"STRID_CHATROOM_ENJOY_CHAT");
		((RichEdit *)FindControl(L"input_edit"))->SetEnabled(true);
		btn_face_->SetVisible(true);
		FindControl(L"btn_jsb")->SetVisible(true);
		auto form = (ChatroomFrontpage*)nim_comp::WindowsManager::GetInstance()->GetWindow(ChatroomFrontpage::kClassName, ChatroomFrontpage::kClassName);
		form->SetAnonymity(false);
		if (nim_comp::LoginManager::GetInstance()->IsAnonymityDemoMode())
		{
			auto logout_btn = FindControl(L"logout");
			logout_btn->SetEnabled(true);
			logout_btn->SetVisible(true);
		}
			
	}
	switch_to_login_status_ = kNone;
}

void ChatroomForm::OnBtnLogin()
{
	auto form = nim_comp::WindowsManager::SingletonShow<LoginForm>(LoginForm::kClassName);
	form->SetLoginFunctionFlag();	
	form->SetAnonymousChatroomVisible(false);
	form->SwitchToLoginPage();
	form->CenterWindow();
	form->ShowWindow();
	nim_ui::OnShowMainWindow cb_login_ok = [this]{
		switch_to_login_status_ = kToLogined;
		nim_chatroom::ChatRoom::Exit(room_id_);
	};

	nim_ui::LoginManager::GetInstance()->RegLoginOKCallback(ToWeakCallback(cb_login_ok));
}

void ChatroomForm::Close(UINT nRet/* = IDOK*/)
{
	if (nRet == kForceClose)
	{
		switch_to_login_status_ = kNone;
		__super::Close();
		return;
	}
	if (switch_to_login_status_ == kToLogined)
	{
		RequestEnter(room_id_);
	}
	else if (switch_to_login_status_ == kToAnonymous)
	{
		AnonymousLogin(room_id_);
	}
	else
		__super::Close(nRet);
}

void ChatroomForm::AnonymousLogin(const __int64 room_id)
{
	if (room_id == 0)
	{
		OnRequestRoomError();
		return;
	}
	room_id_ = room_id;
	switch_to_login_status_ = kToAnonymous;

	app_sdk::AppSDKInterface::GetInstance()->InvokeGetChatroomAddress(room_id_, "", switch_to_login_status_, ToWeakCallback([this](int code, const std::list< std::string>& address_list){
		if (code == nim::kNIMResSuccess)
		{
			ChatRoomAnoymityEnterInfo anonymity_info;
			anonymity_info.app_key_ = app_sdk::AppSDKInterface::GetAppKey();
			anonymity_info.app_data_file_ = "Netease";
			anonymity_info.address_.assign(address_list.begin(), address_list.end());
			anonymity_info.random_id_ = atoi(GetConfigValue("kNIMChatRoomEnterKeyRandomID").c_str()) > 0;
			srand((unsigned int)time(NULL));
			std::wstring nick = nbase::StringPrintf(L"�ο�%d", rand());
			ChatRoomEnterInfo enter_info;
			enter_info.SetNick(nbase::UTF16ToUTF8(nick));
			enter_info.SetAvatar("https://nos.netease.com/b12026/MTAxMTAxMA%3D%3D%2FbmltYV84NDc2NF8xNTA2NjUyMDM1NTE5XzUzZmY4NzkxLThjOGItNDY1My1hZDVhLTczYzkxZjEwN2RmOQ%3D%3D");
			StdClosure cb = [anonymity_info, enter_info, this](){
				if (ChatRoom::AnonymousEnter(room_id_, anonymity_info, enter_info))
				{
					CenterWindow();
					ShowWindow();
				}
			};
			Post2UI(cb);
		}
	}));
}

__int64 ChatroomForm::GetRoomId()
{
	return room_id_;
}

void ChatroomForm::OnReceiveMsgCallback(const ChatRoomMessage& result)
{
	if (result.msg_type_ == kNIMChatRoomMsgTypeNotification)
	{
		// ��¼�ɹ��󣬻��յ�һ������ӭ����ֱ���䡱��֪ͨ��Ϣ�������Ϣ��ʱ�����Ϊ��ȡ��ʷ��Ϣ�ı�׼
		// ֪ͨ��Ϣ��OnNotificationCallback�ص��������ﲻ������
		if (time_start_history_ == 0)
			time_start_history_ = result.timetag_ - 1;
		if (time_start_history_ < 0)
			time_start_history_ = 0;
		return;
	}
		
	AddMsgItem(result, false);
}

void ChatroomForm::OnReceiveMsgsCallback(const std::list<ChatRoomMessage>& result)
{
	for (auto &res : result)
	{
		if (res.msg_type_ == kNIMChatRoomMsgTypeNotification)
		{
			// ��¼�ɹ��󣬻��յ�һ������ӭ����ֱ���䡱��֪ͨ��Ϣ�������Ϣ��ʱ�����Ϊ��ȡ��ʷ��Ϣ�ı�׼
			// ֪ͨ��Ϣ��OnNotificationCallback�ص��������ﲻ������
			if (time_start_history_ == 0)
				time_start_history_ = res.timetag_ - 1;
			if (time_start_history_ < 0)
				time_start_history_ = 0;
			continue;
		}

		AddMsgItem(res, false);
	}
}

void ChatroomForm::OnEnterCallback(int error_code, const ChatRoomInfo& info, const ChatRoomMemberInfo& my_info)
{
	((ui::Box*)this->FindControl(L"loading_tip"))->SetVisible(false);
	if (error_code == nim::kNIMResTimeoutError)
	{
		ui::Box* kicked_tip_box = (ui::Box*)this->FindControl(L"kicked_tip_box");
		if (kicked_tip_box->IsVisible())
			return;
		kicked_tip_box->SetVisible(true);
		std::wstring kick_tip_str;
		ui::MutiLanSupport *multilan = ui::MutiLanSupport::GetInstance();
		kick_tip_str = multilan->GetStringViaID(L"STRID_CHATROOM_TIP_ENTERING");
		ui::Label* kick_tip_label = (ui::Label*)kicked_tip_box->FindSubControl(L"kick_tip");
		kick_tip_label->SetText(kick_tip_str);

// 		ui::Label* room_name_label = (ui::Label*)kicked_tip_box->FindSubControl(L"room_name");
// 		room_name_label->SetDataID(nbase::Int64ToString16(info.id_));
// 		if (!info.name_.empty())
// 			room_name_label->SetUTF8Text(info.name_);
// 		else
// 			room_name_label->SetText(nbase::StringPrintf(multilan->GetStringViaID(L"STRID_CHATROOM_ROOM_ID").c_str(), info.id_));

		//Close();
		return;
	}
	else if (error_code == nim::kNIMResSuccess)
	{
		ui::Box* kicked_tip_box = (ui::Box*)this->FindControl(L"kicked_tip_box");
		kicked_tip_box->SetVisible(false);
	}

	if (info.id_ == 0)
	{
		return;
	}

	room_id_ = info.id_;
	has_enter_ = true;

	StdClosure task = [=](){
		my_info_ = my_info;
		if (!my_info.avatar_.empty() && my_info.avatar_.find_first_of("http") == 0)
			my_icon_->SetBkImage(nim_ui::HttpManager::GetInstance()->GetCustomImage(kChatroomMemberIcon, my_info.account_id_, my_info.avatar_));
		else
			my_icon_->SetBkImage(nim_ui::PhotoManager::GetInstance()->GetUserPhoto(my_info.account_id_));

		std::wstring name = my_info.nick_.empty() ? nim_ui::UserManager::GetInstance()->GetUserName(my_info.account_id_, false) : nbase::UTF8ToUTF16(my_info.nick_);
		my_name_->SetText(name);

	};
	Post2UI(task);

	GetHistorys();

	OnGetChatRoomInfoCallback(room_id_, error_code, info);
	GetMembers();

	if (switch_to_login_status_ == kToLogined)
		SetAnonymity(false);
	else if (switch_to_login_status_ == kToAnonymous)
		SetAnonymity(true);
	else
	{
		if (nim_comp::LoginManager::GetInstance()->IsAnonymityDemoMode())
			SetAnonymity(is_anonymity_);
	}
}

void ChatroomForm::OnGetChatRoomInfoCallback(__int64 room_id, int error_code, const ChatRoomInfo& info)
{
	if (error_code != nim::kNIMResSuccess || room_id != room_id_)
	{
		return;
	}

	StdClosure cb = [=](){
		ASSERT(!info.creator_id_.empty());
		creater_id_ = info.creator_id_;

		MutiLanSupport* mls = MutiLanSupport::GetInstance();
		host_icon_->SetBkImage(nim_ui::PhotoManager::GetInstance()->GetUserPhoto(info.creator_id_));
		host_name_->SetText(mls->GetStringViaID(L"STRID_CHATROOM_HOST_") + nim_ui::UserManager::GetInstance()->GetUserName(info.creator_id_, false));

		std::wstring room_name = nbase::UTF8ToUTF16(info.name_);
		if (room_name.length() > 15)
			room_name = room_name.substr(0, 15) + L"...";
		room_name_->SetText(room_name);

		if (info.online_count_ >= 10000)
			online_num_->SetText(nbase::StringPrintf(mls->GetStringViaID(L"STRID_CHATROOM_ONLINE_NUM_EX2").c_str(), (float)info.online_count_ / (float)10000));
		else
			online_num_->SetText(nbase::StringPrintf(mls->GetStringViaID(L"STRID_CHATROOM_ONLINE_NUM_EX1").c_str(), info.online_count_));

		if (!info.announcement_.empty())
		{
			bulletin_ = ((ui::RichEdit*)FindControl(L"bulletin"));
			bulletin_->SetUTF8Text(info.announcement_);
			bulletin_->SetBkImage(L"");
			if (nim_ui::LoginManager::GetInstance()->GetAccount() == info.creator_id_)
			{
				bulletin_->SetReadOnly(false);
			}
		}

		room_mute_ = info.mute_all_ == 1;
	};
	Post2UI(cb);
}

void ChatroomForm::OnNotificationCallback(const ChatRoomNotification& notification)
{	
	AddNotifyItem(notification, false);
}

void ChatroomForm::OnChatRoomRequestEnterCallback(int error_code, const std::string& result)
{
	StdClosure closure_err = ToWeakCallback([this, error_code]()
	{
		OnRequestRoomError();
	});
	if (error_code != nim::kNIMResSuccess)
	{
		if (error_code == nim::kNIMResForbidden
			|| error_code == nim::kNIMResNotExist
			|| error_code == nim::kNIMLocalResAPIErrorInitUndone
			|| error_code == nim::kNIMLocalResAPIErrorLoginUndone
			|| error_code == nim::kNIMResAccountBlock
			|| error_code == nim::kNIMResTimeoutError
			|| error_code == nim::kNIMResRoomBlackBeOut)
		{
			StdClosure closure = ToWeakCallback([this, error_code]()
			{
				std::wstring error_tip_string_id = L"STRID_CHATROOM_TIP_ENTER_FAIL";
				if (error_code == nim::kNIMResForbidden || error_code == nim::kNIMResRoomBlackBeOut)
					error_tip_string_id = L"STRID_CHATROOM_TIP_BLACKLISTED";
				else if (error_code == nim::kNIMResAccountBlock)
					error_tip_string_id = L"STRID_CHATROOM_TIP_ACCOUNT_BLOCK";
				else if (error_code == nim::kNIMResNotExist)
					error_tip_string_id = L"STRID_CHATROOM_TIP_ROOM_NOT_EXIST";
				else if (error_code == nim::kNIMLocalResAPIErrorInitUndone
					|| error_code == nim::kNIMLocalResAPIErrorLoginUndone
					|| error_code == nim::kNIMResTimeoutError)
					error_tip_string_id = L"STRID_CHATROOM_TIP_NETWORK_ERROR";

				if (!nim_comp::LoginManager::GetInstance()->IsAnonymityDemoMode())
				{
					ChatroomFrontpage* front_page = nim_ui::WindowsManager::GetInstance()->SingletonShow<ChatroomFrontpage>(ChatroomFrontpage::kClassName);
					if (!front_page) return;
					ui::Box* kicked_tip_box = (ui::Box*)front_page->FindControl(L"kicked_tip_box");
					kicked_tip_box->SetVisible(true);
					nbase::ThreadManager::PostDelayedTask(front_page->ToWeakCallback([kicked_tip_box]() {
						kicked_tip_box->SetVisible(false);
					}), nbase::TimeDelta::FromSeconds(2));

					MutiLanSupport* mls = MutiLanSupport::GetInstance();
					std::wstring kick_tip_str = mls->GetStringViaID(error_tip_string_id);

					ui::Label* kick_tip_label = (ui::Label*)kicked_tip_box->FindSubControl(L"kick_tip");
					kick_tip_label->SetText(kick_tip_str);

					ui::Label* room_name_label = (ui::Label*)kicked_tip_box->FindSubControl(L"room_name");
					if (error_code == nim::kNIMResForbidden)
					{
						room_name_label->SetDataID(nbase::Int64ToString16(room_id_));
						ChatRoomInfo info = front_page->GetRoomInfo(room_id_);
						if (!info.name_.empty())
							room_name_label->SetUTF8Text(info.name_);
						else
							room_name_label->SetText(nbase::StringPrintf(mls->GetStringViaID(L"STRID_CHATROOM_ROOM_ID").c_str(), room_id_));
					}
					else if (error_code == nim::kNIMResNotExist)
						room_name_label->SetText(L"");					
					OnRequestRoomError();
				}
				else
				{
					ShowMsgBox(nullptr, MsgboxCallback(), error_tip_string_id);
					Logout();
				}
			});
			Post2UI(closure);
		}
		else
		{
			if (!nim_comp::LoginManager::GetInstance()->IsAnonymityDemoMode())
				Post2UI(closure_err);
			else
				Logout();
			QLOG_APP(L"OnChatRoomRequestEnterCallback error: error id={0}") << error_code;
		}
		return;
	}

	if (!result.empty())
	{
		StdClosure cb = [result, this](){
			room_enter_token_ = result;
			ChatRoomEnterInfo info;
			//Json::Value values;
			//Json::Reader reader;
			//std::string test_string = "{\"remote\":{\"mapmap\":{\"int\":1,\"boolean\":false,\"list\":[1,2,3],\"string\":\"string, lalala\"}}}";
			//if (reader.parse(test_string, values))
			//	info.SetExt(values);
			//info.SetNotifyExt(values);
			bool bRet = ChatRoom::Enter(room_id_, room_enter_token_, info);
			if (bRet)
			{
				this->CenterWindow();
				this->ShowWindow();
			}
			else
			{
				QLOG_APP(L"ChatRoom::Enter error");

				ChatroomFrontpage* front_page = (ChatroomFrontpage*)nim_ui::WindowsManager::GetInstance()->GetWindow(ChatroomFrontpage::kClassName, ChatroomFrontpage::kClassName);
				if (!front_page) return;
				ShowMsgBox(front_page->GetHWND(), MsgboxCallback(), L"STRID_CHATROOM_ENTER_ROOM_FAIL");

				if (!nim_comp::LoginManager::GetInstance()->IsAnonymityDemoMode())
					OnRequestRoomError();
				else
					Logout(); 
			}
		};
		Post2UI(cb);
	}
	else
	{
		if (!nim_comp::LoginManager::GetInstance()->IsAnonymityDemoMode())
			Post2UI(closure_err);
		else
			Logout();
	}

}

void ChatroomForm::OnRegLinkConditionCallback(__int64 room_id, const NIMChatRoomLinkCondition condition)
{
	if (room_id_ != room_id)
		return;

	if (condition == kNIMChatRoomLinkConditionAlive && has_enter_)
	{
		GetHistorys();
	}
	else if (condition == kNIMChatRoomLinkConditionDead)
	{
		input_edit_->SetText(L"");
	}
}

void ChatroomForm::OnSetMemberAttributeCallback(__int64 room_id, int error_code, const ChatRoomMemberInfo& info)
{
	if (room_id_ != room_id)
		return;

	StdClosure cb = [=](){

		if (error_code != nim::kNIMResSuccess)
		{
			std::wstring tip = nbase::StringPrintf(L"Set member attribute, error:%d", error_code);
			nim_ui::ShowToast(tip, 5000);
			return;
		}

		auto it = members_map_.find(info.account_id_);
		if (it != members_map_.end())
		{
			if (it->second.type_ == 2 && info.type_ != 2)
			{
				SetMemberAdmin(info.account_id_, false);
			}
			else if (it->second.type_ != 2 && info.type_ == 2)
			{
				SetMemberAdmin(info.account_id_, true);
			}
		}
		else
		{
			if (info.type_ != 2)
			{
				SetMemberAdmin(info.account_id_, false);
			}
			else if (info.type_ == 2)
			{
				SetMemberAdmin(info.account_id_, true);
			}

			members_map_[info.account_id_] = info;
		}	
	};
	Post2UI(cb);
}

void ChatroomForm::OnKickMemberCallback(__int64 room_id, int error_code)
{
	if (room_id_ != room_id)
		return;

	StdClosure cb = [=](){

		if (error_code != nim::kNIMResSuccess)
		{
			std::wstring tip = nbase::StringPrintf(L"Kick member %s, error:%d", nbase::UTF8ToUTF16(kicked_user_account_).c_str(), error_code);
			nim_ui::ShowToast(tip, 5000);
			return;
		}

		RemoveMember(kicked_user_account_);
		kicked_user_account_.clear();
	};
	Post2UI(cb);

}

void ChatroomForm::OnTempMuteCallback(__int64 room_id, int error_code, const ChatRoomMemberInfo& info)
{
	if (room_id_ != room_id)
		return;

	StdClosure cb = [=](){

		if (error_code != nim::kNIMResSuccess)
		{
			std::wstring tip = nbase::StringPrintf(L"TempMute, error:%d", error_code);
			nim_ui::ShowToast(tip, 5000);
			return;
		}

		auto iter = members_map_.find(info.account_id_);
		if (iter == members_map_.end())
		{
			members_map_[info.account_id_] = info;
		}

		SetMemberTempMute(info.account_id_, info.temp_muted_, info.temp_muted_ ? info.temp_muted_duration_ : 0);
	};
	Post2UI(cb);
}

void ChatroomForm::InitHeader()
{
	std::string my_id = nim_ui::LoginManager::GetInstance()->GetAccount();
	my_icon_->SetBkImage(nim_ui::PhotoManager::GetInstance()->GetUserPhoto(my_id));
	my_name_->SetText(nim_ui::UserManager::GetInstance()->GetUserName(my_id, false));
}

void ChatroomForm::GetOnlineCount()
{
	ChatRoom::GetInfoAsync(room_id_, nbase::Bind(&ChatroomForm::OnGetChatRoomInfoCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void ChatroomForm::GetMembers()
{
	ChatRoomGetMembersParameters member_param;
	member_param.type_ = kNIMChatRoomGetMemberTypeSolid;
	ChatRoom::GetMembersOnlineAsync(room_id_, member_param, nbase::Bind(&ChatroomForm::OnGetMembersCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	member_param.type_ = kNIMChatRoomGetMemberTypeTemp;
	ChatRoom::GetMembersOnlineAsync(room_id_, member_param, nbase::Bind(&ChatroomForm::OnGetMembersCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void ChatroomForm::OnGetMembersCallback(__int64 room_id, int error_code, const std::list<ChatRoomMemberInfo>& infos)
{
	if (error_code != nim::kNIMResSuccess || room_id_ != room_id)
		return;

	StdClosure cb = [=](){
		empty_members_list_->SetVisible(false);

		for (auto& iter : infos)
		{
			// ���������ߵ��ο�
			if (iter.guest_flag_ == kNIMChatRoomGuestFlagGuest && iter.state_ == kNIMChatRoomOnlineStateOffline)
				continue;

			if (members_map_.find(iter.account_id_) != members_map_.end())
				continue;

			members_map_[iter.account_id_] = iter;
			nick_account_map_[iter.nick_] = iter.account_id_;

			if (iter.type_ == 1)
			{
				if (!iter.avatar_.empty() && iter.avatar_.find_first_of("http") == 0)
					host_icon_->SetBkImage(nim_ui::HttpManager::GetInstance()->GetCustomImage(kChatroomMemberIcon, iter.account_id_, iter.avatar_));
				else
					host_icon_->SetBkImage(nim_ui::PhotoManager::GetInstance()->GetUserPhoto(iter.account_id_));
				host_name_->SetText(MutiLanSupport::GetInstance()->GetStringViaID(L"STRID_CHATROOM_HOST_") + nbase::UTF8ToUTF16(iter.nick_));

				has_add_creater_ = true;
			}
			else if (iter.type_ == 2)
				managers_list_.push_back(iter.account_id_);
			else
				members_list_.push_back(iter.account_id_);

//			AddMemberItem(iter);
		}

		if (option_online_members_->IsSelected())
			online_members_virtual_list_->Refresh();
	};

	Post2UI(cb);
}

void ChatroomForm::GetHistorys()
{
	ChatRoomGetMsgHistoryParameters history_param;
	history_param.limit_ = 10;
	history_param.start_timetag_ = time_start_history_;	
	//history_param.reverse_ = false;
	//history_param.RemoveMessageType(kNIMChatRoomMsgTypeAudio);//����ȡ��Ƶ����Ϣ
	//history_param.RemoveMessageType(kNIMChatRoomMsgTypeVideo);//����ȡ��Ƶ����Ϣ
	//history_param.RemoveMessageType(kNIMChatRoomMsgTypeCustom);//����ȡ�Զ�����Ϣ
	ChatRoom::GetMessageHistoryOnlineAsync(room_id_, history_param, nbase::Bind(&ChatroomForm::GetMsgHistoryCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void ChatroomForm::GetMsgHistoryCallback(__int64 room_id, int error_code, const std::list<ChatRoomMessage>& msgs)
{
	if (error_code != nim::kNIMResSuccess || room_id_ != room_id)
	{
		StdClosure cb = [=](){
			is_loading_history_ = false;
		};
		Post2UI(cb);
		return;
	}

	StdClosure cb = [=](){
		is_loading_history_ = false;
		for (auto it = msgs.begin(); it != msgs.end(); it++)
		{
			AddMsgItem(*it, true, false);
			time_start_history_ = it->timetag_ - 1;
			if (time_start_history_ < 0)
				time_start_history_ = 0;
		}
	};
	Post2UI(cb);

}

void ChatroomForm::OnHttoDownloadReady(HttpResourceType type, const std::string& account, const std::wstring& photo_path)
{
	if (type == kChatroomMemberIcon)
	{
		if (nim_ui::LoginManager::GetInstance()->IsEqual(account) || account == my_info_.account_id_)
			my_icon_->SetBkImage(photo_path);

		if (account == creater_id_)
			host_icon_->SetBkImage(photo_path);

		// ���������߳�Ա�б�������ˢ�³�Ա������ֻ���л������߳�Ա�б�ҳʱ���Ų���UI
		if (option_online_members_->IsSelected())
		{
			ui::ButtonBox* room_member_item = (ui::ButtonBox*)online_members_virtual_list_->FindSubControl(nbase::UTF8ToUTF16(account));
			if (room_member_item != NULL)
			{
				ui::Control* header_image = (ui::Control*)room_member_item->FindSubControl(L"header_image");
				header_image->SetBkImage(photo_path);
			}

// 			room_member_item = (ui::ButtonBox*)online_members_list_->FindSubControl(nbase::UTF8ToUTF16(account));
// 			if (room_member_item != NULL)
// 			{
// 				ui::Control* header_image = (ui::Control*)room_member_item->FindSubControl(L"header_image");
// 				header_image->SetBkImage(photo_path);
// 			}
		}
	}
}

void ChatroomForm::OnRequestRoomError()
{
	this->Close(kForceClose);
}

void ChatroomForm::AddMsgItem(const ChatRoomMessage& result, bool is_history, bool first_msg_each_batch)
{
	if (result.msg_type_ == kNIMChatRoomMsgTypeText)
	{
		AddText(nbase::UTF8ToUTF16(result.msg_attach_), nbase::UTF8ToUTF16(result.from_nick_), result.from_id_, kMember, is_history, first_msg_each_batch);
	}
	else if (result.msg_type_ == kNIMChatRoomMsgTypeNotification)
	{
		Json::Value json;
		Json::Reader reader;
		if (reader.parse(result.msg_attach_, json))
		{
			ChatRoomNotification notification;
			notification.ParseFromJsonValue(shared::tools::JsonValueToNimCppWrapperJsonValue(json));
			AddNotifyItem(notification, is_history, first_msg_each_batch);
		}
	}
	else if (result.msg_type_ == kNIMChatRoomMsgTypeCustom)
	{
		Json::Value json;
		Json::Reader reader;
		if (reader.parse(result.msg_attach_, json))
		{
			int sub_type = json["type"].asInt();
			if (sub_type == nim_comp::CustomMsgType_Jsb && json["data"].isObject())
			{
				int value = json["data"]["value"].asInt();
				AddJsb(value, nbase::UTF8ToUTF16(result.from_nick_), result.from_id_, is_history, first_msg_each_batch);
			}
		}
	}

	if (!result.from_nick_.empty())
		nick_account_map_[result.from_nick_] = result.from_id_;
}

void ChatroomForm::AddNotifyItem(const ChatRoomNotification& notification, bool is_history, bool first_msg_each_batch/* = false*/)
{
	MutiLanSupport* mls = MutiLanSupport::GetInstance();
	std::string my_id = nim_ui::LoginManager::GetInstance()->GetAccount();

	auto it_nick = notification.target_nick_.cbegin();
	auto it_id = notification.target_ids_.cbegin();
	for (;	it_nick != notification.target_nick_.cend(), it_id != notification.target_ids_.cend();
		++it_id, ++it_nick)
	{
		std::wstring nick = nbase::UTF8ToUTF16(*it_nick);
		if (*it_id == my_id)
			nick = mls->GetStringViaID(L"STRID_CHATROOM_NOTIFY_YOU");

		std::wstring str;
		if (notification.id_ == kNIMChatRoomNotificationIdMemberIn)
		{
			str = nbase::StringPrintf(mls->GetStringViaID(L"STRID_CHATROOM_NOTIFY_WELCOME").c_str(), nick.c_str());
		}
		else if (notification.id_ == kNIMChatRoomNotificationIdMemberExit)
		{
			str = nbase::StringPrintf(mls->GetStringViaID(L"STRID_CHATROOM_NOTIFY_EXIT").c_str(), nick.c_str());
			RemoveMember(*it_id);
		}
		else if (notification.id_ == kNIMChatRoomNotificationIdAddBlack)
		{
			str = nbase::StringPrintf(mls->GetStringViaID(L"STRID_CHATROOM_NOTIFY_BLACKLISTED").c_str(), nick.c_str());
			SetMemberBlacklist(*it_id, true);
		}
		else if (notification.id_ == kNIMChatRoomNotificationIdRemoveBlack)
		{
			str = nbase::StringPrintf(mls->GetStringViaID(L"STRID_CHATROOM_NOTIFY_UNBLACKLISTED").c_str(), nick.c_str());
			SetMemberBlacklist(*it_id, false);
		}
		else if (notification.id_ == kNIMChatRoomNotificationIdAddMute)
		{
			str = nbase::StringPrintf(mls->GetStringViaID(L"STRID_CHATROOM_NOTIFY_MUTE").c_str(), nick.c_str());
			SetMemberMute(*it_id, true);
		}
		else if (notification.id_ == kNIMChatRoomNotificationIdRemoveMute)
		{
			str = nbase::StringPrintf(mls->GetStringViaID(L"STRID_CHATROOM_NOTIFY_UNMUTE").c_str(), nick.c_str());
			SetMemberMute(*it_id, false);
		}
		else if (notification.id_ == kNIMChatRoomNotificationIdAddManager)
		{
			str = nbase::StringPrintf(mls->GetStringViaID(L"STRID_CHATROOM_NOTIFY_APPOINT").c_str(), nick.c_str());
			SetMemberAdmin(*it_id, true);
		}
		else if (notification.id_ == kNIMChatRoomNotificationIdRemoveManager)
		{
			str = nbase::StringPrintf(mls->GetStringViaID(L"STRID_CHATROOM_NOTIFY_DISMISS").c_str(), nick.c_str());
			SetMemberAdmin(*it_id, false);
		}
		else if (notification.id_ == kNIMChatRoomNotificationIdAddFixed)
		{
			SetMemberFixed(*it_id, true);
		}
		else if (notification.id_ == kNIMChatRoomNotificationIdRemoveFixed)
		{
			SetMemberFixed(*it_id, false);
		}
		else if (notification.id_ == kNIMChatRoomNotificationIdMemberKicked)
		{
			str = nbase::StringPrintf(mls->GetStringViaID(L"STRID_CHATROOM_NOTIFY_KICK").c_str(), nick.c_str());
			RemoveMember(*it_id);
		}
		else if (notification.id_ == kNIMChatRoomNotificationIdMemberTempMute)
		{
			str = nbase::StringPrintf(mls->GetStringViaID(L"STRID_CHATROOM_NOTIFY_TEMP_MUTE").c_str(), nick.c_str(), notification.temp_mute_duration_);
			SetMemberTempMute(*it_id, true, notification.temp_mute_duration_);
		}
		else if (notification.id_ == kNIMChatRoomNotificationIdMemberTempUnMute)
		{
			str = nbase::StringPrintf(mls->GetStringViaID(L"STRID_CHATROOM_NOTIFY_TEMP_UNMUTE").c_str(), nick.c_str(), notification.temp_mute_duration_);
			SetMemberTempMute(*it_id, false, notification.temp_mute_duration_);
		}
		AddNotify(str, is_history, first_msg_each_batch);
	}

	if (notification.id_ == kNIMChatRoomNotificationIdRoomMuted || notification.id_ == kNIMChatRoomNotificationIdRoomDeMuted)
	{
		std::wstring str;
		str = mls->GetStringViaID(notification.id_ == kNIMChatRoomNotificationIdRoomMuted ? L"STRID_CHATROOM_NOTIFY_ROOM_MUTE" : L"STRID_CHATROOM_NOTIFY_ROOM_UNMUTE");
		room_mute_ = notification.id_ == kNIMChatRoomNotificationIdRoomMuted;
		AddNotify(str, is_history, first_msg_each_batch);
	}
	if (notification.id_ == kNIMChatRoomNotificationIdQueueBatchChanged)
	{
		std::string str;
		ChatRoomQueueBatchChangedNotification batch_chg_info;
		batch_chg_info.ParseFromNotification(notification);		
		if (batch_chg_info.type_.compare(kNIMChatRoomNotificationQueueChangedType_PARTCLEAR) == 0)
		{
			for (auto it : batch_chg_info.changed_values_)
			{
				str.append(it.first).append(" : ").append(it.second).append("\r\n");
			}
			if (str.length() > 0)
				AddNotify(nbase::UTF8ToUTF16(str), is_history, first_msg_each_batch);
		}
	}
	if (!notification.ext_.empty())
	{
		std::string toast = nbase::StringPrintf("notification_id:%d, from_nick:%s(%s), notify_ext:%s", notification.id_, notification.operator_nick_.c_str(), notification.operator_id_.c_str(), notification.ext_.c_str());
		nim_ui::ShowToast(nbase::UTF8ToUTF16(toast), 5000, this->GetHWND());
	}
}

void ChatroomForm::OnBtnSend()
{
	if (!nim_ui::LoginManager::GetInstance()->IsLinkActive())
	{
		return;
	}

	MutiLanSupport* mls = MutiLanSupport::GetInstance();

	if (room_mute_)
	{
		std::wstring toast = mls->GetStringViaID(L"STRID_CHATROOM_NOTIFY_ROOM_MUTING");
		nim_ui::ShowToast(toast, 5000, this->GetHWND());
	}
	else
	{
		auto my_info = members_map_.find(nim_ui::LoginManager::GetInstance()->GetAccount());
		if (my_info != members_map_.end())
		{
			if (my_info->second.is_muted_)
			{
				std::wstring toast = mls->GetStringViaID(L"STRID_CHATROOM_NOTIFY_YOU_MUTED");
				nim_ui::ShowToast(toast, 5000, this->GetHWND());
			}
			else if (my_info->second.temp_muted_)
			{
				std::wstring toast = mls->GetStringViaID(L"STRID_CHATROOM_NOTIFY_YOU_TEMP_MUTED");
				nim_ui::ShowToast(toast, 5000, this->GetHWND());
			}
			else
			{
				wstring sText, sTestText;
				nim_comp::Re_GetText(input_edit_->GetTextServices(), sText);
				sTestText = StringHelper::Trim(sText.c_str());
				if (sTestText.empty()) return;
				input_edit_->SetText(_T(""));
				SendText(nbase::UTF16ToUTF8(sText));

				std::string my_id = nim_ui::LoginManager::GetInstance()->GetAccount();
				std::wstring my_name = nim_ui::UserManager::GetInstance()->GetUserName(my_id, false);
				AddText(sText, my_name, my_id, kMember, false);
			}
		}
	}
}

void ChatroomForm::SendText(const std::string &text)
{
	NIMChatRoomMsgType msg_type = kNIMChatRoomMsgTypeText;
	std::string robot_accid;
	//�ж��Ƿ����@ĳ�˵���Ϣ
	if (!uid_at_someone_.empty())
	{
		//����ı���Ϣ���Ƿ���ڡ�@xxx �����ı�
		for (auto it = uid_at_someone_.begin(); it != uid_at_someone_.end(); ++it)
		{
			std::string nick_name = it->first;
			std::string at_str = "@";
			at_str.append(nick_name);
			at_str.append(" ");

			int index = text.find(at_str);
			if (index != std::string::npos)
			{
				if (it->second.is_robot_)
				{
					msg_type = kNIMChatRoomMsgTypeRobot;
					if (robot_accid.empty())//ֻ�����һ��
						robot_accid = it->second.uid_;
				}
			}
		}
		uid_at_someone_.clear();
	}

	nim::IMBotRobot bot;
	bot.robot_accid_ = robot_accid;
	std::wstring robot_content = nbase::UTF8ToUTF16(text);
	int index = robot_content.find(L"@");
	if (index != -1)
	{
		int space_index = 0;
		while (space_index != -1 && index != -1)
		{
			space_index = robot_content.find(L" ", index);
			if (space_index != -1)
			{
				robot_content = robot_content.replace(index, space_index - index + 1, L"");
				index = robot_content.find(L"@", index);
			}
		}
	}
	bot.sent_param_["content"] = robot_content.empty() ? " " : nbase::UTF16ToUTF8(robot_content);
	bot.sent_param_["type"] = "01";

	std::string send_msg;
	if (msg_type == kNIMChatRoomMsgTypeText)
		send_msg = ChatRoom::CreateRoomMessage(msg_type, QString::GetGUID(), text, "", ChatRoomMessageSetting());
	else
		send_msg = ChatRoom::CreateRoomMessage(msg_type, QString::GetGUID(), bot.ToJsonString(), text, ChatRoomMessageSetting());
	ChatRoom::SendMsg(room_id_, send_msg);
}

void ChatroomForm::OnBtnJsb()
{
	if (!nim_ui::LoginManager::GetInstance()->IsLinkActive())
	{
		return;
	}

	auto my_info = members_map_.find(nim_ui::LoginManager::GetInstance()->GetAccount());
	if (my_info != members_map_.end() && !room_mute_ && !my_info->second.is_muted_ && !my_info->second.temp_muted_)
	{
		int jsb = (rand() % 3 + rand() % 4 + rand() % 5) % 3 + 1;

		Json::Value json;
		Json::FastWriter writer;
		json["type"] = nim_comp::CustomMsgType_Jsb;
		json["data"]["value"] = jsb;

		SendJsb(writer.write(json));
		std::wstring my_name = nim_ui::UserManager::GetInstance()->GetUserName(nim_ui::LoginManager::GetInstance()->GetAccount(), false);
		AddJsb(jsb, my_name, nim_ui::LoginManager::GetInstance()->GetAccount(), false);
	}
}

void ChatroomForm::SendJsb(const std::string & attach)
{
	std::string send_msg = ChatRoom::CreateRoomMessage(kNIMChatRoomMsgTypeCustom, QString::GetGUID(), attach, "", ChatRoomMessageSetting());
	ChatRoom::SendMsg(room_id_, send_msg);
}

void ChatroomForm::SendImage(const std::wstring &src)
{
	nim::IMImage img;
	std::string utf8 = nbase::UTF16ToUTF8(src);
	img.md5_ = nim::Tool::GetFileMd5(utf8);
	img.size_ = (long)nbase::GetFileSize(src);
	std::wstring file_name, file_ext;
	nbase::FilePathApartFileName(src, file_name);
	nbase::FilePathExtension(src, file_ext);
	img.display_name_ = nbase::UTF16ToUTF8(file_name);
	img.file_extension_ = nbase::UTF16ToUTF8(file_ext);
	Gdiplus::Image image(src.c_str());
	if (image.GetLastStatus() != Gdiplus::Ok)
	{
		assert(0);
	}
	else
	{
		img.width_ = image.GetWidth();
		img.height_ = image.GetHeight();
	}

	auto weak_flag = this->GetWeakFlag();
	nim::NOS::UploadResource(utf8, [this, img, weak_flag](int res_code, const std::string& url) {
		if (!weak_flag.expired() && res_code == nim::kNIMResSuccess)
		{
			nim::IMImage new_img(img);
			new_img.url_ = url;
			std::string send_msg = ChatRoom::CreateRoomMessage(kNIMChatRoomMsgTypeImage, QString::GetGUID(), new_img.ToJsonString(), "", ChatRoomMessageSetting());
			ChatRoom::SendMsg(room_id_, send_msg);
		}
	});
}
}