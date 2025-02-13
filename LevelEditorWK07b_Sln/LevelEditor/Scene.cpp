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
void Scene::loadNavSet()
{

}
;