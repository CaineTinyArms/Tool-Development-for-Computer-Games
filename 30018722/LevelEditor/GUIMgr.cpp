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
	
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File")) // Add file Menu to the taskbar.
		{
			if (ImGui::MenuItem("Save Scene", "Ctrl+S")) { // Adds Save Scene as a sub button.
				scene.save(); // If pressed call save function.
			}
			if (ImGui::MenuItem("Load Scene", "Ctrl+L")) { // Adds Load Scene as a sub button
				scene.load(); // If pressed call load function.
			}
			if (ImGui::MenuItem("Add Model..", "Ctrl+A")) // Adds Add Model as a sub button.
				showFileBrowser = true; // If pressed set showFileBrowser to true.

			if (ImGui::MenuItem("Add Node", "Ctrl+N")) // Adds Add Node as a sub button.
				scene.addNavPoint(); // If pressed call addNavPoint function.
			ImGui::EndMenu(); // Ends File Menu.
		}
		if (ImGui::BeginMenu("Edit")) // Adds edit Menu to the task bar.
		{
			if (ImGui::MenuItem("Delete Selected Item", "Delete")) // Adds Delete Selected Item sub button.
			{
				if (scene.selected_model) // If clicked, and a model is selected.
				{
					scene.removeModel(); // Remove the model.
				}
				else if (scene.selectedNavPoint) // If clicked, and a node is selected.
				{
					scene.removeNode(); // Remove the node.
				}
				else
				{
					cout << "Nothing Selected, or SceneRoot selected."; // Outputs saying nothing to remove.
				}
			}
			ImGui::Separator(); // Adds a seperator in the edit menu.
			if (ImGui::MenuItem("Clear Nodes", "Ctrl+0")) // Adds a Clear Nodes sub button.
			{
				scene.clearNavSet(); // Calls clearNavSet when pressed.
			}
			if (ImGui::MenuItem("Clear Models", "Ctrl+9")) // Adds a Clear Models sub button.
			{
				scene.clearModels(); // Calls clearModels when pressed.
			}
			ImGui::EndMenu(); // Ends edit menu.
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
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_None) && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered())
	{
		scene.selected_model = nullptr;
		scene.selectedNavPoint = nullptr;
	}

	DrawModel(&scene.rootModel);

	
	if (!scene.NavSet.NavPoints.empty()) // Checks if there are NavPoints added.
	{
		if (ImGui::TreeNode("NavPoints")) // Creates a tree node called NavPoints.
		{
			
			for (int i = 0; i < scene.NavSet.NavPoints.size(); i++) // Runs through all NavPoints.
			{
				NavPoint& nav = scene.NavSet.NavPoints[i]; // Stores a ref to the current NavPoint
				bool isSelected = (scene.selectedNavPoint == &nav); // Checks if the NavPoint is currently selected.

				if (ImGui::Selectable(nav.name.c_str(), isSelected)) // If Clicked in the tree, highlight it.
				{
					scene.selectedNavPoint = &nav; // Set selected nav point as the clicked nav point.
					scene.selected_model = nullptr; // Resets selected Model.
				}
			}
			ImGui::TreePop(); // Closes NavPoint Tree Node.
		}
	}
	else
	{
		ImGui::Text("NavPoints (none)"); // Says there is no NavPoints.
	}

	ImGui::End();
	//9.Add a properties window here
	ImGui::Begin("Properties");
	{
		// If the window is clicked, but nothing inside the window is clicked, sets selected_model and navPoint to null.
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_None) && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered())  
		{
			scene.selected_model = nullptr;
			scene.selectedNavPoint = nullptr;
		}

		if (scene.selected_model) // If a model is selected.
		{
			ImGui::Text(scene.selected_model->name.c_str());
			ImGui::InputFloat3("Position", &scene.selected_model->pos[0]);	// Displays the selected model position in Input Boxes that can be editied.
			ImGui::InputFloat3("Rotation", &scene.selected_model->rot[0]); // Displays the selected model rotation in Input Boxes that can be editied.
			ImGui::InputFloat3("Scale", &scene.selected_model->scale[0]); // Displays the selected model scale in Input Boxes that can be editied.
		}
		else if (scene.selectedNavPoint) // If a navpoint is selected.
		{
			ImGui::Text(scene.selectedNavPoint->name.c_str()); // Adds text of the selected NavPoint name.
			ImGui::InputFloat3("Position", &scene.selectedNavPoint->pos[0]); // Displays the selected NavPoint position in Input Boxes that can be editied.
			ImGui::InputFloat3("Rotation", &scene.selectedNavPoint->rot[0]); // Displays the selected NavPoint rotation in Input Boxes that can be editied.
			ImGui::InputFloat3("Scale", &scene.selectedNavPoint->scale[0]); // Displays the selected NavPoint scale in Input Boxes that can be editied.
		}
		else
		{
			ImGui::Text("No Model or NavPoint selected"); // Changes the text to say nothing is selected.
		}
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
