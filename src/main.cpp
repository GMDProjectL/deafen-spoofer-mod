#include <Geode/Geode.hpp>
#include <Geode/modify/AppDelegate.hpp>
#include "spooferapi.hpp"

using namespace geode::prelude;

intptr_t server_handle = 0;

typedef void (WINAPI* keybd_event_t)(BYTE bVk, BYTE bScan, DWORD dwFlags, ULONG_PTR dwExtraInfo);
keybd_event_t original_keybd_event = nullptr;


void WINAPI hooked_keybd_event(BYTE bVk, BYTE bScan, DWORD dwFlags, ULONG_PTR dwExtraInfo) {
    if (bVk == VK_OEM_5) {
        if (!(dwFlags & KEYEVENTF_KEYUP)) {
            log::info("Backslash key detected! Virtual Key: {:#04x}, Scan Code: {:#04x}, Flags: {:#08x}", 
                     bVk, bScan, dwFlags);
			
			SpooferAPI::get()->toggleDeafen();
		}
    }
    
    if (original_keybd_event) {
        original_keybd_event(bVk, bScan, dwFlags, dwExtraInfo);
    }
}

void hook_keybd_event() {
	HMODULE user32 = GetModuleHandleA("user32.dll");
	void* keybd_event_addr = GetProcAddress(user32, "keybd_event");

	original_keybd_event = reinterpret_cast<keybd_event_t>(keybd_event_addr);

	auto result = Mod::get()->hook(
        keybd_event_addr,                        
        &hooked_keybd_event,                      
        "keybd_event",                       
        tulip::hook::TulipConvention::Stdcall 
    );
}

$execute {
	SpooferAPI::get()->init();

	hook_keybd_event();
}