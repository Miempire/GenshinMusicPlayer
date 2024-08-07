#include <iostream>  
#include <string>  
#include <vector>  
#include <thread>  
#include <Windows.h>  
  
struct HND {  
    std::string windowName;  
    std::string className;  
    HWND hwnd;  
};  
  
HND GetHandle(const std::string& parentClassName, const std::string& parentWindowName, const std::string& windowName, const std::string& windowClassName, bool isControl) {  
    HWND hwnd = FindWindow(windowClassName.c_str(), windowName.c_str());  
    if (hwnd == NULL && !parentClassName.empty()) {  
        hwnd = FindWindow(parentClassName.c_str(), parentWindowName.c_str());  
        if (hwnd != NULL && isControl) {  
            hwnd = FindWindowEx(hwnd, NULL, windowClassName.c_str(), windowName.c_str());  
        }  
    }  
    HND result;
    
    
    result.hwnd = hwnd;  
    if (hwnd != NULL) {  
        GetClassName(hwnd, (LPSTR)result.className.c_str(), 256);  
        GetWindowText(hwnd, (LPSTR)result.windowName.c_str(), 256);  
    }  
    return result;  
}  
  
void SimulateKeyPress(const std::string& key) {  
    // Simplified key simulation, assuming key is a single character  
    if (key.length() == 1) {  
        keybd_event(VkKeyScan(key[0]), 0, 0, 0);  
        Sleep(10);  
        keybd_event(VkKeyScan(key[0]), 0, KEYEVENTF_KEYUP, 0);  
    }  
}  
  
void Play(std::vector<std::string>& playlist, int bpm, int delayAdjustment) {  
    int delay = static_cast<int>(60000.0 / bpm - delayAdjustment);  
    for (size_t i = 0; i < playlist.size(); ++i) {  
        std::cout << "Playing line " << i + 1 << std::endl;  
        for (size_t j = 0; j < playlist[i].length(); ++j) {  
            char c = playlist[i][j];  
            if (isalpha(c)) {  
                SimulateKeyPress(std::string(1, toupper(c)));  
                Sleep(delay);  
            } else if (c == '(') {  
                size_t endPos = playlist[i].find(')', j);  
                std::string chord = playlist[i].substr(j + 1, endPos - j - 1);  
                SimulateKeyPress(chord);  
                Sleep(delay);  
                j = endPos;  
            } else if (c == ' ') {  
                Sleep(delay);  
            }  
            // Handle other special characters similarly  
        }  
        Sleep(100); // Additional delay between lines  
    }  
}  
  
int main() {  
    HND hwnd = GetHandle("cloud game player", "", "", "Qt5151QWindowIcon", true);  
    std::cout << "Window name: " << hwnd.windowName << std::endl;  
  
    std::string playlistText = "AASDKSJ"; // Placeholder for actual playlist text  
    std::vector<std::string> playlist;  
    // Split playlistText into lines and store in playlist vector  
    size_t prevPos = 0, pos;  
    while ((pos = playlistText.find('\n', prevPos)) != std::string::npos) {  
        playlist.push_back(playlistText.substr(prevPos, pos - prevPos));  
        prevPos = pos + 1;  
    }  
    playlist.push_back(playlistText.substr(prevPos));  
  
    int bpm = 120;  
    int delayAdjustment = 0;  
    std::thread playThread(Play, std::ref(playlist), bpm, delayAdjustment);  
    playThread.join();  
  
    return 0;  
}
