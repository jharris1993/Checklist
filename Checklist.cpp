#include <windows.h>
#include <vector>
#include <string>
#include <shellapi.h>
#include "resource.h"

// IDs for controls
// #define IDC_CONTINUE    101
// #define IDC_CHECKBOX    200

// Global Variables
HINSTANCE g_hInstance;
HWND g_hContinueButton;
std::vector<HWND> g_checkboxes;

// Function Prototypes
void UpdateContinueButtonState();

// Dialog Procedure
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_INITDIALOG: {
        // Set the dialog title
        // SetWindowText(hDlg, L"Flight Simulator");

        // Add heading
        HWND hHeading = CreateWindowW(
            L"STATIC", L"Flight Simulator Checklist",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            10, 10, 300, 20,
            hDlg, NULL, g_hInstance, NULL
        );

        // List of checklist items
        std::vector<std::wstring> items = { L"Item 1", L"Item 2", L"Item 3" };
        int yPos = 40;
        for (size_t i = 0; i < items.size(); ++i) {
            HWND hCheckbox = CreateWindowW(
                L"BUTTON", items[i].c_str(),
                WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                20, yPos, 280, 20,
                hDlg, (HMENU)(IDC_CHECKBOX + i), g_hInstance, NULL
            );
            g_checkboxes.push_back(hCheckbox);
            yPos += 30;
        }

        // Add Continue button
        g_hContinueButton = CreateWindowW(
            L"BUTTON", L"Continue",
            WS_CHILD | WS_VISIBLE | WS_DISABLED,
            100, yPos + 10, 120, 30,
            hDlg, (HMENU)IDC_CONTINUE, g_hInstance, NULL
        );
        return TRUE;
    }

    case WM_COMMAND: {
        // Handle checkbox clicks
        if (LOWORD(wParam) >= IDC_CHECKBOX && LOWORD(wParam) < IDC_CHECKBOX + g_checkboxes.size()) {
            UpdateContinueButtonState();
        }

        // Handle Continue button click
        if (LOWORD(wParam) == IDC_CONTINUE) {
            // Launch another program
            ShellExecuteW(NULL, L"open", L"notepad.exe", NULL, NULL, SW_SHOWNORMAL);

            // Exit dialog
            EndDialog(hDlg, 0);
        }
        break;
    }

    case WM_CLOSE:
        EndDialog(hDlg, 0);
        return TRUE;
    }
    return FALSE;
}

// Update the state of the Continue button
void UpdateContinueButtonState() {
    bool allChecked = true;
    for (HWND checkbox : g_checkboxes) {
        if (SendMessageW(checkbox, BM_GETCHECK, 0, 0) != BST_CHECKED) {
            allChecked = false;
            break;
        }
    }
    EnableWindow(g_hContinueButton, allChecked ? TRUE : FALSE);
}

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    g_hInstance = hInstance;

    DialogBoxParam(
        hInstance,
        MAKEINTRESOURCE(IDD_DIALOG1), // Replace with NULL if creating dynamically
        NULL,
        DialogProc,
        0
    );

    return 0;
}
