#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <windows.h>

using namespace std;

// Color codes for Windows console
#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_YELLOW 6
#define COLOR_WHITE 7
#define COLOR_GRAY 8
#define COLOR_LIGHT_BLUE 9
#define COLOR_LIGHT_GREEN 10
#define COLOR_LIGHT_CYAN 11
#define COLOR_LIGHT_RED 12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_LIGHT_YELLOW 14
#define COLOR_BRIGHT_WHITE 15

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void setColor(int foreground, int background = 0) {
    SetConsoleTextAttribute(hConsole, (background << 4) | foreground);
}

void clearScreen() {
    system("cls");
}

void pause() {
    cout << "\n";
    setColor(COLOR_LIGHT_CYAN);
    cout << "  Press any key to continue...";
    setColor(COLOR_WHITE);
    system("pause > nul");
}

// ============================================================================
// CONFIGURATION
// ============================================================================
const string FORK_REPO = "https://github.com/pgwiz/seepo.git";
const string UPSTREAM_REPO = "https://github.com/pkinyanjui461-dev/seepo.git";

void savePATToConfig(const string& configFile, const string& pat, const string& parentPat) {
    ofstream file(configFile.c_str(), ios::trunc);
    if (file.is_open()) {
        file << "PAT=" << pat << endl;
        file << "PARENT_PAT=" << parentPat << endl;
        file << "# SEEPO Git Sync Configuration\n";
        file << "# Created: " << __DATE__ << " " << __TIME__ << endl;
        file.close();
    }
}

string readConfigValue(const string& configFile, const string& key) {
    ifstream file(configFile.c_str());
    if (file.is_open()) {
        string line;
        string searchKey = key + "=";
        while (getline(file, line)) {
            if (line.substr(0, searchKey.length()) == searchKey) {
                file.close();
                return line.substr(searchKey.length());
            }
        }
        file.close();
    }
    return "";
}

void printBanner() {
    clearScreen();
    setColor(COLOR_LIGHT_CYAN);
    cout << "\n";
    cout << "  ╔════════════════════════════════════════════════════════════════════════════╗\n";
    cout << "  ║                                                                            ║\n";
    setColor(COLOR_LIGHT_YELLOW);
    cout << "  ║    ███████╗███████╗███████╗██████╗  ██████╗     ███████╗██╗   ██╗███╗   ███╗ ║\n";
    cout << "  ║    ██╔════╝██╔════╝██╔════╝██╔══██╗██╔═══██╗    ██╔════╝╚██╗ ██╔╝████╗ ████║ ║\n";
    cout << "  ║    ███████╗█████╗  █████╗  ██████╔╝██║   ██║    ███████╗ ╚████╔╝ ██╔████╔██║ ║\n";
    cout << "  ║    ╚════██║██╔══╝  ██╔══╝  ██╔═══╝ ██║   ██║    ╚════██║  ╚██╔╝  ██║╚██╔╝██║ ║\n";
    cout << "  ║    ███████║███████╗███████╗██║     ╚██████╔╝    ███████║   ██║   ██║ ╚═╝ ██║ ║\n";
    cout << "  ║    ╚══════╝╚══════╝╚══════╝╚═╝      ╚═════╝     ╚══════╝   ╚═╝   ╚═╝     ╚═╝ ║\n";
    setColor(COLOR_LIGHT_CYAN);
    cout << "  ║                                                                            ║\n";
    cout << "  ║                     ██████╗ ██╗████████╗                                  ║\n";
    cout << "  ║                    ██╔════╝ ██║╚══██╔══╝                                  ║\n";
    setColor(COLOR_LIGHT_GREEN);
    cout << "  ║                    ██║  ███╗██║   ██║                                     ║\n";
    cout << "  ║                    ██║   ██║██║   ██║                                     ║\n";
    cout << "  ║                    ╚██████╔╝██║   ██║                                     ║\n";
    cout << "  ║                     ╚═════╝ ╚═╝   ╚═╝                                     ║\n";
    setColor(COLOR_LIGHT_CYAN);
    cout << "  ║                                                                            ║\n";
    setColor(COLOR_LIGHT_YELLOW);
    cout << "  ║                 GitHub Repository Fork & Upstream Push Tool               ║\n";
    cout << "  ║                              Version 1.0                                  ║\n";
    setColor(COLOR_LIGHT_CYAN);
    cout << "  ║                                                                            ║\n";
    cout << "  ╚════════════════════════════════════════════════════════════════════════════╝\n";
    setColor(COLOR_WHITE);
    cout << "\n";
}

void printMenu() {
    clearScreen();
    setColor(COLOR_LIGHT_CYAN);
    cout << "\n";
    cout << "  ╔════════════════════════════════════════════════════════════════════════════╗\n";
    setColor(COLOR_LIGHT_YELLOW);
    cout << "  ║                        SELECT OPERATION TO PERFORM                        ║\n";
    setColor(COLOR_LIGHT_CYAN);
    cout << "  ╚════════════════════════════════════════════════════════════════════════════╝\n";
    cout << "\n";
    setColor(COLOR_WHITE);
    cout << "  PUSH TO PARENT (Upstream Repository)\n";
    setColor(COLOR_LIGHT_GREEN);
    cout << "     [1] ► Push MAIN branch to Parent\n";
    setColor(COLOR_LIGHT_MAGENTA);
    cout << "     [2] ► Push MASTER branch to Parent\n";
    setColor(COLOR_LIGHT_YELLOW);
    cout << "     [3] ► Push BOTH branches to Parent\n";
    setColor(COLOR_WHITE);
    cout << "\n";
    cout << "  CONFIGURATION\n";
    setColor(COLOR_LIGHT_CYAN);
    cout << "     [4] ► Update PAT Token        (Fork authentication)\n";
    cout << "     [5] ► Update PARENT_PAT       (Upstream authentication)\n";
    setColor(COLOR_RED);
    cout << "     [0] ► EXIT\n";
    setColor(COLOR_WHITE);
    cout << "\n";
    cout << "  ────────────────────────────────────────────────────────────────────────────\n";
    cout << "\n";
}

void executePushCommand(const string& parentPat, const string& branch) {
    clearScreen();
    setColor(COLOR_LIGHT_CYAN);
    cout << "\n";
    cout << "  ╔════════════════════════════════════════════════════════════════════════════╗\n";
    setColor(COLOR_LIGHT_YELLOW);
    cout << "  ║                   PUSHING " << (branch == "main" ? "MAIN" : "MASTER") << " BRANCH TO PARENT...                    ║\n";
    setColor(COLOR_LIGHT_CYAN);
    cout << "  ╚════════════════════════════════════════════════════════════════════════════╝\n";
    setColor(COLOR_WHITE);
    cout << "\n";

    // Construct parent upstream URL with PAT
    string parentURL = "https://" + parentPat + "@github.com/pkinyanjui461-dev/seepo.git";

    // Build git push command
    stringstream commands;
    commands << "git push " << parentURL << " " << branch << " --quiet";

    setColor(COLOR_LIGHT_GREEN);
    cout << "  Pushing " << branch << " branch to parent repository...\n\n";
    setColor(COLOR_LIGHT_YELLOW);

    system(commands.str().c_str());

    setColor(COLOR_LIGHT_GREEN);
    cout << "\n  [SUCCESS] Push completed!\n";
    setColor(COLOR_WHITE);
    cout << "\n";
}

int main() {
    setColor(COLOR_LIGHT_CYAN);

    string configFile = "menu.conf";
    string pat;
    string parentPat;
    int choice;

    // Try to load PAT and PARENT_PAT from config
    pat = readConfigValue(configFile, "PAT");
    parentPat = readConfigValue(configFile, "PARENT_PAT");

    if (pat.empty() || parentPat.empty()) {
        printBanner();

        if (pat.empty()) {
            setColor(COLOR_LIGHT_GREEN);
            cout << "  [INFO] Enter GitHub PAT (Fork: pgwiz/seepo):\n";
            setColor(COLOR_WHITE);
            cout << "  > PAT Token: ";
            getline(cin, pat);
            if (pat.empty()) {
                setColor(COLOR_RED);
                cout << "\n  [ERROR] PAT token cannot be empty!\n\n";
                setColor(COLOR_WHITE);
                return 1;
            }
        }

        if (parentPat.empty()) {
            setColor(COLOR_LIGHT_GREEN);
            cout << "\n  [INFO] Enter PARENT_PAT (Upstream: pkinyanjui461-dev/seepo):\n";
            setColor(COLOR_WHITE);
            cout << "  > PARENT_PAT Token: ";
            getline(cin, parentPat);
            if (parentPat.empty()) {
                setColor(COLOR_RED);
                cout << "\n  [ERROR] PARENT_PAT token cannot be empty!\n\n";
                setColor(COLOR_WHITE);
                return 1;
            }
        }

        // Save both tokens to config
        savePATToConfig(configFile, pat, parentPat);
        setColor(COLOR_LIGHT_GREEN);
        cout << "\n  [SUCCESS] Tokens saved to menu.conf\n\n";
        setColor(COLOR_WHITE);
        pause();
    } else {
        printBanner();
        setColor(COLOR_LIGHT_GREEN);
        cout << "  [SUCCESS] Tokens loaded from menu.conf\n\n";
        setColor(COLOR_WHITE);
        pause();
    }

    // Main loop
    while (true) {
        printMenu();

        setColor(COLOR_LIGHT_CYAN);
        cout << "  Select option (0-5): ";
        setColor(COLOR_WHITE);

        string input;
        getline(cin, input);

        if (input.empty()) {
            continue;
        }

        choice = atoi(input.c_str());

        switch (choice) {
            case 0:
                clearScreen();
                setColor(COLOR_LIGHT_CYAN);
                cout << "\n";
                cout << "  ╔════════════════════════════════════════════════════════════════════════════╗\n";
                setColor(COLOR_LIGHT_YELLOW);
                cout << "  ║                                                                            ║\n";
                cout << "  ║                 Thank you for using SEEPO Sync (C++)!                     ║\n";
                cout << "  ║                                                                            ║\n";
                setColor(COLOR_LIGHT_CYAN);
                cout << "  ╚════════════════════════════════════════════════════════════════════════════╝\n";
                setColor(COLOR_WHITE);
                cout << "\n";
                return 0;

            case 1:
                executePushCommand(parentPat, "main");
                pause();
                break;

            case 2:
                executePushCommand(parentPat, "master");
                pause();
                break;

            case 3:
                clearScreen();
                setColor(COLOR_LIGHT_CYAN);
                cout << "\n";
                cout << "  ╔════════════════════════════════════════════════════════════════════════════╗\n";
                setColor(COLOR_LIGHT_YELLOW);
                cout << "  ║                   PUSHING BOTH BRANCHES TO PARENT...                     ║\n";
                setColor(COLOR_LIGHT_CYAN);
                cout << "  ╚════════════════════════════════════════════════════════════════════════════╝\n";
                setColor(COLOR_WHITE);
                cout << "\n";
                setColor(COLOR_LIGHT_CYAN);
                cout << "  Step 1/2: Pushing MAIN branch...\n";
                setColor(COLOR_LIGHT_GREEN);
                executePushCommand(parentPat, "main");
                setColor(COLOR_LIGHT_CYAN);
                cout << "  Step 2/2: Pushing MASTER branch...\n";
                setColor(COLOR_LIGHT_GREEN);
                executePushCommand(parentPat, "master");
                setColor(COLOR_LIGHT_GREEN);
                cout << "  [SUCCESS] Both branches pushed to parent!\n";
                setColor(COLOR_WHITE);
                pause();
                break;

            case 4:
                printBanner();
                setColor(COLOR_LIGHT_GREEN);
                cout << "  [INFO] Enter new PAT (Fork authentication):\n";
                setColor(COLOR_WHITE);
                cout << "  > PAT Token: ";
                string newPat;
                getline(cin, newPat);
                if (!newPat.empty()) {
                    pat = newPat;
                    savePATToConfig(configFile, pat, parentPat);
                    setColor(COLOR_LIGHT_GREEN);
                    cout << "\n  [SUCCESS] PAT token updated!\n\n";
                    setColor(COLOR_WHITE);
                } else {
                    setColor(COLOR_RED);
                    cout << "\n  [ERROR] PAT token cannot be empty!\n\n";
                    setColor(COLOR_WHITE);
                }
                pause();
                break;

            case 5:
                printBanner();
                setColor(COLOR_LIGHT_GREEN);
                cout << "  [INFO] Enter new PARENT_PAT (Upstream authentication):\n";
                setColor(COLOR_WHITE);
                cout << "  > PARENT_PAT Token: ";
                string newParentPat;
                getline(cin, newParentPat);
                if (!newParentPat.empty()) {
                    parentPat = newParentPat;
                    savePATToConfig(configFile, pat, parentPat);
                    setColor(COLOR_LIGHT_GREEN);
                    cout << "\n  [SUCCESS] PARENT_PAT token updated!\n\n";
                    setColor(COLOR_WHITE);
                } else {
                    setColor(COLOR_RED);
                    cout << "\n  [ERROR] PARENT_PAT token cannot be empty!\n\n";
                    setColor(COLOR_WHITE);
                }
                pause();
                break;

            default:
                printBanner();
                setColor(COLOR_RED);
                cout << "  [ERROR] Invalid choice. Please select 0-5.\n\n";
                setColor(COLOR_WHITE);
                pause();
                break;
        }
    }

    return 0;
}
