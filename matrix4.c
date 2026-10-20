#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <wininet.h> //downlod

#pragma comment(lib, "wininet.lib")

// speaking
void speak_text(void* message) {
    char cmd[512];
    wsprintf(cmd, "start /b powershell -WindowStyle Hidden -Command \"Add-Type -AssemblyName System.Speech; (New-Object System.Speech.Synthesis.SpeechSynthesizer).Speak('%s');\" > NUL 2>&1", (char*)message);
    system(cmd);
    _endthread();
}

// matrix but red
void spawn_matrix_window(void* dummy) {
    FILE* f = fopen("matrix_temp.bat", "w");
    if (f) {
        fprintf(f, "@echo off\n");
        fprintf(f, "color 0c\n");
        fprintf(f, ":start\n");
        fprintf(f, "echo %%random%% %%random%% %%random%% %%random%% %%random%% %%random%% %%random%% %%random%%\n");
        fprintf(f, "goto start\n");
        fclose(f);
    }
    system("start matrix_temp.bat");
    _endthread();
}

// dir /s
void spawn_password_window(void* dummy) {
    FILE* f = fopen("pass_temp.bat", "w");
    if (f) {
        fprintf(f, "@echo off\n");
        fprintf(f, "color 0c\n"); 
        fprintf(f, "echo fetching passwords...\n");
        fprintf(f, "ping 1.1.1.1 -n 2 > nul\n"); 
        fprintf(f, "dir /s c:\\\n");
        fclose(f);
    }
    system("start pass_temp.bat");
    _endthread();
}

//wpp
void change_wallpaper() { //uh if you wanna changve the image you can do it here point it to an image
    const char* url = "https://raw.githubusercontent.com/le8pold/prankware/main/image(5).jpg";
    char path[MAX_PATH];
    
    // temp fo;lder
    GetTempPathA(MAX_PATH, path);
    strcat(path, "prank_bg.jpg");

    // download with powershell
    char cmd[1024];
    wsprintf(cmd, "powershell -WindowStyle Hidden -Command \"Invoke-WebRequest -Uri '%s' -OutFile '%s'\" > NUL 2>&1", url, path);
    system(cmd);

    // make the bg
    SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, (void*)path, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
}

// hidden in bg
unsigned __stdcall delayed_prank(void* param) {
    Sleep(10000); // 10 secs
    
    change_wallpaper();

    //im not using matrix i like dir /s better but uncomment for matrix
    //_beginthread(spawn_matrix_window, 0, NULL);
    _beginthread(spawn_password_window, 0, NULL);

    MessageBox(NULL, "You didn't fully get rid of me... I will forever be in ur files now", "System Alert", MB_OK | MB_ICONERROR);
    
    // clean
    Sleep(3000);
    system("del matrix_temp.bat pass_temp.bat > NUL 2>&1");

    return 0;
}
int main() {
    // timer
    HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, delayed_prank, NULL, 0, NULL);

    // init voice 
    char* initial_msg = "You have been hacked by leopold";
    _beginthread(speak_text, 0, (void*)initial_msg);

    // first popup
    MessageBox(NULL, "You've been hacked lol", "System Warning", MB_OK | MB_ICONWARNING);

    //waiting until timer done then kablouey
    if (hThread != NULL) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }

    return 0;
}
