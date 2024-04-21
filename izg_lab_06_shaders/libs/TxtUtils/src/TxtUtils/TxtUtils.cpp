#include<TxtUtils/TxtUtils.h>
#include<fstream>
#include<iostream>
#include<string>
#include<set>
#include<MealyMachine/MealyMachine.h>

using namespace mealyMachine;

std::string txtUtils::loadTextFile(std::string const&fileName){
  std::ifstream f(fileName.c_str());
  if(!f.is_open()){
    throw std::runtime_error(std::string("cannot open file: ")+fileName);
    return "";
  }
  std::string str((std::istreambuf_iterator<char>(f)),std::istreambuf_iterator<char>());
  f.close();
  return str;
}

bool txtUtils::isNan(std::string const&text){
  return std::set<std::string>({"NAN","Nan","nan"}).count(text)>0;
}

bool txtUtils::isInfinity(std::string const&text){
  return std::set<std::string>({"inf","Inf","INF","+inf","+Inf","+INF","-inf","-Inf","-INF"}).count(text)>0;
}


bool txtUtils::isFloat(std::string const&text){
  if(isNan(text))return true;
  if(isInfinity(text))return true;
  MealyMachine mm;

  auto start            = mm.addState();
  auto sign             = mm.addState();
  auto immediateDot     = mm.addState();
  auto fractionalNumber = mm.addState();
  auto wholeNumber      = mm.addState();
  auto exponent         = mm.addState();
  auto postfix          = mm.addState();
  auto exponentSign     = mm.addState();
  auto exponentNumber   = mm.addState();

  mm.addTransition   (start           ,"+-"   ,sign            );
  mm.addTransition   (start           ,"."    ,immediateDot    );
  mm.addTransition   (start           ,"0","9",wholeNumber     );
  mm.addTransition   (sign            ,"."    ,immediateDot    );
  mm.addTransition   (sign            ,"0","9",wholeNumber     );
  mm.addTransition   (immediateDot    ,"0","9",fractionalNumber);
  mm.addTransition   (wholeNumber     ,"0","9",wholeNumber     );
  mm.addTransition   (wholeNumber     ,"."    ,fractionalNumber);
  mm.addTransition   (wholeNumber     ,"fF"   ,postfix         );
  mm.addTransition   (wholeNumber     ,"eE"   ,exponent        );
  mm.addTransition   (fractionalNumber,"0","9",fractionalNumber);
  mm.addTransition   (fractionalNumber,"fF"   ,postfix         );
  mm.addTransition   (fractionalNumber,"eE"   ,exponent        );
  mm.addEOFTransition(postfix                                  );
  mm.addTransition   (exponent        ,"+-"   ,exponentSign    );
  mm.addTransition   (exponent        ,"0","9",exponentNumber  );
  mm.addTransition   (exponentSign    ,"0","9",exponentNumber  );
  mm.addTransition   (exponentNumber  ,"0","9",exponentNumber  );
  mm.addTransition   (exponentNumber  ,"fF"   ,postfix         );
  mm.setQuiet(true);

  return mm.match(text.c_str());
}

bool txtUtils::isDouble(std::string const&text){
  if(isNan(text))return true;
  if(isInfinity(text))return true;
  MealyMachine mm;

  auto start            = mm.addState();
  auto sign             = mm.addState();
  auto immediateDot     = mm.addState();
  auto fractionalNumber = mm.addState();
  auto wholeNumber      = mm.addState();
  auto exponent         = mm.addState();
  auto exponentSign     = mm.addState();
  auto exponentNumber   = mm.addState();

  mm.addTransition   (start           ,"+-"   ,sign            );
  mm.addTransition   (start           ,"."    ,immediateDot    );
  mm.addTransition   (start           ,"0","9",wholeNumber     );
  mm.addTransition   (sign            ,"."    ,immediateDot    );
  mm.addTransition   (sign            ,"0","9",wholeNumber     );
  mm.addTransition   (immediateDot    ,"0","9",fractionalNumber);
  mm.addTransition   (wholeNumber     ,"0","9",wholeNumber     );
  mm.addTransition   (wholeNumber     ,"."    ,fractionalNumber);
  mm.addTransition   (wholeNumber     ,"eE"   ,exponent        );
  mm.addEOFTransition(wholeNumber                              );
  mm.addTransition   (fractionalNumber,"0","9",fractionalNumber);
  mm.addTransition   (fractionalNumber,"eE"   ,exponent        );
  mm.addEOFTransition(fractionalNumber                         );
  mm.addTransition   (exponent        ,"+-"   ,exponentSign    );
  mm.addTransition   (exponent        ,"0","9",exponentNumber  );
  mm.addTransition   (exponentSign    ,"0","9",exponentNumber  );
  mm.addTransition   (exponentNumber  ,"0","9",exponentNumber  );
  mm.addEOFTransition(exponentNumber                           );
  mm.setQuiet(true);

  return mm.match(text.c_str());

}

bool txtUtils::isFloatingPoint(std::string const&text){
  return isFloat(text) || isDouble(text);
}

bool txtUtils::isIntegral(std::string const&text){
  return isInt(text) || isUint(text);
}

bool txtUtils::isInt(std::string const&text){
  MealyMachine mm;

  auto start = mm.addState();
  auto sign  = mm.addState();
  auto digs  = mm.addState();

  mm.addTransition(start,"+-"   ,sign);
  mm.addTransition(start,"0","9",digs);

  mm.addTransition(sign,"0","9",digs);

  mm.addTransition(digs,"0","9",digs);
  mm.addEOFTransition(digs);
  mm.setQuiet(true);

  return mm.match(text.c_str());
}

bool txtUtils::isUint(std::string const&text){
  MealyMachine mm;

  auto start = mm.addState();
  auto sign  = mm.addState();
  auto digs  = mm.addState();

  mm.addTransition(start,"+"    ,sign);
  mm.addTransition(start,"0","9",digs);

  mm.addTransition(sign,"0","9",digs);

  mm.addTransition(digs,"0","9",digs);
  mm.addEOFTransition(digs);
  mm.setQuiet(true);

  return mm.match(text.c_str());
}

bool txtUtils::isString(std::string const&){
  return true;
}
