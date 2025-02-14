#include "Scene.h"

void Scene::load()
{
	// Prepare to load fresh data
	rootModel.children.clear();
	CurrentObject = 3;
	selected_model = nullptr;

	// Also clear NavPoints
	NavSet.NavPoints.clear();
	NavSet.init();
	CurrentNavPoint = 255;
	selectedNavPoint = nullptr;

	// Load from the same combined file
	tinyxml2::XMLDocument doc;
	doc.LoadFile("..\\..\\Resources\\Levels\\Scene01.xml");
	if (doc.Error())
	{
		std::cerr << "Error parsing XML: " << doc.ErrorStr() << std::endl;
		return;
	}

	// <Scene> root
	tinyxml2::XMLElement* sceneElement = doc.FirstChildElement("Scene");
	if (!sceneElement)
	{
		std::cerr << "No <Scene> element found in Scene01.xml\n";
		return;
	} 

	// 1) Load Models
	// Find first <Model> child of <Scene> (if any)
	tinyxml2::XMLElement* modelElement = sceneElement->FirstChildElement("Model");
	if (modelElement)
	{
		// Re-use your existing loadModel function 
		// that parses <Model> elements recursively
		loadModel(modelElement, &rootModel.children, &CurrentObject);
	}

	// 2) Load NavPoints from <NavSet> block
	tinyxml2::XMLElement* navSetElement = sceneElement->FirstChildElement("NavSet");
	if (navSetElement)
	{
		tinyxml2::XMLElement* navPointElement = navSetElement->FirstChildElement("NavPoint");
		while (navPointElement)
		{
			tinyxml2::XMLElement* nameElem = navPointElement->FirstChildElement("Name");
			tinyxml2::XMLElement* dataElem = navPointElement->FirstChildElement("Data");

			if (nameElem && dataElem)
			{
				const char* nameText = nameElem->GetText();
				const char* dataText = dataElem->GetText();

				if (nameText && dataText)
				{
					NavPoint newNavPoint(nameText);

					// parseNextVec3 is your existing helper
					std::string dataStr(dataText);
					newNavPoint.pos = parseNextVec3(dataStr);
					newNavPoint.rot = parseNextVec3(dataStr);
					newNavPoint.scale = parseNextVec3(dataStr);

					newNavPoint.ID = CurrentNavPoint;
					CurrentNavPoint--; 
					NavSet.NavPoints.push_back(newNavPoint);
				}
				else
				{
					std::cerr << "Malformed <NavPoint> (missing text in <Name> or <Data>).\n";
				}
			}
			else
			{
				std::cerr << "Malformed <NavPoint> (missing <Name> or <Data> element).\n";
			}

			navPointElement = navPointElement->NextSiblingElement("NavPoint");
		}
	}

	std::cout << "Scene loaded: " << rootModel.children.size() << " top-level models, "
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
		cout << "Loading Model"<<*CurrentObject<<" PathName: " << pathName << " Data: " << data << endl;
		modelList->push_back(*new Model(pathName.string(), pathName.filename().string(), data));
		(*modelList)[count].ID = (*CurrentObject);
		(*CurrentObject)++;


		ti::XMLElement* modelChild = model->FirstChildElement("Model");
		if (modelChild)
			loadModel(modelChild, &((*modelList)[count].children), CurrentObject);

		cout << "Loaded:" << pathName.string() << endl;
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
		cout << "Saving model" << endl;
		*outfile << "<Model>";
		cout << model->toXML() << endl;
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
	std::ofstream outfile("..\\..\\Resources\\Levels\\Scene01.xml");
	if (!outfile)
	{
		std::cerr << "Failed to open Scene01.xml for writing." << std::endl;
		return;
	}

	std::cout << "Saving scene..." << std::endl;
	outfile << "<Scene>\n";

	// 1) Save all model data
	saveModel(&rootModel, &outfile);

	// 2) Save all NavPoints in a <NavSet> block
	outfile << "  <NavSet>\n";
	for (NavPoint& np : NavSet.NavPoints)
	{
		outfile << "    <NavPoint>\n";
		outfile << np.toXML() << "\n";   // e.g., <Name>, <Data>
		outfile << "    </NavPoint>\n";
	}
	outfile << "  </NavSet>\n";

	outfile << "</Scene>\n";
	outfile.close();

	std::cout << "Scene saved successfully (models + navpoints)." << std::endl;
}

void Scene::addNavPoint()
{
	if (NavSet.navModel == nullptr)
	{
		NavSet.init();
	}
	string name = "nav " + to_string(255 - CurrentNavPoint);
	NavSet.NavPoints.push_back(*new NavPoint(name));
	selected_model = nullptr;
	selectedNavPoint = &NavSet.NavPoints.back();
	selectedNavPoint->ID = CurrentNavPoint;
	CurrentNavPoint--;
}

bool Scene::selectNodeByID(int currentNodeID)
{
	for (NavPoint &NavPoint : NavSet.NavPoints)
	{
		if (NavPoint.ID == currentNodeID)
		{
			selectedNavPoint = &NavPoint;
			selected_model = nullptr;
			cout << "Node " << selectedNavPoint->name << " has been selected.";
			return true;
		}
	}
	return false;
}

glm::vec3 Scene::parseNextVec3(std::string& data)
{
	glm::vec3 result(0.0f);

	// Look for '(' and ')' to isolate "x, y, z"
	size_t start = data.find("(");
	size_t end = data.find(")");
	if (start == std::string::npos || end == std::string::npos)
		return result; // If malformed, return (0,0,0)

	// Extract the substring inside parentheses
	std::string inside = data.substr(start + 1, end - start - 1);

	// Erase this portion from 'data' so next call finds the next vec3
	data.erase(0, end + 1);

	// inside might look like "0.000000, 0.000000, 0.000000"
	std::replace(inside.begin(), inside.end(), ',', ' ');

	std::istringstream iss(inside);
	float x, y, z;
	iss >> x >> y >> z;
	result = glm::vec3(x, y, z);

	return result;
}

void Scene::clearNavSet()
{
	selectedNavPoint = nullptr;
	CurrentNavPoint = 255;
	NavSet.NavPoints.clear();
}

void Scene::removeNode()
{
	if (selectedNavPoint)
	{
		for (int NavPoint = 0; NavPoint < NavSet.NavPoints.size(); NavPoint++)
		{
			if (&NavSet.NavPoints[NavPoint] == selectedNavPoint)
			{
				NavSet.NavPoints.erase(NavSet.NavPoints.begin() + NavPoint);
				selectedNavPoint = nullptr;
				CurrentNavPoint++;
				break;
			}
		}

		for (int i = 0; i < NavSet.NavPoints.size(); i++)
		{
			NavSet.NavPoints[i].name = "nav " + std::to_string(i);
		}
	}
}

bool Scene::removeModelFromChildren(Model* parent, Model* toRemove)
{
	for (int i = 0; i < parent->children.size(); i++)
	{
		if (&parent->children[i] == toRemove)
		{
			parent->children.erase(parent->children.begin() + i);
			return true;
		}

		else
		{
			if (removeModelFromChildren(&parent->children[i], toRemove))
			{
				return true;
			}
		}
	}
	return false;
}

bool Scene::removeModel()
{
	if (!selected_model) 
	{
		return false;
	}
	if (selected_model->name == "SceneRoot")
	{
		return false;
	}

	bool removed = removeModelFromChildren(&rootModel, selected_model);

	if (removed)
	{
		selected_model = nullptr;
	}

	return removed;
}

;