#ifndef GCM_UTILS_H_
#define GCM_UTILS_H_

#include <fstream>
#include <map>
#include <string>

#include <libxml++/libxml++.h>

#include "Exception.h"

/*
 * Foreach macro to iterate over STL containers.
 */
#define foreach(i, v) for (typeof(v.begin()) i = v.begin(); i != v.end(); i++)

using namespace std;

namespace gcm {
	/*
	 * Class to hold params while loading tasks.
	 */

	class Params: public map<string, string> {
		public:
			/*
			 * Constructor
			 */
			Params(xmlpp::Element* el);
			/*
			 * Checks if specified key is in params.
			 */
			bool has(string param);
			/*
			 * Checks if value stored in map equals to specifie one.
			 */
			bool paramEquals(string param, string value);
	 };

}

/*
 * Checks if previous read operation succeeded.
 */
void checkStream(fstream &f);

/*
 * Returns value of named attribute.
 */
string getAttributeByName(xmlpp::Element::AttributeList attrs, string name);

void replaceAll(std::string& str, const std::string& from, const std::string& to);

template<class T>
static std::string t_to_string(T i)
{
	std::stringstream ss;
	std::string s;
	ss << i;
	s = ss.str();
	return s;
}

#endif
