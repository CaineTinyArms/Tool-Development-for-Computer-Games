#include "NavPoint.h"

std::string NavPoint::buildDataString(glm::vec3& pos, glm::vec3& rot, glm::vec3& scale)
{
    return glm::to_string(pos) + glm::to_string(rot) + glm::to_string(scale); // Returns the information from the Vec3 as a string for saving purposes.
}

std::string NavPoint::toXML() 
{
    std::stringstream ss; 
    ss << "        <Name>" << name << "</Name>\n"; // Stores the name variable between the Name Tag for XML.
    ss << "        <Data>" << buildDataString(pos, rot, scale) << "</Data>"; // Stores the string returned from the buildDataString between the Data Tag for XML.

    return ss.str(); // Returns the Name and Data.
}