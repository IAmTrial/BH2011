#include "SpamFilter.h"
#include "../../D2Ptrs.h"
#include "../../D2Intercepts.h"
#include "../../D2Stubs.h"
#include <sstream>
#include "../../BH.h"
#include <time.h>
#include <WinInet.h>

#include <iterator>

#pragma comment(lib, "wininet")

void SpamFilter::OnLoad()
{
	// load the token database and settings, maybe check for updates?
	Config spamfilter(BH::config->ReadAssoc("Spam Filter"));
	log = spamfilter.ReadString("Log File", "chatlog.txt");
	url = spamfilter.ReadString("Update URL", "");
	bayes = Bayes(spamfilter.ReadString("Token DB", "tokens.db").c_str());

	Toggles["Log Chat"] = spamfilter.ReadToggle("Log Chat", "None", false);
	Toggles["Log Blocks"] = spamfilter.ReadToggle("Log Blocked Messages", "None", false);
	Toggles["Enabled"] = spamfilter.ReadToggle("Enabled", "None", false);
	Toggles["Autosave"] = spamfilter.ReadToggle("Autosave Databases", "None", false);
}

void SpamFilter::OnUnload()
{
	bayes.SaveDatabases();
}

void SpamFilter::DoUpdate()
{
	HINTERNET hInt = InternetOpen("SpamFilter 1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);

	if(hInt == NULL) {
		return;
	}

	char *upurl = new char[INTERNET_MAX_URL_LENGTH];
	memset(upurl, 0, INTERNET_MAX_URL_LENGTH);
	sprintf_s(upurl, INTERNET_MAX_URL_LENGTH, "%s?date=%d", url, bayes.GetLastUpdate());

	HINTERNET hFile = InternetOpenUrl(hInt, upurl, NULL, 0, INTERNET_FLAG_NO_UI|INTERNET_FLAG_NO_CACHE_WRITE|INTERNET_FLAG_RELOAD, NULL);
	delete[] upurl;
	if(hFile == NULL) {
		return;
	}

	char buf[1024];
	std::stringstream result;
	BOOL done = FALSE;
	do {
		DWORD dummy;
		if((done = InternetReadFile(hFile, buf, 1024, &dummy)) == TRUE)
			result << buf;
		done = (done == TRUE && dummy == 0);
	} while(done);
	TokenList ham, spam;
	time_t updated;
	bayes.ParseUpdate(result, &updated, ham, spam);
	if(updated > bayes.GetLastUpdate()) {
		bayes.Merge(ham, Ham);
		bayes.Merge(spam, Spam);
		bayes.SaveDatabases();
	}

	InternetCloseHandle(hInt);
}

void SpamFilter::OnUserInput(const wchar_t* msg, bool fromGame, bool* parsed)
{
	wchar_t *buf = const_cast<wchar_t*>(msg), *ctx = NULL, *seps = L" ";
	wchar_t* cmd = wcstok_s(buf, seps, &ctx);
	wchar_t* t = buf+wcslen(cmd)+1;
	std::string token;
	std::copy(token.begin(), token.end(), std::back_inserter(std::wstring(t)));
	const char* param = token.c_str();

	if(_wcsicmp(cmd, L"spam") == 0)
	{
		*parsed = true;
		int count = bayes.GetSpamTokenCount();
		bayes.AddSpam(param);
		count = bayes.GetSpamTokenCount() - count;
		//Print(".: SpamFilter :. Added %d new spam tokens! (previous token counts may have been updated)", count);
		if(Toggles["Autosave"].state)
			bayes.SaveDatabases();
	}
	else if(_wcsicmp(cmd, L"ham") == 0)
	{
		*parsed = true;
		int count = bayes.GetHamTokenCount();
		bayes.AddHam(param);
		count = bayes.GetHamTokenCount() - count;
		//Print(".: SpamFilter :. Added %d new ham tokens! (previous token counts may have been updated)", count);
		if(Toggles["Autosave"].state)
			bayes.SaveDatabases();
	}
	else if(_wcsicmp(cmd, L"update") == 0)
	{
		*parsed = true;
		DoUpdate();
	}
	else if(_wcsicmp(cmd, L"save") == 0)
	{
		*parsed = true;
		bayes.SaveDatabases();
		//Print(".: SpamFilter :. Saved databases!");
	}
	else if(_wcsicmp(cmd, L"reload") == 0)
	{
		*parsed = true;
		bayes.LoadDatabases();
		//Print(".: SpamFilter :. Loaded databases! Total spam tokens: %d, ham tokens: %d", bayes.GetSpamTokenCount(), bayes.GetHamTokenCount());
	}
	else if(_wcsicmp(cmd, L"set") == 0)
	{
		*parsed = true;
		if(strlen(param) == 0)
		{
			//Print(".: SpamFilter :. Options are currently: enabled: %s, logchat: %s, logblocks: %s, autosave: %s",
			//		(enabled ? "on" : "off"), (logchat ? "on" : "off"), (logblocks ? "on" : "off"), (autosave ? "on" : "off"));
			return;
		}
		char *buf = const_cast<char*>(param), *ctx = NULL, *seps = " ";
		char* token = strtok_s(buf, seps, &ctx);
		char* value = buf+strlen(token)+1;
		int len = strlen(value);
		if(len > 1)
		{
			if(_stricmp(token, "enabled") == 0)
			{
				Toggles["Enabled"].state = StringToBool(value);
				//Print(".: SpamFilter :. SpamFilter is now %s", (enabled ? "enabled" : "disabled"));
			}
			else if(_stricmp(token, "logchat") == 0)
			{
				Toggles["Log Chat"].state = StringToBool(value);
				//Print(".: SpamFilter :. SpamFilter is now %slogging chat", (logchat ? "" : "not "));
			}
			else if(_stricmp(token, "logblocks") == 0)
			{
				Toggles["Log Blocks"].state = StringToBool(value);
				//Print(".: SpamFilter :. SpamFilter is now %slogging blocked messages (though chat logging may still be off!)", (logblocks ? "" : "not "));
			}
			else if(_stricmp(token, "autosave") == 0)
			{
				Toggles["Autosave"].state = StringToBool(value);
				//Print(".: SpamFilter :. SpamFilter is now %sautosaving the databases", (autosave ? "" : "not "));
			}
			//else Print(".: SpamFilter :. Unknown option!");
		}
	}
	else if(_wcsicmp(cmd, L"help") == 0)
	{
		*parsed = true;
		/*Print(".: SpamFilter :. Options");
		Print(".: SpamFilter :. .reload - Reload the spam and ham databases");
		Print(".: SpamFilter :. .save - Save the spam and ham databases");
		Print(".: SpamFilter :. .update - Check to see if there is a spam/ham database update");
		Print(".: SpamFilter :. .spam <string> - Add <string> to the spam database");
		Print(".: SpamFilter :. .ham <string> - Add <string> to the ham database");
		Print(".: SpamFilter :. .set <param> <value> - Set various parameters (true, TRUE, 1, and on are all enabled, false, FALSE, 0, and off are all disabled)");
		Print(".: SpamFilter :. Parameters for .set:");
		Print(".: SpamFilter :. enabled - Enable or disable spam blocking (default: true)");
		Print(".: SpamFilter :. logchat - Enable or disable chat logging (default: true)");
		Print(".: SpamFilter :. logblocks - Enable or disable logging blocked chat messages (default: false)");
		Print(".: SpamFilter :. autosave - Enable or disable auto-saving the spam and ham databases (default: true)");*/
	}
}

void SpamFilter::OnChatMsg(const char* szAcc, const char* szText, bool fromGame, bool* block)
{
	time_t currentTime;
	time(&currentTime);
	char szTime[60] = "";
	struct tm time;
	localtime_s(&time, &currentTime);
	strftime(szTime, sizeof(szTime), "%x %X", &time);

	int cat = bayes.Categorize(szText);
	*block = cat == -1;

	if(Toggles["Log Chat"].state && (!block || (block && Toggles["Log Blocks"].state)))
	{
		FILE* chatlog = NULL;
		fopen_s(&chatlog, log.c_str(), "a+");
		char* pos = strchr(const_cast<char*>(szAcc), '*');
		if(pos != NULL && pos != szAcc)
			*pos = '\0';
		fprintf(chatlog, "[%s] %s%s: %s\n", szTime, (block ? "BLOCKING: " : ""), szAcc, szText);
		fclose(chatlog);
	}
}
#include "SpamFilter.h"
#include "../../D2Ptrs.h"
#include "../../D2Intercepts.h"
#include "../../D2Stubs.h"
#include <sstream>
#include "../../BH.h"
#include <time.h>
#include <WinInet.h>

#include <iterator>

#pragma comment(lib, "wininet")

void SpamFilter::OnLoad()
{
	// load the token database and settings, maybe check for updates?
	Config spamfilter(BH::config->ReadAssoc("Spam Filter"));
	log = spamfilter.ReadString("Log File", "chatlog.txt");
	url = spamfilter.ReadString("Update URL", "");
	bayes = Bayes(spamfilter.ReadString("Token DB", "tokens.db").c_str());

	Toggles["Log Chat"] = spamfilter.ReadToggle("Log Chat", "None", false);
	Toggles["Log Blocks"] = spamfilter.ReadToggle("Log Blocked Messages", "None", false);
	Toggles["Enabled"] = spamfilter.ReadToggle("Enabled", "None", false);
	Toggles["Autosave"] = spamfilter.ReadToggle("Autosave Databases", "None", false);
}

void SpamFilter::OnUnload()
{
	bayes.SaveDatabases();
}

void SpamFilter::DoUpdate()
{
	HINTERNET hInt = InternetOpen("SpamFilter 1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);

	if(hInt == NULL) {
		return;
	}

	char *upurl = new char[INTERNET_MAX_URL_LENGTH];
	memset(upurl, 0, INTERNET_MAX_URL_LENGTH);
	sprintf_s(upurl, INTERNET_MAX_URL_LENGTH, "%s?date=%d", url, bayes.GetLastUpdate());

	HINTERNET hFile = InternetOpenUrl(hInt, upurl, NULL, 0, INTERNET_FLAG_NO_UI|INTERNET_FLAG_NO_CACHE_WRITE|INTERNET_FLAG_RELOAD, NULL);
	delete[] upurl;
	if(hFile == NULL) {
		return;
	}

	char buf[1024];
	std::stringstream result;
	BOOL done = FALSE;
	do {
		DWORD dummy;
		if((done = InternetReadFile(hFile, buf, 1024, &dummy)) == TRUE)
			result << buf;
		done = (done == TRUE && dummy == 0);
	} while(done);
	TokenList ham, spam;
	time_t updated;
	bayes.ParseUpdate(result, &updated, ham, spam);
	if(updated > bayes.GetLastUpdate()) {
		bayes.Merge(ham, Ham);
		bayes.Merge(spam, Spam);
		bayes.SaveDatabases();
	}

	InternetCloseHandle(hInt);
}

void SpamFilter::OnUserInput(const wchar_t* msg, bool fromGame, bool* parsed)
{
	wchar_t *buf = const_cast<wchar_t*>(msg), *ctx = NULL, *seps = L" ";
	wchar_t* cmd = wcstok_s(buf, seps, &ctx);
	wchar_t* t = buf+wcslen(cmd)+1;
	std::string token;
	std::copy(token.begin(), token.end(), std::back_inserter(std::wstring(t)));
	const char* param = token.c_str();

	if(_wcsicmp(cmd, L"spam") == 0)
	{
		*parsed = true;
		int count = bayes.GetSpamTokenCount();
		bayes.AddSpam(param);
		count = bayes.GetSpamTokenCount() - count;
		//Print(".: SpamFilter :. Added %d new spam tokens! (previous token counts may have been updated)", count);
		if(Toggles["Autosave"].state)
			bayes.SaveDatabases();
	}
	else if(_wcsicmp(cmd, L"ham") == 0)
	{
		*parsed = true;
		int count = bayes.GetHamTokenCount();
		bayes.AddHam(param);
		count = bayes.GetHamTokenCount() - count;
		//Print(".: SpamFilter :. Added %d new ham tokens! (previous token counts may have been updated)", count);
		if(Toggles["Autosave"].state)
			bayes.SaveDatabases();
	}
	else if(_wcsicmp(cmd, L"update") == 0)
	{
		*parsed = true;
		DoUpdate();
	}
	else if(_wcsicmp(cmd, L"save") == 0)
	{
		*parsed = true;
		bayes.SaveDatabases();
		//Print(".: SpamFilter :. Saved databases!");
	}
	else if(_wcsicmp(cmd, L"reload") == 0)
	{
		*parsed = true;
		bayes.LoadDatabases();
		//Print(".: SpamFilter :. Loaded databases! Total spam tokens: %d, ham tokens: %d", bayes.GetSpamTokenCount(), bayes.GetHamTokenCount());
	}
	else if(_wcsicmp(cmd, L"set") == 0)
	{
		*parsed = true;
		if(strlen(param) == 0)
		{
			//Print(".: SpamFilter :. Options are currently: enabled: %s, logchat: %s, logblocks: %s, autosave: %s",
			//		(enabled ? "on" : "off"), (logchat ? "on" : "off"), (logblocks ? "on" : "off"), (autosave ? "on" : "off"));
			return;
		}
		char *buf = const_cast<char*>(param), *ctx = NULL, *seps = " ";
		char* token = strtok_s(buf, seps, &ctx);
		char* value = buf+strlen(token)+1;
		int len = strlen(value);
		if(len > 1)
		{
			if(_stricmp(token, "enabled") == 0)
			{
				Toggles["Enabled"].state = StringToBool(value);
				//Print(".: SpamFilter :. SpamFilter is now %s", (enabled ? "enabled" : "disabled"));
			}
			else if(_stricmp(token, "logchat") == 0)
			{
				Toggles["Log Chat"].state = StringToBool(value);
				//Print(".: SpamFilter :. SpamFilter is now %slogging chat", (logchat ? "" : "not "));
			}
			else if(_stricmp(token, "logblocks") == 0)
			{
				Toggles["Log Blocks"].state = StringToBool(value);
				//Print(".: SpamFilter :. SpamFilter is now %slogging blocked messages (though chat logging may still be off!)", (logblocks ? "" : "not "));
			}
			else if(_stricmp(token, "autosave") == 0)
			{
				Toggles["Autosave"].state = StringToBool(value);
				//Print(".: SpamFilter :. SpamFilter is now %sautosaving the databases", (autosave ? "" : "not "));
			}
			//else Print(".: SpamFilter :. Unknown option!");
		}
	}
	else if(_wcsicmp(cmd, L"help") == 0)
	{
		*parsed = true;
		/*Print(".: SpamFilter :. Options");
		Print(".: SpamFilter :. .reload - Reload the spam and ham databases");
		Print(".: SpamFilter :. .save - Save the spam and ham databases");
		Print(".: SpamFilter :. .update - Check to see if there is a spam/ham database update");
		Print(".: SpamFilter :. .spam <string> - Add <string> to the spam database");
		Print(".: SpamFilter :. .ham <string> - Add <string> to the ham database");
		Print(".: SpamFilter :. .set <param> <value> - Set various parameters (true, TRUE, 1, and on are all enabled, false, FALSE, 0, and off are all disabled)");
		Print(".: SpamFilter :. Parameters for .set:");
		Print(".: SpamFilter :. enabled - Enable or disable spam blocking (default: true)");
		Print(".: SpamFilter :. logchat - Enable or disable chat logging (default: true)");
		Print(".: SpamFilter :. logblocks - Enable or disable logging blocked chat messages (default: false)");
		Print(".: SpamFilter :. autosave - Enable or disable auto-saving the spam and ham databases (default: true)");*/
	}
}

void SpamFilter::OnChatMsg(const char* szAcc, const char* szText, bool fromGame, bool* block)
{
	time_t currentTime;
	time(&currentTime);
	char szTime[60] = "";
	struct tm time;
	localtime_s(&time, &currentTime);
	strftime(szTime, sizeof(szTime), "%x %X", &time);

	int cat = bayes.Categorize(szText);
	*block = cat == -1;

	if(Toggles["Log Chat"].state && (!block || (block && Toggles["Log Blocks"].state)))
	{
		FILE* chatlog = NULL;
		fopen_s(&chatlog, log.c_str(), "a+");
		char* pos = strchr(const_cast<char*>(szAcc), '*');
		if(pos != NULL && pos != szAcc)
			*pos = '\0';
		fprintf(chatlog, "[%s] %s%s: %s\n", szTime, (block ? "BLOCKING: " : ""), szAcc, szText);
		fclose(chatlog);
	}
}
