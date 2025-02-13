#include "Scene.h"

void Scene::load()
{
	rootModel.children.clear();
	CurrentObject = 3;
	selected_model = nullptr;
	ti::XMLDocument doc;
	doc.LoadFile("..\\..\\Resources\\Levels\\Scene01.xml");
	if (doc.Error())
		std::cerr << "Error parsing XML: " << doc.ErrorStr() << std::endl;
	else
	{
		ti::XMLElement* scene = doc.RootElement();
		ti::XMLElement* model = scene->FirstChildElement("Model");
		if (model) //the scene has at lease 1 valid model so load it
			loadModel(model, &rootModel.children, &CurrentObject);
	}
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
	ofstream outfile;
	cout << "Opening file" << endl;
	outfile.open("..\\..\\Resources\\Levels\\Scene01.xml");

	if (outfile) {
		//Scene start tag
		cout << "Saving scene" << endl;
		outfile << "<Scene>" << endl;
		saveModel(&rootModel, &outfile);
		//Scene end tag
		outfile << "</Scene>" << endl;
		outfile.close();
	}
	cout << "Done saving" << endl;
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
			return true;
		}
	}
	return false;
}

void Scene::saveNavSet()
{
	// Open "..\..\Resources\Levels\NavSet01.xml" for writing
	std::ofstream outfile("..\\..\\Resources\\Levels\\NavSet01.xml");
	if (outfile.is_open())
	{
		// Write the <NavSet> start tag
		outfile << "<NavSet>\n";

		// For each NavPoint
		for (NavPoint& np : NavSet.NavPoints)
		{
			outfile << "    <NavPoint>\n";
			// Use np.toXML() to write <Name> and <Data>
			outfile << np.toXML() << "\n";
			outfile << "    </NavPoint>\n";
		}

		// Write the </NavSet> end tag
		outfile << "</NavSet>\n";

		outfile.close();
		std::cout << "NavSet saved successfully.\n";
	}
	else
	{
		std::cerr << "Failed to open NavSet01.xml for writing.\n";
	}
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

void Scene::loadNavSet()
{
	// Clear NavPoints, reset CurrentNavPoint & selectedNavPoint
	NavSet.NavPoints.clear();
	NavSet.init();
	CurrentNavPoint = 255;
	selectedNavPoint = nullptr;

	// Load "..\..\Resources\Levels\NavSet01.xml"
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError result = doc.LoadFile("..\\..\\Resources\\Levels\\NavSet01.xml");
	if (result != tinyxml2::XML_SUCCESS)
	{
		std::cerr << "Error loading NavSet01.xml: " << doc.ErrorStr() << std::endl;
		return;
	}

	// Get root <NavSet>
	tinyxml2::XMLElement* navSetElement = doc.FirstChildElement("NavSet");
	if (!navSetElement)
	{
		std::cerr << "No <NavSet> element found in NavSet01.xml\n";
		return;
	}

	// First <NavPoint>
	tinyxml2::XMLElement* navPointElement = navSetElement->FirstChildElement("NavPoint");

	// While valid
	while (navPointElement)
	{
		// <Name> and <Data>
		tinyxml2::XMLElement* nameElem = navPointElement->FirstChildElement("Name");
		tinyxml2::XMLElement* dataElem = navPointElement->FirstChildElement("Data");

		if (nameElem && dataElem)
		{
			const char* nameText = nameElem->GetText();
			const char* dataText = dataElem->GetText();

			if (nameText && dataText)
			{
				// Create new NavPoint
				NavPoint newNavPoint(nameText);

				// Parse <Data> into pos, rot, scale
				std::string dataStr(dataText);
				newNavPoint.pos = parseNextVec3(dataStr);
				newNavPoint.rot = parseNextVec3(dataStr);
				newNavPoint.scale = parseNextVec3(dataStr);

				// Set ID, decrement
				newNavPoint.ID = CurrentNavPoint;
				CurrentNavPoint--;

				// Add to vector
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

		// Next sibling <NavPoint>
		navPointElement = navPointElement->NextSiblingElement("NavPoint");
	}

	std::cout << "Loaded " << NavSet.NavPoints.size()
		<< " NavPoints from NavSet01.xml\n";
}
;