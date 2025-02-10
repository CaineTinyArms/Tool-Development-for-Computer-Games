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

namespace ti = tinyxml2;
namespace fs = std::filesystem;
using namespace std;

class Scene
{
public:
	int CurrentObject=0;
	Model* selected_model = nullptr;
	Model rootModel= Model("SceneRoot");
	void load();
	void loadModel(ti::XMLElement* model, vector<Model>* modelList, int* CurrentObject);
	void saveModel(Model* model, ofstream* outfile);
	void save();
	bool selectModelByID(Model* model, int CurrentObjectID);
};

