#include "NavPoint.h"

std::string NavPoint::buildDataString(glm::vec3& pos, glm::vec3& rot, glm::vec3& scale)
{
    // Example output: "vec3(0.000000, 0.000000, 0.000000)vec3(...)vec3(...)"
    return glm::to_string(pos) + glm::to_string(rot) + glm::to_string(scale);
}

std::string NavPoint::toXML() 
{
    std::stringstream ss;

    // For example:
    //     <Name>nav0</Name>
    //     <Data>vec3(0,0,0)vec3(0,0,0)vec3(1,1,1)</Data>
    ss << "        <Name>" << name << "</Name>\n";
    ss << "        <Data>" << buildDataString(pos, rot, scale) << "</Data>";

    return ss.str();
}