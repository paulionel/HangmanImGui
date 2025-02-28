#include "Application.h"
#include "imgui.h"
#include <vector>
#include <algorithm>
#include <string>

namespace MyApp
{
    const std::string word = "INTEGRATION";
    static std::vector<char> wrongLetters;
    static std::vector<char> correctLetters;
    static bool showHead = false;
    static bool showBody = false;
    static bool showRightArm = false;
    static bool showLeftArm = false;
    static bool showRightLeg = false;
    static bool showLeftLeg = false;

    void checkLetter(char letter)
    {
        if (word.find(letter) != std::string::npos)
        {
            if (std::find(correctLetters.begin(), correctLetters.end(), letter) == correctLetters.end())
            {
                correctLetters.push_back(letter);
                std::sort(correctLetters.begin(), correctLetters.end());
            }
        }
        else
        {
            if (std::find(wrongLetters.begin(), wrongLetters.end(), letter) == wrongLetters.end())
            {
                wrongLetters.push_back(letter);
                std::sort(wrongLetters.begin(), wrongLetters.end());
            }
            int wrongGuesses = wrongLetters.size();
            switch (wrongGuesses)
            {
            case 1: showHead = true; break;
            case 2: showBody = true; break;
            case 3: showRightArm = true; break;
            case 4: showLeftArm = true; break;
            case 5: showRightLeg = true; break;
            case 6: showLeftLeg = true; break;
            }
        }
    }

    void RenderUI()
    {
        // ---- DockSpace Setup (unchanged) ----
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", nullptr, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();
        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);
                ImGui::Separator();
                if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0))
                {
                    dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode;
                }
                if (ImGui::MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0))
                {
                    dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit;
                }
                if (ImGui::MenuItem("Flag: NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0))
                {
                    dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking;
                }
                if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))
                {
                    dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
                }
                if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))
                {
                    dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
                }
                if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen))
                {
                    dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
                }
                ImGui::Separator();
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        // ---- End DockSpace Setup ----

        // ---- Single "Viewport" Window (contains hangman, letters, and letter buttons) ----
        ImGui::Begin("Viewport");

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 viewportPos = ImGui::GetWindowPos();
        ImVec2 viewportSize = ImGui::GetWindowSize();
        ImVec2 center = ImVec2(viewportPos.x + viewportSize.x / 2, viewportPos.y + viewportSize.y / 2);

        // 1. Render Letter Buttons First (At the Bottom)
        ImGui::SetCursorPosY(viewportSize.y - 80); // Push the buttons lower
        if (ImGui::BeginTable("table1", 13)) {
            const char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            static bool buttonStates[26] = { false };
            int letterIndex = 0;

            for (int row = 0; row < 2; row++) {
                ImGui::TableNextRow();
                for (int column = 0; column < 13; column++) {
                    ImGui::TableSetColumnIndex(column);
                    if (letterIndex < 26) {
                        char buttonLabel[2] = { alphabet[letterIndex], '\0' };

                        if (buttonStates[letterIndex] == false) {
                            if (ImGui::Button(buttonLabel)) {
                                buttonStates[letterIndex] = true;
                                checkLetter(alphabet[letterIndex]);
                            }
                        }
                        else {
                            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
                            ImGui::Button(buttonLabel);
                            ImGui::PopStyleVar();
                        }
                        letterIndex++;
                    }
                }
            }
            ImGui::EndTable();
        }

        // 2. Render Correct and Wrong Letters Above Buttons
        ImGui::SetCursorPosY(viewportSize.y - 120);
        ImGui::Text("Correct letters: ");
        for (char c : word) {
            if (std::find(correctLetters.begin(), correctLetters.end(), c) != correctLetters.end()) {
                ImGui::SameLine();
                ImGui::Text("%c", c);
            }
            else {
                ImGui::SameLine();
                ImGui::Text("_");
            }
        }

        ImGui::SetCursorPosY(viewportSize.y - 100);
        ImGui::Text("Wrong letters: ");
        for (char letter : wrongLetters) {
            ImGui::SameLine();
            ImGui::Text("%c", letter);
        }

        // 3. Render Hangman LAST (On Top)
        const int height = 200;
        if (showHead) {
            ImVec2 circlePos = ImVec2(center.x, center.y - height);
            drawList->AddCircle(circlePos, 50.0f, IM_COL32(255, 0, 0, 255), 32, 2.0f);
        }

        if (showBody) {
            ImVec2 start = ImVec2(center.x, center.y - 150);
            ImVec2 end = ImVec2(center.x, center.y);
            drawList->AddLine(start, end, IM_COL32(255, 0, 0, 255), 2.0f);
        }

        if (showRightArm) {
            ImVec2 start = ImVec2(center.x, center.y - 100);
            ImVec2 end = ImVec2(center.x + 50, center.y - 150);
            drawList->AddLine(start, end, IM_COL32(255, 0, 0, 255), 2.0f);
        }

        if (showLeftArm) {
            ImVec2 start = ImVec2(center.x, center.y - 100);
            ImVec2 end = ImVec2(center.x - 50, center.y - 150);
            drawList->AddLine(start, end, IM_COL32(255, 0, 0, 255), 2.0f);
        }

        if (showRightLeg) {
            ImVec2 start = ImVec2(center.x, center.y);
            ImVec2 end = ImVec2(center.x + 50, center.y + 50);
            drawList->AddLine(start, end, IM_COL32(255, 0, 0, 255), 2.0f);
        }

        if (showLeftLeg) {
            ImVec2 start = ImVec2(center.x, center.y);
            ImVec2 end = ImVec2(center.x - 50, center.y + 50);
            drawList->AddLine(start, end, IM_COL32(255, 0, 0, 255), 2.0f);
        }

        ImGui::End(); // End of viewport

        ImGui::End(); // End of DockSpace Demo window
    }
}
