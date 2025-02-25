#pragma once

#ifndef MODEL_H
#include "Model.h"
#endif
#include <tinyxml2.h>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "NavSet.h"

namespace ti = tinyxml2;
namespace fs = std::filesystem;
using namespace std;

class Scene
{
public:
	NavSet NavSet; // An instance of the NavSet.
	int CurrentObject=3;
	int CurrentNavPoint = 255; // Int counter for the NavPoint.
	Model* selected_model = nullptr;
	NavPoint* selectedNavPoint = nullptr; // NavPoint to hold the currently selected NavPoint.
	Model rootModel= Model("SceneRoot");
	void load();
	void loadModel(ti::XMLElement* model, vector<Model>* modelList, int* CurrentObject);
	void saveModel(Model* model, ofstream* outfile);
	void save();
	bool selectModelByID(Model* model, int CurrentObjectID);
	void addNavPoint(); // Function prototype to add a NavPoint
	bool selectNodeByID(int currentNodeID); // Function Prototype for selecting a node via the ID.
	glm::vec3 parseNextVec3(std::string& data); // Function Prototype for Parsing Vec3 for loading purposes.
	void clearNavSet(); // Function prototype to clear every NavPoint from the NavSet.
	void removeNode(); // Function prototype to remove a selected Node.
	bool removeModelFromChildren(Model* parent, Model* toRemove); // Function prototype to remove model from children.
	bool removeModel(); // Function prototype to remove a selected Model.
	void clearModels(); // Function prototype to clear every model.
};

