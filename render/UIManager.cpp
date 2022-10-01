#include "UIManager.h"
#include <iostream>
#include "implot.h"
#include "imgui_filebrowser.h"
#include "imgui_internal.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"

static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
ImGuiFileBrowser m_FileDialog; // As a class member or globally

UIManager::
UIManager()
{
    initialize();
}

void
UIManager::
initialize()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    ImGui::StyleColorsDark();
    ImPlot::CreateContext();


    // Setup Dear ImGui style
    ImGui::StyleColorsClassic();
    ImGui_ImplGLUT_Init();
    ImGui_ImplOpenGL2_Init();

}

void
UIManager::
render()
{
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();

    display();
    // Rendering
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void
UIManager::
display()
{


    bool open = false;
    

    if (ImGui::BeginMainMenuBar())
    {
        

        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("(demo menu)", NULL, false, false);
			if (ImGui::MenuItem("New")) {}
			if (ImGui::BeginMenu("Open", "Ctrl+O")) 
            {
                if(ImGui::MenuItem("BVH", NULL))
                    open = true;

                if(ImGui::MenuItem("FBX", NULL))
                    open = true;

                ImGui::EndMenu();
            }

			ImGui::Separator();
			if (ImGui::BeginMenu("Options"))
			{
				static bool enabled = true;
				ImGui::MenuItem("Enabled", "", &enabled);
				ImGui::BeginChild("child", ImVec2(0, 60), true);
				for (int i = 0; i < 10; i++)
					ImGui::Text("Scrolling Text %d", i);
				ImGui::EndChild();
				static float f = 0.5f;
				static int n = 0;
				ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
				ImGui::InputFloat("Input", &f, 0.1f);
				ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Checked", NULL, true)) {}
			if (ImGui::MenuItem("Quit", "Alt+F4")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if(open)
        ImGui::OpenPopup("Open File");

    if(m_FileDialog.showFileDialog("Open File", ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".bvh,.fbx"))
    {
        // std::cout << "Open File : "<< m_FileDialog.selected_fn << std::endl;      // The name of the selected file or directory in case of Select Directory dialog mode
        std::cout << "Open File : "<< m_FileDialog.selected_path << std::endl;    // The absolute path to the selected file
    }

    // // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
		ImGui::Begin("Performance"); 
        static int counter = 0;
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // // 3. Show another simple window.
    {
        ImGui::Begin("Indicator");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

        if (ImGui::CollapsingHeader("Discriminator value"))
        {
    	
    	}
        ImGui::End();
    }
}

UIManager::
~UIManager()
{
    // m_FileDialog = nullptr;
    // Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();
}

void
UIManager::
keyboardEvent(unsigned char key, int x, int y)
{
	ImGui_ImplGLUT_KeyboardFunc(key,x,y);
}
void
UIManager::
specialEvent(int key,int x,int y)
{
	ImGui_ImplGLUT_SpecialFunc(key,x,y);
}
void
UIManager::
mouseEvent(int button, int state, int x,int y)
{
	ImGui_ImplGLUT_MouseFunc(button,state,x,y);
}
void
UIManager::
motionEvent(int x,int y)
{
	ImGui_ImplGLUT_MotionFunc(x,y);
}
void
UIManager::
reshapeEvent(int w,int h)
{
	ImGui_ImplGLUT_ReshapeFunc(w,h);
}
