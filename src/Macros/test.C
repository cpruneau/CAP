#include <iostream>

//template < class T >
//class Parameter : public TObject
//{
//public:
//
//  Parameter(const TString & _name, T _value)
//  :
//  name(_name),
//  value(_value)
//  {
//  // no ops
//  }
//
//  virtual ~Parameter() {}
//
//  Parameter(const Parameter & cp)
//  :
//  name(cp.name),
//  value(cp.bValue)
//  {}
//
//  Parameter & operator=(const Parameter & cp)
//  {
//  if (this != &cp)
//    {
//    name = cp.name;
//    value = cp.value;
//    }
//  return *this;
//  }
//
//  const TString & getName() const { return name; }
//  T   getValue() const     { return value; }
//
//  void setName(const TString & _name) { name = _name;    }
//  void setValue(T _value)      { value = _value; }
//
//protected:
//
//  TString name;
//  T       value;
//};

class TestConfiguration
{
public:
  
  TestConfiguration()
  :
  boolMap(),
  intMap(),
  doubleMap(),
  stringMap()
  {
  
  }

  TestConfiguration(const TestConfiguration & config)
  :
  boolMap(config.boolMap),
  intMap(config.intMap),
  doubleMap(config.doubleMap),
  stringMap(config.stringMap)
  {

  }

  
  TestConfiguration & operator=(const TestConfiguration & config)
  {
  if (this!=&config)
    {
    boolMap   = config.boolMap;
    intMap    = config.intMap;
    doubleMap = config.doubleMap;
    stringMap = config.stringMap;
    }
  return *this;
  }
  
  void clear()
  {
  boolMap.clear();
  intMap.clear();
  doubleMap.clear();
  stringMap.clear();
  }

  void addParameter(const TString & name, bool value)
  {
  std::map<TString,bool>::iterator it;
  it = boolMap.find(name);
  if (it != boolMap.end())
    {
    it->second = value;
    }
  else
    {
    boolMap[name] = value;
    }
  }

  void addParameter(const TString & name, int value)
  {
  std::map<TString,int>::iterator it;
  it = intMap.find(name);
  if (it != intMap.end())
    {
    it->second = value;
    }
  else
    {
    intMap[name] = value;
    }
  }
  
  void addParameter(const TString & name, double value)
  {
  std::map<TString,double>::iterator it;
  it = doubleMap.find(name);
  if (it != doubleMap.end())
    {
    it->second = value;
    }
  else
    {
    doubleMap[name] = value;
    }
  }
  
  void addParameter(const TString & name, const TString &  value)
  {
  std::map<TString,TString>::iterator it;
  it = stringMap.find(name);
  if (it != stringMap.end())
    {
    it->second = value;
    }
  else
    {
    stringMap[name] = value;
    }
  }
  
  bool getValueBool(const TString & name)
  {
  bool value = false;
  std::map<TString,bool>::iterator it;
  it = boolMap.find(name);
   if (it != boolMap.end())
     {
     cout << " Key:" <<  name << " value:" <<  it->second << endl;
     value = it->second;
     }
  else
    {
    cout << " did not find key:" << name << endl;
    value = false;
    }
  return value;
  }
  
  int getValueInt(const TString & name)
  {
  int value = -99999;
  std::map<TString,int>::iterator it;
  it = intMap.find(name);
   if (it != intMap.end())
     {
     cout << " Key:" <<  name << " value:" <<  it->second << endl;
     value = it->second;
     }
  else
    {
    cout << " did not find key:" << name << endl;
    }
  return value;
  }

  double getValueDouble(const TString & name)
  {
  double value = -9.99E100;
  std::map<TString,double>::iterator it;
  it = doubleMap.find(name);
   if (it != doubleMap.end())
     {
     cout << " Key:" <<  name << " value:" <<  it->second << endl;
     value = it->second;
     }
  else
    {
    cout << " did not find key:" << name << endl;
    }
  return value;
  }

  TString  getValueString(const TString & name)
  {
  TString value = "notFound";
  std::map<TString,TString>::iterator it;
  it = stringMap.find(name);
   if (it != stringMap.end())
     {
     cout << " Key:" <<  name << " value:" <<  it->second << endl;
     value = it->second;
     }
  else
    {
    cout << " did not find key:" << name << endl;
    }
  return value;
  }

protected:
  
  std::map<TString,bool>    boolMap;
  std::map<TString,int>     intMap;
  std::map<TString,double>  doubleMap;
  std::map<TString,TString> stringMap;
};


int test()
{
  TestConfiguration config;
  
  config.addParameter("aa",true);
  config.addParameter("bb",2);
  config.addParameter("cc",4);
  cout << config.getValueInt("cc") << endl;
  config.addParameter("cc",66);
  cout << config.getValueInt("cc") << endl;
  config.addParameter("cc",666);
  cout << config.getValueInt("cc") << endl;
  config.addParameter("cc",6666);
  cout << config.getValueInt("cc") << endl;
  
  config.addParameter("dd",3.1415927);
  cout << config.getValueDouble("dd") << endl;
  config.addParameter("dd",6.2815927);
  cout << config.getValueDouble("dd") << endl;
  config.addParameter("dd",1119.2815927);
  cout << config.getValueDouble("dd") << endl;
  config.addParameter("dd",9.2815927);
  cout << config.getValueDouble("dd") << endl;

  config.clear();
  cout << config.getValueDouble("dd") << endl;
  config.addParameter("ee",1.0E20);
  config.addParameter("ss",TString("somethingSaid"));

  cout << config.getValueBool("aa") << endl;
  cout << config.getValueInt("cc") << endl;
  cout << config.getValueDouble("dd") << endl;
  cout << config.getValueDouble("ee") << endl;
  cout << config.getValueString("ss") << endl;

  return 0;
}

