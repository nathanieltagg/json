#include <sstream>
#include <iomanip>
#include "json.hpp"
#include <iostream>

int main(void)
{
  using json = nlohmann::json;
  // create an empty structure (null)
  json j;

  // // add a number that is stored as double (note the implicit conversion of j to an object)
  // j["pi"] = 3.141;
  //
  // // add a Boolean that is stored as bool
  // j["happy"] = true;
  //
  // // add a string that is stored as std::string
  // j["name"] = "Niels";
  //
  // // add another null object by passing nullptr
  // j["nothing"] = nullptr;
  //
  // // add an object inside the object
  // j["answer"]["everything"] = 42;
  //
  // // add an array that is stored as std::vector (using an initializer list)
  // j["list"] = { 1, 0, 2, 1./3. };
  //
  // // add another object (using an initializer list of pairs)
  // j["object"] = { {"currency", "USD"}, {"value", 42.99} };

  // json un(json::value_t::unquoted_string) = "abcd";
  // j["unquoted"] = un;
  j["unquoted"] = json(json::unquoted_string,"{ \"custom_subobject\": \"abcd\" }");

  // json arr;
  // arr.push_back(0);
  // arr.push_back("blah");
  // arr.push_back(0);
  // arr.push_back(json(json::unquoted_string,"1.234"));  
  // j["arr"] = arr;

  json arr;
  std::vector<double> list = {  0.0,
                                0.123, 10, 123.0, 123.456, 12.3456789e8, 12.3456789e-8,  12.34e8,
                               -0.123, -12.3456789e8,-12.3456789e-8,
                               1./0.
                              };
  for(double a: list) {
    json o;
    o["default"]= a;
    o["fixed_1"] = json(json::fixed,a,1);
    o["fixed_3"] = json(json::fixed,a,3);
    o["sigfig2"] = json(json::sigfig,a,2);
    o["sigfig3"] = json(json::sigfig,a,3);
    arr.push_back(o);    
  }
  j["tests"] = arr;
  
  std::cout << j.dump(4) << std::endl;
  
}