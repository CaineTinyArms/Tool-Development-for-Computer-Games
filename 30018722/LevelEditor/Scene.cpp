#include "Scene.h"

void Scene::load()
{
	rootModel.children.clear(); // Clears any existing models when loading begins.
	CurrentObject = 3; // Resets the Model Counter.
	selected_model = nullptr; // Resets the selected model pointer.
	NavSet.NavPoints.clear(); // Clears any existing NavPoints when loading begins. 
	NavSet.init(); // Calls NavSet.init so the NavSet is ready for populating.
	CurrentNavPoint = 255; // Resets the NavPoint counter.
	selectedNavPoint = nullptr; // Resets the selected NavPoint pointer.

	tinyxml2::XMLDocument doc; 
	doc.LoadFile("..\\..\\Resources\\Levels\\Scene01.xml"); // Loads the file from the path.
	if (doc.Error())
	{
		std::cerr << "Error parsing XML: " << doc.ErrorStr() << std::endl; // If the file has an error output this message.
		return;
	}

	tinyxml2::XMLElement* sceneElement = doc.FirstChildElement("Scene"); // Sets sceneElement to the child element with the Scene tag.
	if (!sceneElement)
	{
		std::cerr << "No <Scene> element found in Scene01.xml\n"; // If there is no Scene tag output message.
		return;
	} 

	// Model Loading
	tinyxml2::XMLElement* modelElement = sceneElement->FirstChildElement("Model");
	if (modelElement)
	{
		loadModel(modelElement, &rootModel.children, &CurrentObject); // Loads the models recurrsivley using existing function.
	}

	// NavPoint Loading
	tinyxml2::XMLElement* navSetElement = sceneElement->FirstChildElement("NavSet"); // Gets the NavSet tag.
	if (navSetElement) // Checks if there is a NavSet Tag.
	{
		tinyxml2::XMLElement* navPointElement = navSetElement->FirstChildElement("NavPoint"); // Gets the First Nav Point
		while (navPointElement) // While there is still Nav Points
		{
			tinyxml2::XMLElement* nameElem = navPointElement->FirstChildElement("Name"); // Gets the name data.
			tinyxml2::XMLElement* dataElem = navPointElement->FirstChildElement("Data"); // Gets the data.

			if (nameElem && dataElem) // Checks there's both a name and data.
			{
				const char* nameText = nameElem->GetText(); // Gets the text from the Name data.
				const char* dataText = dataElem->GetText(); // Gets the text from the Data Tag.

				if (nameText && dataText) // Checks there is text for both of them.
				{
					NavPoint newNavPoint(nameText); // Creates a new NavPoint with the Name.

					// Puts the data from Data Text into the newly created NavPoint
					std::string dataStr(dataText);
					newNavPoint.pos = parseNextVec3(dataStr);
					newNavPoint.rot = parseNextVec3(dataStr);
					newNavPoint.scale = parseNextVec3(dataStr);

					newNavPoint.ID = CurrentNavPoint; // Sets the newNavPoint to the CurrentNavPoint counter.
					CurrentNavPoint--; // Decrements the counter.
					NavSet.NavPoints.push_back(newNavPoint); // Adds the newNavPoint to the NavSet.
				}
				else
				{
					std::cerr << "Malformed <NavPoint> (missing text in <Name> or <Data>).\n"; // Outputs if there no text in the name or data.
				}
			}
			else
			{
				std::cerr << "Malformed <NavPoint> (missing <Name> or <Data> element).\n"; // Outputs if there no Name or Data tag.
			}

			navPointElement = navPointElement->NextSiblingElement("NavPoint"); // Moves onto the next NavPoint.
		}
	}

	std::cout << "Scene loaded: " << rootModel.children.size() << " top-level models, " // Outputs letting the user know the loading has taken place and says how many models and nav points were loaded.
		<< NavSet.NavPoints.size() << " nav points." << std::endl;
}

void Scene::loadModel(ti::XMLElement* model, vector<Model>* modelList, int* CurrentObject)
{
	int count = 0;
	while (model)
	{
		string path = model->FirstChildElement("PathName")->GetText();
		string data = model->FirstChildElement("Data")->GetText();
		fs::path pathName(path);
		modelList->push_back(*new Model(pathName.string(), pathName.filename().string(), data));
		(*modelList)[count].ID = (*CurrentObject);
		(*CurrentObject)++;


		ti::XMLElement* modelChild = model->FirstChildElement("Model");
		if (modelChild)
			loadModel(modelChild, &((*modelList)[count].children), CurrentObject);

		model = model->NextSiblingElement("Model");
		count++;
	}
}


bool Scene::selectModelByID(Model* model, int CurrentObjectID)
{
	if (model->ID == CurrentObjectID)
	{
		selected_model = model;
		cout << "Selected Model ID = " << selected_model->ID << ", Name = " << selected_model->name<< endl;
		selectedNavPoint = nullptr;
		return true;
	}
	else
	{
		for (int i = 0; i < model->children.size(); i++)
			if (selectModelByID(&model->children[i], CurrentObjectID))
				selectedNavPoint = nullptr;
				return true;
	}
	return false;
}


void Scene::saveModel(Model* model, ofstream* outfile)
{
	if(model->name!="SceneRoot")
	{ 
		*outfile << "<Model>";
		*outfile << model->toXML() << endl;
	}
	if (model->children.size() > 0)
		for (int i = 0; i < model->children.size(); i++)
			saveModel(&model->children[i], outfile);
	if (model->name != "SceneRoot")
		*outfile << "</Model>";
}


void Scene::save()
{
	std::ofstream outfile("..\\..\\Resources\\Levels\\Scene01.xml"); // Opens the file to save to.
	if (!outfile)
	{
		std::cerr << "Failed to open Scene01.xml for writing." << std::endl; // Outputs if the file could not be opened.
		return;
	}

	std::cout << "Saving scene..." << std::endl; // Tells the user the scene is saving.
	outfile << "<Scene>\n"; // Starts the file with the scene tag.
	 
	saveModel(&rootModel, &outfile); // Saves all models using the existing saveModel function.

	
	outfile << "  <NavSet>\n"; // Outputs the navset tag.
	for (NavPoint& np : NavSet.NavPoints) // Runs through all the navpoints.
	{
		outfile << "    <NavPoint>\n"; // Outputs the navpoint tag.
		outfile << np.toXML() << "\n";   // Outputs the navpoint data using the toXML function from the NavPoint class.
		outfile << "    </NavPoint>\n"; // Closes the navpoint tag.
	}
	outfile << "  </NavSet>\n"; // Closes the navset.

	outfile << "</Scene>\n"; // Closes the scene. 
	outfile.close(); // Closes the file.

	std::cout << "Scene saved successfully (models + navpoints)." << std::endl; // Outputs that the saving was successful.
}

void Scene::addNavPoint()
{
	if (NavSet.navModel == nullptr) // Checks if the NavSet has not been initialised.
	{
		NavSet.init(); // Initalises the NavSet
	}
	string name = "nav " + to_string(255 - CurrentNavPoint); // Creates a string with the name Nav and the current nav point counter.
	NavSet.NavPoints.push_back(*new NavPoint(name)); // Creates a new NavPoint with the name and adds it to the NavSet.
	selected_model = nullptr; // Sets the Selected Model to a Nullptr.
	selectedNavPoint = &NavSet.NavPoints.back(); // Sets the selectedNavPoint to the newly created NavPoint
	selectedNavPoint->ID = CurrentNavPoint; // Sets the ID to the navpoint counter
	CurrentNavPoint--; // Decrements the NavPoint counter.
}

bool Scene::selectNodeByID(int currentNodeID) 
{
	for (NavPoint &NavPoint : NavSet.NavPoints) // Loops through every NavPoint Node
	{
		if (NavPoint.ID == currentNodeID) // Checks if the ID is the node ID that is passed to the function.
		{
			selectedNavPoint = &NavPoint; // Sets the selected nav point as the newly found nav point.
			selected_model = nullptr; // Changes selected model to a nullptr.
			cout << "Selected Node ID = " << selectedNavPoint->ID << ", Name = " << selectedNavPoint->name << endl; // Tells the user what node has been selected.
			return true;
		}
	}
	return false;
}

glm::vec3 Scene::parseNextVec3(std::string& data)
{
	glm::vec3 result(0.0f); // Creates a vec 3 full of 0's in case data can't be found.

	size_t start = data.find("("); // Finds the first ( in the string.
	size_t end = data.find(")"); // Finds the first ) in the string.
	if (start == std::string::npos || end == std::string::npos) // If ( and ) cannot be found, it returns the blank vec3.
	{
		return result;
	}
	
	std::string inside = data.substr(start + 1, end - start - 1); // Gets the string between the ( ).

	
	data.erase(0, end + 1); // Deletes the data just recovered, including the ( ). Allowing the next vec3 to be parsed.

	
	std::replace(inside.begin(), inside.end(), ',', ' '); // Replaces commas with spaces.

	std::istringstream iss(inside); // Creates a string stream from the extracted data.
	float x, y, z; // Floats to hold the x y and z
	iss >> x >> y >> z; // Stores the x y and z from the string stream.
	result = glm::vec3(x, y, z); // Assigns result to the found x,y,z data.

	return result; // Returns the parsed vector.
}

void Scene::clearNavSet()
{
	selectedNavPoint = nullptr; // Sets selected NavPoint to nullptr;
	CurrentNavPoint = 255; // Resets NavPoint Counter.
	NavSet.NavPoints.clear(); // Clears the NavSet vector.
}

void Scene::removeNode()
{
	if (selectedNavPoint) // Checks if there is a selected nav point.
	{
		for (int NavPoint = 0; NavPoint < NavSet.NavPoints.size(); NavPoint++) // Loops through every NavPoint
		{
			if (&NavSet.NavPoints[NavPoint] == selectedNavPoint) // Checks if the NavPoint is the selected NavPoint.
			{
				NavSet.NavPoints.erase(NavSet.NavPoints.begin() + NavPoint); // Removes the selected NavPoint from the NavSet.
				selectedNavPoint = nullptr; // Resets the selected NavPoint.
				CurrentNavPoint++; // Increments the NavPoint counter.
				break; // Breaks the for loop.
			}
		}

		for (int i = 0; i < NavSet.NavPoints.size(); i++) // Loops through the size of the navset.
		{
			NavSet.NavPoints[i].name = "nav " + std::to_string(i); // Renames all remaining navnodes to prevent new nodes from having duplicated names.
		}
	}
}

bool Scene::removeModelFromChildren(Model* parent, Model* toRemove)
{
	for (int i = 0; i < parent->children.size(); i++) // Loops through all children of the parent model.
	{
		if (&parent->children[i] == toRemove) // If the current child matches the child to remove.
		{
			parent->children.erase(parent->children.begin() + i); // Removes the child from the vector.
			return true; // Returns true so we know it's been removed.
		}

		else
		{
			if (removeModelFromChildren(&parent->children[i], toRemove)) // Call the function recursively on the current child. 
			{
				return true;
			}
		}
	}
	return false;
}

bool Scene::removeModel()
{
	if (!selected_model) // If there is no model selected, do nothing.
	{
		return false;
	}
	if (selected_model->name == "SceneRoot") // If the selected model is the Scene Root, do nothing.
	{
		return false;
	}

	bool removed = removeModelFromChildren(&rootModel, selected_model); // Bool to check is the model has been removed and calls the previous function.

	if (removed)
	{
		cout << "Removing Selected Model" << endl; // Outputs if removing was successful.
		selected_model = nullptr; // Resets selected model.
	}

	return removed;
}

void Scene::clearModels()
{
	rootModel.children.clear(); // Clears all models from the scene root.
	selected_model = nullptr; // Resets selected model.
	CurrentObject = 3; // Resets Model counter.
}

;