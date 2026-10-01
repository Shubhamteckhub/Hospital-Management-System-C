#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

/* Database Credentials */
#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASSWORD "abcd"    
#define DB_NAME "hospital_db23"
#define DB_PORT 3300                

/* Global Connection Handle */
MYSQL *conn = NULL;

/* Window Handles */
HWND hMainWnd = NULL;
HWND hPatientWnd = NULL;
HWND hDoctorWnd = NULL;
HWND hApptWnd = NULL;
HWND hBillWnd = NULL;

/* Control IDs - Dashboard */
#define ID_BTN_PATIENT     101
#define ID_BTN_DOCTOR      102
#define ID_BTN_APPOINTMENT 103
#define ID_BTN_BILLING     104
#define ID_BTN_EXIT        105

/* Control IDs - Patient Form */
#define ID_TXT_PAT_ID      201
#define ID_TXT_PAT_NAME    202
#define ID_TXT_PAT_AGE     203
#define ID_TXT_PAT_GENDER  204
#define ID_TXT_PAT_DISEASE 205
#define ID_BTN_PAT_ADD     206
#define ID_BTN_PAT_VIEW    207
#define ID_BTN_PAT_SEARCH  208
#define ID_BTN_PAT_UPDATE  209
#define ID_BTN_PAT_DELETE  210
#define ID_BTN_PAT_BACK    211
#define ID_TXT_PAT_OUTPUT  212

/* Control IDs - Doctor Form */
#define ID_TXT_DOC_ID      301
#define ID_TXT_DOC_NAME    302
#define ID_TXT_DOC_SPEC    303
#define ID_BTN_DOC_ADD     304
#define ID_BTN_DOC_VIEW    305
#define ID_BTN_DOC_SEARCH  306
#define ID_BTN_DOC_BACK    307
#define ID_TXT_DOC_OUTPUT  308

/* Control IDs - Appointment Form */
#define ID_TXT_APT_PAT_ID  401
#define ID_TXT_APT_DOC_ID  402
#define ID_TXT_APT_DATE    403
#define ID_BTN_APT_BOOK    404
#define ID_BTN_APT_VIEW    405
#define ID_BTN_APT_SEARCH  406
#define ID_BTN_APT_BACK    407
#define ID_TXT_APT_OUTPUT  408

/* Control IDs - Billing Form */
#define ID_TXT_BIL_PAT_ID  501
#define ID_TXT_BIL_FEE     502
#define ID_TXT_BIL_MED     503
#define ID_BTN_BIL_GEN     504
#define ID_BTN_BIL_VIEW    505
#define ID_BTN_BIL_SEARCH  506
#define ID_BTN_BIL_BACK    507
#define ID_TXT_BIL_OUTPUT  508

/* Edit Handles - Patient Form */
HWND hPatId, hPatName, hPatAge, hPatGender, hPatDisease, hPatOutput;

/* Edit Handles - Doctor Form */
HWND hDocId, hDocName, hDocSpec, hDocOutput;

/* Edit Handles - Appointment Form */
HWND hAptPatId, hAptDocId, hAptDate, hAptOutput;

/* Edit Handles - Billing Form */
HWND hBilPatId, hBilFee, hBilMed, hBilOutput;

/* Function Prototypes */
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PatientWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DoctorWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ApptWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK BillWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int connectDatabase(HWND hwnd);

void addPatientGUI(HWND hwnd);
void viewPatientsGUI(HWND hwnd);
void searchPatientGUI(HWND hwnd);
void updatePatientGUI(HWND hwnd);
void deletePatientGUI(HWND hwnd);

void addDoctorGUI(HWND hwnd);
void viewDoctorsGUI(HWND hwnd);
void searchDoctorGUI(HWND hwnd);

void bookApptGUI(HWND hwnd);
void viewApptsGUI(HWND hwnd);
void searchApptGUI(HWND hwnd);

void generateBillGUI(HWND hwnd);
void viewBillsGUI(HWND hwnd);
void searchBillGUI(HWND hwnd);

/* ========================================================================= */
/* DATABASE CONNECTION                                                       */
/* ========================================================================= */
int connectDatabase(HWND hwnd)
{
    if (conn != NULL) return 1;

    conn = mysql_init(NULL);
    if (conn == NULL)
    {
        MessageBox(hwnd, "MySQL Initialization Failed!", "Database Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0) == NULL)
    {
        char err[300];
        sprintf(err, "MySQL Connection Failed!\nError: %s", mysql_error(conn));
        MessageBox(hwnd, err, "Database Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    return 1;
}

/* ========================================================================= */
/* WINMAIN - APPLICATION ENTRY POINT                                        */
/* ========================================================================= */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    MSG Msg;

    /* 1. Register Main Dashboard Class */
    wc.cbSize = sizeof(WNDCLASSEX); wc.style = 0; wc.lpfnWndProc = MainWndProc;
    wc.cbClsExtra = 0; wc.cbWndExtra = 0; wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); wc.lpszMenuName = NULL;
    wc.lpszClassName = "MainDashboardClass"; wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&wc);

    /* 2. Register Patient Window Class */
    wc.lpfnWndProc = PatientWndProc; wc.lpszClassName = "PatientWindowClass";
    RegisterClassEx(&wc);

    /* 3. Register Doctor Window Class */
    wc.lpfnWndProc = DoctorWndProc; wc.lpszClassName = "DoctorWindowClass";
    RegisterClassEx(&wc);

    /* 4. Register Appointment Window Class */
    wc.lpfnWndProc = ApptWndProc; wc.lpszClassName = "ApptWindowClass";
    RegisterClassEx(&wc);

    /* 5. Register Billing Window Class */
    wc.lpfnWndProc = BillWndProc; wc.lpszClassName = "BillWindowClass";
    RegisterClassEx(&wc);

    /* Create Main Window */
    hMainWnd = CreateWindowEx(
        WS_EX_CLIENTEDGE, "MainDashboardClass",
        "Hospital Management System - Main Dashboard",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 550,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hMainWnd, nCmdShow);
    UpdateWindow(hMainWnd);

    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    if (conn) mysql_close(conn);
    return Msg.wParam;
}

/* ========================================================================= */
/* 1. DASHBOARD WINDOW PROCEDURE                                             */
/* ========================================================================= */
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            CreateWindow("STATIC", "HOSPITAL MANAGEMENT SYSTEM", WS_VISIBLE | WS_CHILD | SS_CENTER, 150, 30, 500, 30, hwnd, NULL, NULL, NULL);
            CreateWindow("STATIC", "Select a module to manage:", WS_VISIBLE | WS_CHILD | SS_CENTER, 150, 65, 500, 20, hwnd, NULL, NULL, NULL);

            CreateWindow("BUTTON", "1. Patient Management", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 200, 110, 400, 45, hwnd, (HMENU)ID_BTN_PATIENT, NULL, NULL);
            CreateWindow("BUTTON", "2. Doctor Management", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 200, 170, 400, 45, hwnd, (HMENU)ID_BTN_DOCTOR, NULL, NULL);
            CreateWindow("BUTTON", "3. Appointment Management", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 200, 230, 400, 45, hwnd, (HMENU)ID_BTN_APPOINTMENT, NULL, NULL);
            CreateWindow("BUTTON", "4. Billing Management", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 200, 290, 400, 45, hwnd, (HMENU)ID_BTN_BILLING, NULL, NULL);
            CreateWindow("BUTTON", "5. Exit System", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 200, 370, 400, 45, hwnd, (HMENU)ID_BTN_EXIT, NULL, NULL);
            break;
        }

        case WM_COMMAND:
        {
            if (!connectDatabase(hwnd)) break;

            switch (LOWORD(wParam))
            {
                case ID_BTN_PATIENT:
                    if (!hPatientWnd) hPatientWnd = CreateWindowEx(WS_EX_CLIENTEDGE, "PatientWindowClass", "Patient Management Module", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 750, 550, hwnd, NULL, GetModuleHandle(NULL), NULL);
                    ShowWindow(hMainWnd, SW_HIDE); ShowWindow(hPatientWnd, SW_SHOW);
                    break;

                case ID_BTN_DOCTOR:
                    if (!hDoctorWnd) hDoctorWnd = CreateWindowEx(WS_EX_CLIENTEDGE, "DoctorWindowClass", "Doctor Management Module", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 750, 550, hwnd, NULL, GetModuleHandle(NULL), NULL);
                    ShowWindow(hMainWnd, SW_HIDE); ShowWindow(hDoctorWnd, SW_SHOW);
                    break;

                case ID_BTN_APPOINTMENT:
                    if (!hApptWnd) hApptWnd = CreateWindowEx(WS_EX_CLIENTEDGE, "ApptWindowClass", "Appointment Management Module", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 750, 550, hwnd, NULL, GetModuleHandle(NULL), NULL);
                    ShowWindow(hMainWnd, SW_HIDE); ShowWindow(hApptWnd, SW_SHOW);
                    break;

                case ID_BTN_BILLING:
                    if (!hBillWnd) hBillWnd = CreateWindowEx(WS_EX_CLIENTEDGE, "BillWindowClass", "Billing Management Module", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 750, 550, hwnd, NULL, GetModuleHandle(NULL), NULL);
                    ShowWindow(hMainWnd, SW_HIDE); ShowWindow(hBillWnd, SW_SHOW);
                    break;

                case ID_BTN_EXIT:
                    PostQuitMessage(0);
                    break;
            }
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

/* ========================================================================= */
/* 2. PATIENT MANAGEMENT FORM PROCEDURE                                     */
/* ========================================================================= */
LRESULT CALLBACK PatientWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            CreateWindow("STATIC", "PATIENT MANAGEMENT MODULE", WS_VISIBLE | WS_CHILD | SS_CENTER, 50, 15, 630, 25, hwnd, NULL, NULL, NULL);

            CreateWindow("STATIC", "Patient ID:", WS_VISIBLE | WS_CHILD, 30, 55, 100, 20, hwnd, NULL, NULL, NULL);
            hPatId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 140, 55, 150, 25, hwnd, (HMENU)ID_TXT_PAT_ID, NULL, NULL);

            CreateWindow("STATIC", "Name:", WS_VISIBLE | WS_CHILD, 30, 90, 100, 20, hwnd, NULL, NULL, NULL);
            hPatName = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 140, 90, 150, 25, hwnd, (HMENU)ID_TXT_PAT_NAME, NULL, NULL);

            CreateWindow("STATIC", "Age:", WS_VISIBLE | WS_CHILD, 30, 125, 100, 20, hwnd, NULL, NULL, NULL);
            hPatAge = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 140, 125, 150, 25, hwnd, (HMENU)ID_TXT_PAT_AGE, NULL, NULL);

            CreateWindow("STATIC", "Gender:", WS_VISIBLE | WS_CHILD, 30, 160, 100, 20, hwnd, NULL, NULL, NULL);
            hPatGender = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 140, 160, 150, 25, hwnd, (HMENU)ID_TXT_PAT_GENDER, NULL, NULL);

            CreateWindow("STATIC", "Disease:", WS_VISIBLE | WS_CHILD, 30, 195, 100, 20, hwnd, NULL, NULL, NULL);
            hPatDisease = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 140, 195, 150, 25, hwnd, (HMENU)ID_TXT_PAT_DISEASE, NULL, NULL);

            CreateWindow("BUTTON", "Add Patient", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 310, 55, 120, 30, hwnd, (HMENU)ID_BTN_PAT_ADD, NULL, NULL);
            CreateWindow("BUTTON", "View Patients", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 440, 55, 120, 30, hwnd, (HMENU)ID_BTN_PAT_VIEW, NULL, NULL);
            CreateWindow("BUTTON", "Search Patient", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 310, 95, 120, 30, hwnd, (HMENU)ID_BTN_PAT_SEARCH, NULL, NULL);
            CreateWindow("BUTTON", "Update Patient", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 440, 95, 120, 30, hwnd, (HMENU)ID_BTN_PAT_UPDATE, NULL, NULL);
            CreateWindow("BUTTON", "Delete Patient", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 310, 135, 120, 30, hwnd, (HMENU)ID_BTN_PAT_DELETE, NULL, NULL);
            CreateWindow("BUTTON", "<-- Back", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 440, 135, 120, 30, hwnd, (HMENU)ID_BTN_PAT_BACK, NULL, NULL);

            CreateWindow("STATIC", "Patient Database Display:", WS_VISIBLE | WS_CHILD, 30, 230, 300, 20, hwnd, NULL, NULL, NULL);
            hPatOutput = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY, 30, 255, 670, 220, hwnd, (HMENU)ID_TXT_PAT_OUTPUT, NULL, NULL);
            break;
        }

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case ID_BTN_PAT_ADD: addPatientGUI(hwnd); break;
                case ID_BTN_PAT_VIEW: viewPatientsGUI(hwnd); break;
                case ID_BTN_PAT_SEARCH: searchPatientGUI(hwnd); break;
                case ID_BTN_PAT_UPDATE: updatePatientGUI(hwnd); break;
                case ID_BTN_PAT_DELETE: deletePatientGUI(hwnd); break;
                case ID_BTN_PAT_BACK: ShowWindow(hPatientWnd, SW_HIDE); ShowWindow(hMainWnd, SW_SHOW); break;
            }
            break;
        }

        case WM_CLOSE:
            ShowWindow(hPatientWnd, SW_HIDE); ShowWindow(hMainWnd, SW_SHOW); break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

/* ========================================================================= */
/* 3. DOCTOR MANAGEMENT FORM PROCEDURE                                      */
/* ========================================================================= */
LRESULT CALLBACK DoctorWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            CreateWindow("STATIC", "DOCTOR MANAGEMENT MODULE", WS_VISIBLE | WS_CHILD | SS_CENTER, 50, 15, 630, 25, hwnd, NULL, NULL, NULL);

            CreateWindow("STATIC", "Doctor ID:", WS_VISIBLE | WS_CHILD, 30, 60, 120, 20, hwnd, NULL, NULL, NULL);
            hDocId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 160, 60, 150, 25, hwnd, (HMENU)ID_TXT_DOC_ID, NULL, NULL);

            CreateWindow("STATIC", "Doctor Name:", WS_VISIBLE | WS_CHILD, 30, 100, 120, 20, hwnd, NULL, NULL, NULL);
            hDocName = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 100, 150, 25, hwnd, (HMENU)ID_TXT_DOC_NAME, NULL, NULL);

            CreateWindow("STATIC", "Specialization:", WS_VISIBLE | WS_CHILD, 30, 140, 120, 20, hwnd, NULL, NULL, NULL);
            hDocSpec = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 140, 150, 25, hwnd, (HMENU)ID_TXT_DOC_SPEC, NULL, NULL);

            CreateWindow("BUTTON", "Add Doctor", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 340, 60, 130, 30, hwnd, (HMENU)ID_BTN_DOC_ADD, NULL, NULL);
            CreateWindow("BUTTON", "View Doctors", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 480, 60, 130, 30, hwnd, (HMENU)ID_BTN_DOC_VIEW, NULL, NULL);
            CreateWindow("BUTTON", "Search Doctor", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 340, 100, 130, 30, hwnd, (HMENU)ID_BTN_DOC_SEARCH, NULL, NULL);
            CreateWindow("BUTTON", "<-- Back", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 480, 100, 130, 30, hwnd, (HMENU)ID_BTN_DOC_BACK, NULL, NULL);

            CreateWindow("STATIC", "Doctor Database Display:", WS_VISIBLE | WS_CHILD, 30, 190, 300, 20, hwnd, NULL, NULL, NULL);
            hDocOutput = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY, 30, 215, 670, 260, hwnd, (HMENU)ID_TXT_DOC_OUTPUT, NULL, NULL);
            break;
        }

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case ID_BTN_DOC_ADD: addDoctorGUI(hwnd); break;
                case ID_BTN_DOC_VIEW: viewDoctorsGUI(hwnd); break;
                case ID_BTN_DOC_SEARCH: searchDoctorGUI(hwnd); break;
                case ID_BTN_DOC_BACK: ShowWindow(hDoctorWnd, SW_HIDE); ShowWindow(hMainWnd, SW_SHOW); break;
            }
            break;
        }

        case WM_CLOSE:
            ShowWindow(hDoctorWnd, SW_HIDE); ShowWindow(hMainWnd, SW_SHOW); break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

/* ========================================================================= */
/* 4. APPOINTMENT MANAGEMENT FORM PROCEDURE                                  */
/* ========================================================================= */
LRESULT CALLBACK ApptWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            CreateWindow("STATIC", "APPOINTMENT MANAGEMENT MODULE", WS_VISIBLE | WS_CHILD | SS_CENTER, 50, 15, 630, 25, hwnd, NULL, NULL, NULL);

            CreateWindow("STATIC", "Patient ID:", WS_VISIBLE | WS_CHILD, 30, 60, 120, 20, hwnd, NULL, NULL, NULL);
            hAptPatId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 160, 60, 150, 25, hwnd, (HMENU)ID_TXT_APT_PAT_ID, NULL, NULL);

            CreateWindow("STATIC", "Doctor ID:", WS_VISIBLE | WS_CHILD, 30, 100, 120, 20, hwnd, NULL, NULL, NULL);
            hAptDocId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 160, 100, 150, 25, hwnd, (HMENU)ID_TXT_APT_DOC_ID, NULL, NULL);

            CreateWindow("STATIC", "Date (YYYY-MM-DD):", WS_VISIBLE | WS_CHILD, 30, 140, 130, 20, hwnd, NULL, NULL, NULL);
            hAptDate = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 140, 150, 25, hwnd, (HMENU)ID_TXT_APT_DATE, NULL, NULL);

            CreateWindow("BUTTON", "Book Appointment", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 340, 60, 150, 30, hwnd, (HMENU)ID_BTN_APT_BOOK, NULL, NULL);
            CreateWindow("BUTTON", "View Appointments", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 500, 60, 150, 30, hwnd, (HMENU)ID_BTN_APT_VIEW, NULL, NULL);
            CreateWindow("BUTTON", "Search by Patient ID", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 340, 100, 150, 30, hwnd, (HMENU)ID_BTN_APT_SEARCH, NULL, NULL);
            CreateWindow("BUTTON", "<-- Back", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 500, 100, 150, 30, hwnd, (HMENU)ID_BTN_APT_BACK, NULL, NULL);

            CreateWindow("STATIC", "Appointment Records Display (SQL JOIN):", WS_VISIBLE | WS_CHILD, 30, 190, 350, 20, hwnd, NULL, NULL, NULL);
            hAptOutput = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY, 30, 215, 670, 260, hwnd, (HMENU)ID_TXT_APT_OUTPUT, NULL, NULL);
            break;
        }

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case ID_BTN_APT_BOOK: bookApptGUI(hwnd); break;
                case ID_BTN_APT_VIEW: viewApptsGUI(hwnd); break;
                case ID_BTN_APT_SEARCH: searchApptGUI(hwnd); break;
                case ID_BTN_APT_BACK: ShowWindow(hApptWnd, SW_HIDE); ShowWindow(hMainWnd, SW_SHOW); break;
            }
            break;
        }

        case WM_CLOSE:
            ShowWindow(hApptWnd, SW_HIDE); ShowWindow(hMainWnd, SW_SHOW); break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

/* ========================================================================= */
/* 5. BILLING MANAGEMENT FORM PROCEDURE                                     */
/* ========================================================================= */
LRESULT CALLBACK BillWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            CreateWindow("STATIC", "BILLING MANAGEMENT MODULE", WS_VISIBLE | WS_CHILD | SS_CENTER, 50, 15, 630, 25, hwnd, NULL, NULL, NULL);

            CreateWindow("STATIC", "Patient ID:", WS_VISIBLE | WS_CHILD, 30, 60, 130, 20, hwnd, NULL, NULL, NULL);
            hBilPatId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 170, 60, 150, 25, hwnd, (HMENU)ID_TXT_BIL_PAT_ID, NULL, NULL);

            CreateWindow("STATIC", "Consultation Fee:", WS_VISIBLE | WS_CHILD, 30, 100, 130, 20, hwnd, NULL, NULL, NULL);
            hBilFee = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 170, 100, 150, 25, hwnd, (HMENU)ID_TXT_BIL_FEE, NULL, NULL);

            CreateWindow("STATIC", "Medicine Charge:", WS_VISIBLE | WS_CHILD, 30, 140, 130, 20, hwnd, NULL, NULL, NULL);
            hBilMed = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 170, 140, 150, 25, hwnd, (HMENU)ID_TXT_BIL_MED, NULL, NULL);

            CreateWindow("BUTTON", "Generate Bill", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 340, 60, 140, 30, hwnd, (HMENU)ID_BTN_BIL_GEN, NULL, NULL);
            CreateWindow("BUTTON", "View All Bills", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 490, 60, 140, 30, hwnd, (HMENU)ID_BTN_BIL_VIEW, NULL, NULL);
            CreateWindow("BUTTON", "Search Bill", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 340, 100, 140, 30, hwnd, (HMENU)ID_BTN_BIL_SEARCH, NULL, NULL);
            CreateWindow("BUTTON", "<-- Back", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 490, 100, 140, 30, hwnd, (HMENU)ID_BTN_BIL_BACK, NULL, NULL);

            CreateWindow("STATIC", "Billing Records Display (SQL JOIN):", WS_VISIBLE | WS_CHILD, 30, 190, 350, 20, hwnd, NULL, NULL, NULL);
            hBilOutput = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY, 30, 215, 670, 260, hwnd, (HMENU)ID_TXT_BIL_OUTPUT, NULL, NULL);
            break;
        }

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case ID_BTN_BIL_GEN: generateBillGUI(hwnd); break;
                case ID_BTN_BIL_VIEW: viewBillsGUI(hwnd); break;
                case ID_BTN_BIL_SEARCH: searchBillGUI(hwnd); break;
                case ID_BTN_BIL_BACK: ShowWindow(hBillWnd, SW_HIDE); ShowWindow(hMainWnd, SW_SHOW); break;
            }
            break;
        }

        case WM_CLOSE:
            ShowWindow(hBillWnd, SW_HIDE); ShowWindow(hMainWnd, SW_SHOW); break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

/* ========================================================================= */
/* PATIENT OPERATIONS                                                        */
/* ========================================================================= */
void addPatientGUI(HWND hwnd)
{
    char idStr[20], name[100], ageStr[20], gender[20], disease[100], query[500];
    GetWindowText(hPatId, idStr, sizeof(idStr)); GetWindowText(hPatName, name, sizeof(name));
    GetWindowText(hPatAge, ageStr, sizeof(ageStr)); GetWindowText(hPatGender, gender, sizeof(gender));
    GetWindowText(hPatDisease, disease, sizeof(disease));

    if (!strlen(idStr) || !strlen(name) || !strlen(ageStr)) { MessageBox(hwnd, "Fill ID, Name, Age!", "Error", MB_OK | MB_ICONWARNING); return; }

    sprintf(query, "INSERT INTO patients VALUES (%d, '%s', %d, '%s', '%s')", atoi(idStr), name, atoi(ageStr), gender, disease);
    if (mysql_query(conn, query) == 0)
    {
        MessageBox(hwnd, "[SUCCESS] Patient Added!", "Success", MB_OK | MB_ICONINFORMATION);
        SetWindowText(hPatId, ""); SetWindowText(hPatName, ""); SetWindowText(hPatAge, ""); SetWindowText(hPatGender, ""); SetWindowText(hPatDisease, "");
        viewPatientsGUI(hwnd);
    }
    else MessageBox(hwnd, mysql_error(conn), "Error", MB_OK | MB_ICONERROR);
}

void viewPatientsGUI(HWND hwnd)
{
    MYSQL_RES *res; MYSQL_ROW row; char buffer[4000] = "", line[250];
    if (mysql_query(conn, "SELECT id, name, age, gender, disease FROM patients") != 0) return;
    res = mysql_store_result(conn); if (!res) return;

    sprintf(line, "%-8s %-20s %-8s %-10s %-20s\r\n----------------------------------------------------------------------------------\r\n", "ID", "NAME", "AGE", "GENDER", "DISEASE");
    strcat(buffer, line);
    while ((row = mysql_fetch_row(res)))
    {
        sprintf(line, "%-8s %-20s %-8s %-10s %-20s\r\n", row[0], row[1], row[2], row[3], row[4]);
        strcat(buffer, line);
    }
    SetWindowText(hPatOutput, buffer); mysql_free_result(res);
}

void searchPatientGUI(HWND hwnd)
{
    char idStr[20], query[300]; GetWindowText(hPatId, idStr, sizeof(idStr));
    if (!strlen(idStr)) { MessageBox(hwnd, "Enter Patient ID!", "Warning", MB_OK | MB_ICONWARNING); return; }

    sprintf(query, "SELECT id, name, age, gender, disease FROM patients WHERE id=%d", atoi(idStr));
    if (mysql_query(conn, query) != 0) return;

    MYSQL_RES *res = mysql_store_result(conn); MYSQL_ROW row = mysql_fetch_row(res);
    if (row)
    {
        SetWindowText(hPatName, row[1]); SetWindowText(hPatAge, row[2]); SetWindowText(hPatGender, row[3]); SetWindowText(hPatDisease, row[4]);
        MessageBox(hwnd, "Patient Found!", "Success", MB_OK | MB_ICONINFORMATION);
    }
    else MessageBox(hwnd, "Patient Not Found!", "Info", MB_OK | MB_ICONWARNING);
    mysql_free_result(res);
}

void updatePatientGUI(HWND hwnd)
{
    char idStr[20], disease[100], query[300];
    GetWindowText(hPatId, idStr, sizeof(idStr)); GetWindowText(hPatDisease, disease, sizeof(disease));
    if (!strlen(idStr) || !strlen(disease)) return;

    sprintf(query, "UPDATE patients SET disease='%s' WHERE id=%d", disease, atoi(idStr));
    if (mysql_query(conn, query) == 0 && mysql_affected_rows(conn) > 0)
    { MessageBox(hwnd, "Patient Updated!", "Success", MB_OK | MB_ICONINFORMATION); viewPatientsGUI(hwnd); }
    else MessageBox(hwnd, "Patient ID Not Found!", "Error", MB_OK | MB_ICONERROR);
}

void deletePatientGUI(HWND hwnd)
{
    char idStr[20], query[300]; GetWindowText(hPatId, idStr, sizeof(idStr));
    if (!strlen(idStr)) return;

    sprintf(query, "DELETE FROM patients WHERE id=%d", atoi(idStr));
    if (mysql_query(conn, query) == 0 && mysql_affected_rows(conn) > 0)
    { MessageBox(hwnd, "Patient Deleted!", "Success", MB_OK | MB_ICONINFORMATION); viewPatientsGUI(hwnd); }
    else MessageBox(hwnd, "Patient ID Not Found!", "Error", MB_OK | MB_ICONERROR);
}

/* ========================================================================= */
/* DOCTOR OPERATIONS                                                         */
/* ========================================================================= */
void addDoctorGUI(HWND hwnd)
{
    char idStr[20], name[100], spec[100], query[500];
    GetWindowText(hDocId, idStr, sizeof(idStr)); GetWindowText(hDocName, name, sizeof(name)); GetWindowText(hDocSpec, spec, sizeof(spec));

    if (!strlen(idStr) || !strlen(name)) { MessageBox(hwnd, "Fill Doctor ID and Name!", "Error", MB_OK | MB_ICONWARNING); return; }

    sprintf(query, "INSERT INTO doctors VALUES (%d, '%s', '%s')", atoi(idStr), name, spec);
    if (mysql_query(conn, query) == 0)
    {
        MessageBox(hwnd, "[SUCCESS] Doctor Added!", "Success", MB_OK | MB_ICONINFORMATION);
        SetWindowText(hDocId, ""); SetWindowText(hDocName, ""); SetWindowText(hDocSpec, "");
        viewDoctorsGUI(hwnd);
    }
    else MessageBox(hwnd, mysql_error(conn), "Error", MB_OK | MB_ICONERROR);
}

void viewDoctorsGUI(HWND hwnd)
{
    MYSQL_RES *res; MYSQL_ROW row; char buffer[4000] = "", line[250];
    if (mysql_query(conn, "SELECT id, name, specialization FROM doctors") != 0) return;
    res = mysql_store_result(conn); if (!res) return;

    sprintf(line, "%-8s %-25s %-25s\r\n----------------------------------------------------------------------------------\r\n", "ID", "DOCTOR NAME", "SPECIALIZATION");
    strcat(buffer, line);
    while ((row = mysql_fetch_row(res)))
    {
        sprintf(line, "%-8s %-25s %-25s\r\n", row[0], row[1], row[2]);
        strcat(buffer, line);
    }
    SetWindowText(hDocOutput, buffer); mysql_free_result(res);
}

void searchDoctorGUI(HWND hwnd)
{
    char idStr[20], query[300]; GetWindowText(hDocId, idStr, sizeof(idStr));
    if (!strlen(idStr)) { MessageBox(hwnd, "Enter Doctor ID!", "Warning", MB_OK | MB_ICONWARNING); return; }

    sprintf(query, "SELECT id, name, specialization FROM doctors WHERE id=%d", atoi(idStr));
    if (mysql_query(conn, query) != 0) return;

    MYSQL_RES *res = mysql_store_result(conn); MYSQL_ROW row = mysql_fetch_row(res);
    if (row)
    {
        SetWindowText(hDocName, row[1]); SetWindowText(hDocSpec, row[2]);
        MessageBox(hwnd, "Doctor Found!", "Success", MB_OK | MB_ICONINFORMATION);
    }
    else MessageBox(hwnd, "Doctor Not Found!", "Info", MB_OK | MB_ICONWARNING);
    mysql_free_result(res);
}

/* ========================================================================= */
/* APPOINTMENT OPERATIONS                                                    */
/* ========================================================================= */
void bookApptGUI(HWND hwnd)
{
    char pIdStr[20], dIdStr[20], date[30], query[500];
    GetWindowText(hAptPatId, pIdStr, sizeof(pIdStr)); GetWindowText(hAptDocId, dIdStr, sizeof(dIdStr)); GetWindowText(hAptDate, date, sizeof(date));

    if (!strlen(pIdStr) || !strlen(dIdStr) || !strlen(date)) { MessageBox(hwnd, "Fill Patient ID, Doctor ID, Date!", "Error", MB_OK | MB_ICONWARNING); return; }

    sprintf(query, "INSERT INTO appointments (patient_id, doctor_id, appointment_date) VALUES (%d, %d, '%s')", atoi(pIdStr), atoi(dIdStr), date);
    if (mysql_query(conn, query) == 0)
    {
        MessageBox(hwnd, "[SUCCESS] Appointment Booked Successfully!", "Success", MB_OK | MB_ICONINFORMATION);
        SetWindowText(hAptPatId, ""); SetWindowText(hAptDocId, ""); SetWindowText(hAptDate, "");
        viewApptsGUI(hwnd);
    }
    else MessageBox(hwnd, mysql_error(conn), "Error", MB_OK | MB_ICONERROR);
}

void viewApptsGUI(HWND hwnd)
{
    MYSQL_RES *res; MYSQL_ROW row; char buffer[4000] = "", line[250];
    char query[] = "SELECT appointments.id, patients.name, doctors.name, appointments.appointment_date "
                   "FROM appointments "
                   "JOIN patients ON appointments.patient_id = patients.id "
                   "JOIN doctors ON appointments.doctor_id = doctors.id";

    if (mysql_query(conn, query) != 0) return;
    res = mysql_store_result(conn); if (!res) return;

    sprintf(line, "%-8s %-20s %-20s %-15s\r\n----------------------------------------------------------------------------------\r\n", "APPT ID", "PATIENT NAME", "DOCTOR NAME", "DATE");
    strcat(buffer, line);
    while ((row = mysql_fetch_row(res)))
    {
        sprintf(line, "%-8s %-20s %-20s %-15s\r\n", row[0], row[1], row[2], row[3]);
        strcat(buffer, line);
    }
    SetWindowText(hAptOutput, buffer); mysql_free_result(res);
}

void searchApptGUI(HWND hwnd)
{
    char pIdStr[20], query[500]; GetWindowText(hAptPatId, pIdStr, sizeof(pIdStr));
    if (!strlen(pIdStr)) { MessageBox(hwnd, "Enter Patient ID!", "Warning", MB_OK | MB_ICONWARNING); return; }

    sprintf(query, "SELECT appointments.id, patients.name, doctors.name, appointments.appointment_date "
                   "FROM appointments "
                   "JOIN patients ON appointments.patient_id = patients.id "
                   "JOIN doctors ON appointments.doctor_id = doctors.id "
                   "WHERE appointments.patient_id = %d", atoi(pIdStr));

    if (mysql_query(conn, query) != 0) return;
    MYSQL_RES *res = mysql_store_result(conn); MYSQL_ROW row;
    char buffer[4000] = "", line[250];

    sprintf(line, "%-8s %-20s %-20s %-15s\r\n----------------------------------------------------------------------------------\r\n", "APPT ID", "PATIENT NAME", "DOCTOR NAME", "DATE");
    strcat(buffer, line);
    int count = 0;
    while ((row = mysql_fetch_row(res)))
    {
        sprintf(line, "%-8s %-20s %-20s %-15s\r\n", row[0], row[1], row[2], row[3]);
        strcat(buffer, line); count++;
    }
    if (count == 0) strcat(buffer, "No appointments found for this Patient ID.\r\n");
    SetWindowText(hAptOutput, buffer); mysql_free_result(res);
}

/* ========================================================================= */
/* BILLING OPERATIONS                                                        */
/* ========================================================================= */
void generateBillGUI(HWND hwnd)
{
    char pIdStr[20], feeStr[20], medStr[20], query[500];
    GetWindowText(hBilPatId, pIdStr, sizeof(pIdStr)); GetWindowText(hBilFee, feeStr, sizeof(feeStr)); GetWindowText(hBilMed, medStr, sizeof(medStr));

    if (!strlen(pIdStr) || !strlen(feeStr)) { MessageBox(hwnd, "Fill Patient ID and Consultation Fee!", "Error", MB_OK | MB_ICONWARNING); return; }

    float fee = atof(feeStr);
    float med = atof(medStr);
    float total = fee + med;

    sprintf(query, "INSERT INTO bills (patient_id, consultation_fee, medicine_charge, total) VALUES (%d, %.2f, %.2f, %.2f)", atoi(pIdStr), fee, med, total);
    if (mysql_query(conn, query) == 0)
    {
        char msg[300];
        sprintf(msg, "[SUCCESS] Bill Generated!\nConsultation: %.2f\nMedicine: %.2f\nTOTAL: %.2f", fee, med, total);
        MessageBox(hwnd, msg, "Bill Summary", MB_OK | MB_ICONINFORMATION);
        SetWindowText(hBilPatId, ""); SetWindowText(hBilFee, ""); SetWindowText(hBilMed, "");
        viewBillsGUI(hwnd);
    }
    else MessageBox(hwnd, mysql_error(conn), "Error", MB_OK | MB_ICONERROR);
}

void viewBillsGUI(HWND hwnd)
{
    MYSQL_RES *res; MYSQL_ROW row; char buffer[4000] = "", line[250];
    char query[] = "SELECT bills.id, patients.name, bills.consultation_fee, bills.medicine_charge, bills.total "
                   "FROM bills JOIN patients ON bills.patient_id = patients.id";

    if (mysql_query(conn, query) != 0) return;
    res = mysql_store_result(conn); if (!res) return;

    sprintf(line, "%-8s %-20s %-15s %-15s %-15s\r\n----------------------------------------------------------------------------------\r\n", "BILL ID", "PATIENT NAME", "CONSULTATION", "MEDICINE", "TOTAL");
    strcat(buffer, line);
    while ((row = mysql_fetch_row(res)))
    {
        sprintf(line, "%-8s %-20s %-15s %-15s %-15s\r\n", row[0], row[1], row[2], row[3], row[4]);
        strcat(buffer, line);
    }
    SetWindowText(hBilOutput, buffer); mysql_free_result(res);
}

void searchBillGUI(HWND hwnd)
{
    char pIdStr[20], query[500]; GetWindowText(hBilPatId, pIdStr, sizeof(pIdStr));
    if (!strlen(pIdStr)) { MessageBox(hwnd, "Enter Patient ID!", "Warning", MB_OK | MB_ICONWARNING); return; }

    sprintf(query, "SELECT bills.id, patients.name, bills.consultation_fee, bills.medicine_charge, bills.total "
                   "FROM bills JOIN patients ON bills.patient_id = patients.id WHERE bills.patient_id = %d", atoi(pIdStr));

    if (mysql_query(conn, query) != 0) return;
    MYSQL_RES *res = mysql_store_result(conn); MYSQL_ROW row;
    char buffer[4000] = "", line[250];

    sprintf(line, "%-8s %-20s %-15s %-15s %-15s\r\n----------------------------------------------------------------------------------\r\n", "BILL ID", "PATIENT NAME", "CONSULTATION", "MEDICINE", "TOTAL");
    strcat(buffer, line);
    int count = 0;
    while ((row = mysql_fetch_row(res)))
    {
        sprintf(line, "%-8s %-20s %-15s %-15s %-15s\r\n", row[0], row[1], row[2], row[3], row[4]);
        strcat(buffer, line); count++;
    }
    if (count == 0) strcat(buffer, "No bills found for this Patient ID.\r\n");
    SetWindowText(hBilOutput, buffer); mysql_free_result(res);
}
