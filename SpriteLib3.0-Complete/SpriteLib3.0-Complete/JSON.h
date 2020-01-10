#ifndef __JSON_H__
#define __JSON_H__

#include <nlohmann/json.hpp>
#include <fstream>

namespace File
{
	//The default directory for json files
	static std::string defaultDir = "./assets/configs/";

	//Loads in the file and places it in a json object and returns the object
	nlohmann::json LoadJSON(std::string fileName);

	//Takes a json and a file name and outputs the dat within the json object to the file
	void CreateJSON(std::string fileName, nlohmann::json j);

}

#endif // !__JSON_H__
