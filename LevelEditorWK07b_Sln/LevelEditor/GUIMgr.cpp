#include "GUIMgr.h"

void GUIMgr::DrawModel(Model* node) {
	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	if (node->children.empty()) {

		ImGui::Selectable(node->name.c_str(), scene.selected_model == node);
		if (ImGui::IsItemClicked())
			scene.selected_model = node;
	}
	else {
		ImGui::SetNextItemOpen(node->open);
		ImGuiTreeNodeFlags node_flags = base_flags;

		if (node == scene.selected_model)
			node_flags |= ImGuiTreeNodeFlags_Selected;
		bool open = ImGui::TreeNodeEx(node->name.c_str(), node_flags);
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			scene.selected_model = node;
			scene.CurrentObject = node->ID;
		}
			
		if (open)
		{
			node->open = true;

			for (Model& child : node->children) {
				DrawModel(&child);
			}
			ImGui::TreePop();
		}
		else
			node->open = false;
	}
}

void GUIMgr::drawConsoleWindow(std::stringstream* buffer)
{
	//Console window, cout redirected here
	ImGui::Begin("Console");
	std::string console_text = (*buffer).str();
	ImGui::Text(console_text.c_str());
	ImGui::SetScrollHereY(1.0f);
	ImGui::End();

}

void GUIMgr::init(GLFWwindow* window)
{
	//2.Add IMGU initialisation code :
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

}
void GUIMgr::createFrame()
{
	//3.Create a new imGUI frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

}
void GUIMgr::drawMenu()
{
	//7.Add a main menu here
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			//8.Add New, Open, Save and Exit menu items here
			if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
				cout << "Saving scene" << endl;
				scene.save();
			}
			if (ImGui::MenuItem("Load Scene", "Ctrl+L")) {
				scene.load();
			}
			if (ImGui::MenuItem("Add Model..", "Ctrl+A"))
				showFileBrowser = true;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	if (showFileBrowser)
	{

		static fs::path currentDirectory = fs::current_path();
		static std::vector<fs::path> currentFiles;

		if (ImGui::Begin("File Browser", &showFileBrowser))
		{
			ImGui::NewLine();
			// Display current directory path
			ImGui::Text("Current directory: %s", currentDirectory.c_str());
			ImGui::NewLine();
			std::string path = "../../Resources/Models/";

			// Display directory contents
			for (const auto& entry : fs::recursive_directory_iterator(path))
			{
				//cout << entry.path().c_str() << endl;
				if (entry.path().extension() == ".obj" || entry.path().extension() == ".glb")
				{
					if (ImGui::Selectable(entry.path().string().c_str()))
					{
						// User selected a file, set the selectedFile variable
						scene.rootModel.children.push_back(*new Model(entry.path().string(), entry.path().filename().string()));
						scene.selected_model = &scene.rootModel.children[scene.rootModel.children.size() - 1];
						scene.selected_model->ID = scene.CurrentObject;
						scene.CurrentObject++;
						showFileBrowser = false;

					}
				}
			}
		}
		ImGui::End();
	}
}
void GUIMgr::drawPropertiesWindow(Model* model)
{
	ImGui::Begin("Model Tree");
	DrawModel(&scene.rootModel);
	ImGui::End();
	//9.Add a properties window here
	ImGui::Begin("Properties");
	{
		if (scene.selected_model)
		{
			//11.Add text input here
			ImGui::Text(scene.selected_model->name.c_str());
			ImGui::InputFloat3("Position", &scene.selected_model->pos[0]);	
			ImGui::InputFloat3("Rotation", &scene.selected_model->rot[0]);
			ImGui::InputFloat3("Scale", &scene.selected_model->scale[0]);
		}
		else
			ImGui::Text("No Model selected");
	}
	ImGui::End();

}
void GUIMgr::draw()//Finalise ImGUI rendering
{
	//Finalise ImGUI rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void GUIMgr::drawAll(std::stringstream* buffer)
{
	//Create a new imGUI frame
	createFrame();
	//Show the ImGUI Demo
	bool show = true;
	ImGui::ShowDemoWindow(&show);
	//Add a main menu here
	drawMenu();
	//Add a properties window here
	drawPropertiesWindow(&scene.rootModel);
	//Add console
	drawConsoleWindow(buffer);
	//Finalise ImGUI rendering
	draw();
}
void GUIMgr::shutDown()//cleanup ImG
{
	//Cleanup ImGUI. After the render loop but before “glfwTerminate(); ” 
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
