#include <stdio.h>    // for fopen, popen etc.
#include <stdlib.h>    // for bsearch
#include <ctype.h>    // for isdigit
#include <errno.h>    // for errno
#include <wx/filedlg.h>    // for wxFileSelector
#include "graphviz.h"

GraphViz::GraphViz(wxWindow *parent, const char *dot_path) :
    wxCanvas(parent),
    dot_changed(false),
    dot_data(NULL),
    xdot_data(NULL)
{
    int len = strlen(dot_path) + 1;
    this->dot_path = new char[len];
    memcpy(this->dot_path, dot_path, len);
}

GraphViz::~GraphViz()
{
    delete dot_path;
    if (dot_data != NULL)
        delete dot_data;
}

// __RAMARK__
// The system call on Windows causes a console window to quickly open
// and close while dot is being executed. This function avoids it by
// using Win32 API functions with the right parameters.
#ifdef __WIN32__
static int
execute(char *path)
{
    STARTUPINFO startup;
    startup.cb = sizeof(startup);
    startup.lpReserved = NULL;
    startup.lpDesktop = NULL;
    startup.lpTitle = NULL;
    startup.dwFlags = 0;
    startup.cbReserved2 = 0;
    startup.lpReserved2 = NULL;
    
    PROCESS_INFORMATION info;
    BOOL ok = CreateProcess(
        NULL,                        // pointer to name of executable module 
        path,                        // pointer to command line string
        NULL,                        // pointer to process security attributes 
        NULL,                        // pointer to thread security attributes 
        FALSE,                    // handle inheritance flag 
        DETACHED_PROCESS,            // creation flags 
        NULL,                        // pointer to new environment block 
        NULL,                        // pointer to current directory name 
        &startup,                    // pointer to STARTUPINFO 
        &info                        // pointer to PROCESS_INFORMATION  
    );
    if (ok == 0)
        return -1;
    
    DWORD exitcode;
    do {
        Sleep(1);
        if (GetExitCodeProcess(info.hProcess, &exitcode) == 0)
            return -1;
    } while (exitcode == STILL_ACTIVE);
    
    CloseHandle(info.hThread);
    CloseHandle(info.hProcess);
    return 0;
}
#else
#define execute system
#endif

void
GraphViz::Paint(wxDC& dc)
{
    // If the dot data was changed we have to
    // call dot to translate it to xdot and parse
    // the xdot data.
    if (dot_changed)
    {
#ifdef __WIN32__
        FILE *dot = fopen("C:\\temp.dot", "wb");
#else
        FILE *dot = fopen("/tmp/temp.dot", "wb");
#endif        
        fwrite(dot_data, 1, strlen(dot_data), dot);
        fclose(dot);
        char dot_app[1024];
        strcpy(dot_app, dot_path);
#ifdef __WIN32__
        strcat(dot_app, " -Txdot -y -oC:\\temp.xdot C:\\temp.dot");
#else
        // __BUG__
        // dot is segfaulting on x86 Fedora even when called via
        // the shell prompt.
        strcat(dot_app, " -Txdot -y -o/tmp/temp.xdot /tmp/temp.dot");
#endif
        if (execute(dot_app) == -1)
        {
            printf("ERROR: %s\n", strerror(errno));
            return;
        }
#ifdef __WIN32__
        dot = fopen("C:\\temp.xdot", "rb");
#else
        dot = fopen("/tmp/temp.xdot", "rb");
#endif
        if (dot == NULL)
        {
            printf("ERROR: %s\n", strerror(errno));
            return;
        }
        fseek(dot, 0, SEEK_END);
        long size = ftell(dot);
        if (xdot_data != NULL)
            delete xdot_data;
        xdot_data = new char[size + 1];
        fseek(dot, 0, SEEK_SET);
        fread(xdot_data, 1, size, dot);
        xdot_data[size] = '\0';
        fclose(dot);
        char *source = xdot_data;
        char *dest = xdot_data;
        do
        {
            if (source[0] == '\\')
                if (source[1] == '\n')
                    source += 2;
                else if (source[1] == '\r' && source[2] == '\n')
                    source += 3;
            *dest++ = *source++;
        } while (dest[-1] != '\0');
#ifdef __WIN32__
        unlink("C:\\temp.dot");
        unlink("C:\\temp.xdot");
#else
        unlink("/tmp/temp.dot");
        unlink("/tmp/temp.xdot");
#endif
        xdot.Parse(xdot_data);
        dot_changed = false;
    }
    // xdot caches drawing operations in
    // classes so we don't have to parse the
    // xdot data everytime.
    xdot.Execute(dc);
}

void
GraphViz::Open(const char *name)
{
    FILE *file = fopen(name, "rb");
    if (file == NULL)
    {
        return;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (dot_data != NULL)
        delete dot_data;
    dot_data = new char[size + 1];
    fread(dot_data, size, 1, file);
    fclose(file);
    dot_data[size] = '\0';
    dot_changed = true;
    Redraw(true);
}

void
GraphViz::SetDotData(const char *dot_data)
{
    if (this->dot_data != NULL)
        delete this->dot_data;
    int len = strlen(dot_data) + 1;
    this->dot_data = new char[len];
    memcpy(this->dot_data, dot_data, len);
    dot_changed = true;
    Redraw(true);
}

const char *
GraphViz::GetXDotData()
{
    return xdot_data;
}
