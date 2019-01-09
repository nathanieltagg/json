#include "json.hpp"
#include <iostream>

nlohmann::json Jsigfigs(double value, unsigned int sig_figs=3){
  if(!std::isfinite(value)) { return nlohmann::json("nan"); }
  if(value==0) { return nlohmann::json(0); }
  
  // Find exponent
  int X = (int)floor(log10(value));
  unsigned int maxchar = sig_figs + 7;
  char* buff = new char[maxchar];
  if((X+1<sig_figs) ||  (X > sig_figs+4) ){
    // For most of the above cases, the %g format works well!
    snprintf(buff,maxchar,"%.*g",sig_figs,value);
  } else { // if (X <= S+2)   This rounds to an integer, which is the most efficient.
    snprintf(buff,maxchar,"%d",(int)value);
  }
  std::string s(buff);
  delete [] buff;
  return nlohmann::json(nlohmann::json::unquoted_string,s);
}

nlohmann::json Jfixed(double value, unsigned int precision=3){
  if(!std::isfinite(value)) { return nlohmann::json("nan"); }
  if(value==0) { return nlohmann::json(0); }
  
  const int maxchar = 30;
  char* buff = new char[maxchar];
  int p = snprintf(buff,maxchar,"%.*f",precision,value);
  p--;
  int dec = precision;
  while(buff[p] == '0' && dec>0) {
     dec--;
     buff[p]=0;
     p--;
  }
  if(buff[p]=='.') buff[p]=0;
  std::string s(buff);
  delete [] buff;
  return nlohmann::json(nlohmann::json::unquoted_string,s);
}

//
// struct fixed_identifier_t {};  // Syntatic sugar: allows overriding the constructor.
// static constexpr fixed_identifier_t fixed = fixed_identifier_t();
// basic_json(fixed_identifier_t, double value, int precision=3) : m_type(value_t::unquoted_string) {
//   if(!std::isfinite(value)) { m_value=std::string("\"nan\""); return; }
//   if(value==0) {m_value=std::string("0"); return;}
//   const int maxchar = 30;
//   char* buff = new char[maxchar];
//   int p = snprintf(buff,maxchar,"%.*f",precision,value);
//   p--;
//   int dec = precision;
//   while(buff[p] == '0' && dec>0) {
//      dec--;
//      buff[p]=0;
//      p--;
//   }
//   if(buff[p]=='.') buff[p]=0;
//   m_value = std::string(buff);
//   delete [] buff;
//   // std::ostringstream oss;
//   // oss << std::fixed << std::setprecision(precision) << value;
//   // std::string tmp = oss.str();
//   // int dec = precision;
//   // // remove trailing 0s after the decimal point.
//   // while(*tmp.rbegin()=='0' && dec>0) {
//   //   dec--;
//   //   tmp.erase(tmp.end()-1);
//   // }
//   // // remove trailing decimal point.
//   // if(*tmp.rbegin()=='.') tmp.erase(tmp.end()-1);
//   // m_value = tmp;
// }
//

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
  j["unquoted"] = json(json::unquoted_string,"{ \"inline_sub_object\": \"abcd\" }");

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
    o["fixed_1"] = Jfixed(a,1);
    o["fixed_3"] = Jfixed(a,3);
    o["sigfig2"] = Jsigfigs(a,2);
    o["sigfig3"] = Jsigfigs(a,3);

    // This fails!
    //   double b = o["sigfig2"].get<double>();
    // std::cout << a << " readback: " << b;
    arr.push_back(o);    
  }
  j["tests"] = arr;
  
  std::cout << j.dump(4) << std::endl;
  
}