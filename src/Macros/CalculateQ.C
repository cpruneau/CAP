
#include "TString.h"
#include "TRandom.h"
#include <vector>

class Term()
{
  Term();
  Term(double _coefficient,
       const TString & _variable,
       const TString & _exponent);
  Term multiply(const Term & t1, const Term & t2);
  Term multiply(double coefficient);

  ~Term() {}

  double coefficient;
  vector<TString> factors;
  vector<TString> exponents;

}

Term::Term()
:
coefficient(0.0),
factors(),
exponents()
{
}

Term::Term(double _coefficient,
           const TString & _variable,
           const TString & _exponent)
:
coefficient(_coefficient),
factors(),
exponents()
{
  factors.push_back(_variable);
  exponents.push_back(_exponent);
}

Term multiply(const Term & t1, const Term & t2)
{
  Term newTerm;
  newTerm.coefficient = t1.coefficient * t2.coefficient;
  for (int k=0;k<t1.factors.size(); k++)
    {
    newTerm.factors.push_back(t1.factors[k]);
    newTerm.exponents.push_back(t1.exponents[k]);
    }
  for (int k=0;k<t2.factors.size(); k++)
    {
    newTerm.factors.push_back(t2.factors[k]);
    newTerm.exponents.push_back(t2.exponents[k]);
    }
  return newTerm;
}

Term multiply(double _coefficient)
{
  coefficient *= _coefficient;
  return *this;
}



class Formula()
{
  Formula();
  ~Formula() {}
  void multiply(double a);
  void multiply(const Term & t);
  void add(const Term & t, double a);

  vector<Term> terms;

}

Formula::Formula()
:
terms()
{ }

void Formula::multiply(double a)
{
  for (int k=0; k<terms.size(); k++)
    {
    terms[k].coefficient *= a;
    }
}

void Formula::multiply(const Term & t)
{
  for (int k=0; k<terms.size(); k++)
    {
    terms[k].multiply(t);
    }
}

void Formula::add(const Term & t, double a)
{
  Term newTerm(t);
  newTerm.coefficient *= a;
  terms.push_back(newTerm);
}




TString Qn1(const TString & n1)
{
  TString answer = "Q_{"; answer += n1; answer += "}";
  return answer;
}

TString Qn1Qn2(const TString & n1, const TString & n2)
{
  TString answer = Qn1(n1); answer += Qn1(n2);
  return answer;
}

TString Qn1Qn2Qn3(const TString & n1, const TString & n2, const TString & n3)
{
  TString answer = Qn1Qn2(n1,n2); answer += Qn1(n3);
  return answer;
}

TString Qn1Qn2Qn3Qn4(const TString & n1, const TString & n2, const TString & n3, const TString & n4)
{
  TString answer = Qn1Qn2Qn3(n1,n2,n3); answer += Qn1(n4);
  return answer;
}

TString Qn1Qn2Qn3Qn4Qn5(const TString & n1, const TString & n2, const TString & n3, const TString & n4, const TString & n5)
{
  TString answer = Qn1Qn2Qn3Qn4(n1,n2,n3,n4); answer += Qn1(n5);
  return answer;
}

TString Qn1Qn2Qn3Qn4Qn5Qn6(const TString & n1, const TString & n2, const TString & n3, const TString & n4, const TString & n5, const TString & n6)
{
  TString answer = Qn1Qn2Qn3Qn4Qn5(n1,n2,n3,n4,n5); answer += Qn1(n6);
  return answer;
}

TString Sum_qn1(const TString & n1)
{
  TString answer = "\\sum_{i1} q_{i1}^{";
  answer += n1;
  answer += "}";
  return answer;
}

TString Sum_qn1_qn2(const TString & n1, const TString & n2)
{
  TString answer = "\\sum_{i1 \\ne i2} q_{i1}^{";
  answer += n1;
  answer += "} q_{i2}^{";
  answer += n2;
  answer += "}";
  return answer;
}

TString Sum_qn1_qn2_qn3(const TString & n1, const TString & n2, const TString & n3)
{
  TString answer = "\\sum_{i1 \\ne i2  \\ne i3} q_{i1}^{";
  answer += n1;
  answer += "} q_{i2}^{";
  answer += n2;
  answer += "} q_{i3}^{";
  answer += n3;
  answer += "}";
  return answer;
}

TString Sum_qn1_qn2_qn3_qn4(const TString & n1, const TString & n2, const TString & n3, const TString & n4)
{
  TString answer = "\\sum_{i1 \\ne i2  \\ne i3 \\ne i4} q_{i1}^{";
  answer += n1;
  answer += "} q_{i2}^{";
  answer += n2;
  answer += "} q_{i3}^{";
  answer += n3;
  answer += "} q_{i4}^{";
  answer += n4;
  answer += "}";
  return answer;
}

TString Sum_qn1_qn2_qn3_qn4_qn5(const TString & n1, const TString & n2, const TString & n3, const TString & n4, const TString & n5)
{
  TString answer = "\\sum_{i1 \\ne i2  \\ne i3 \\ne i4 \\ne i5} q_{i1}^{";
  answer += n1;
  answer += "} q_{i2}^{";
  answer += n2;
  answer += "} q_{i3}^{";
  answer += n3;
  answer += "} q_{i4}^{";
  answer += n4;
  answer += "} q_{i5}^{";
  answer += n5;
  answer += "}";
  return answer;
}

TString Sum_qn1_qn2_qn3_qn4_qn5_qn6(const TString & n1, const TString & n2, const TString & n3, const TString & n4, const TString & n5, const TString & n6)
{
  TString answer = "\\sum_{i1 \\ne i2  \\ne i3 \\ne i4 \\ne i5 \\ne i6} q_{i1}^{";
  answer += n1;
  answer += "} q_{i2}^{";
  answer += n2;
  answer += "} q_{i3}^{";
  answer += n3;
  answer += "} q_{i4}^{";
  answer += n4;
  answer += "} q_{i5}^{";
  answer += n5;
  answer += "} q_{i6}^{";
  answer += n6;
  answer += "}";
  return answer;
}

TString Parenthesis(const TString & s)
{
  TString answer = "("; answer += s; answer += ")";
  return answer;
}

TString Plus(const TString & n1, const TString & n2)
{
  TString answer = n1; answer += "+"; answer += n2;
  return answer;
}

TString Minus(const TString & n1, const TString & n2, bool paren=false)
{
  TString answer = n1; answer += "-";
  if (paren)
    answer += Parenthesis(n2);
  else
    answer += n2;
  return answer;
}


TString Calculate_Sum_qn1(const TString & n1)
{
  TString answer = Qn1(n1);
  return answer;
}

TString Calculate_Sum_qn1_qn2(const TString & n1, const TString & n2)
{
  TString n1n2 = Plus(n1,n2);
  TString answer = Qn1Qn2(n1,n2);
  answer = Minus(answer,Qn1(n1n2));
  return answer;
}

TString Calculate_Sum_qn1_qn2_qn3(const TString & n1, const TString & n2, const TString & n3)
{
  TString n1n2 = Plus(n1,n2);
  TString n1n3 = Plus(n1,n3);
  TString n2n3 = Plus(n2,n3);
  TString n1n2n3 = Plus(n1n2,n3);
  TString answer = Qn1Qn2Qn3(n1,n2,n3);
  answer = Minus(answer, Qn1(n1n2n3));
  answer = Minus(answer, Calculate_Sum_qn1_qn2(n1n2,n3),1);
  answer = Minus(answer, Calculate_Sum_qn1_qn2(n1n3,n2),1);
  answer = Minus(answer, Calculate_Sum_qn1_qn2(n1,n2n3),1);
  return answer;
}

TString Calculate_Sum_qn1_qn2_qn3_qn4(const TString & n1, const TString & n2, const TString & n3, const TString & n4)
{
  TString n1n2 = Plus(n1,n2);
  TString n1n3 = Plus(n1,n3);
  TString n1n4 = Plus(n1,n4);
  TString n2n3 = Plus(n2,n3);
  TString n2n4 = Plus(n2,n4);
  TString n3n4 = Plus(n3,n4);
  TString n1n2n3 = Plus(n1n2,n3);
  TString n1n2n4 = Plus(n1n2,n4);
  TString n1n3n4 = Plus(n1n3,n4);
  TString n2n3n4 = Plus(n2n3,n4);
  TString n1n2n3n4 = Plus(n1n2n3,n4);
  TString answer = Qn1Qn2Qn3Qn4(n1,n2,n3,n4);
  answer = Minus(answer, Qn1(n1n2n3n4));
  answer = Minus(answer, Calculate_Sum_qn1_qn2(n1n2n3,n4),1);
  answer = Minus(answer, Calculate_Sum_qn1_qn2(n1n2n4,n3),1);
  answer = Minus(answer, Calculate_Sum_qn1_qn2(n1n2,n3n4),1);
  answer = Minus(answer, Calculate_Sum_qn1_qn2_qn3(n1n2,n3,n4),1);
  answer = Minus(answer, Calculate_Sum_qn1_qn2(n1n3n4,n2),1);
  answer = Minus(answer, Calculate_Sum_qn1_qn2(n1n3,n2n4),1);
  answer = Minus(answer, Calculate_Sum_qn1_qn2_qn3(n1n3,n2,n4),1);

  answer = Minus(answer, Calculate_Sum_qn1_qn2(n1n3,n2n4),1);
  answer = Minus(answer, Calculate_Sum_qn1_qn2(n1,n2n3n4),1);
  answer = Minus(answer, Calculate_Sum_qn1_qn2_qn3(n1,n2n3,n4),1);

  answer = Minus(answer, Calculate_Sum_qn1_qn2_qn3(n1n3,n2,n4),1);
  answer = Minus(answer, Calculate_Sum_qn1_qn2_qn3(n1,n2n3,n4),1);
  answer = Minus(answer, Calculate_Sum_qn1_qn2_qn3(n1,n2,n3n4),1);
  return answer;
}



int CalculateQ()
{
  TString n1 = "n1";
  TString n2 = "n2";
  TString n3 = "n3";
  TString n4 = "n4";
  TString n5 = "n5";
  TString n6 = "n6";

  cout << "                Qn1: " <<  Qn1(n1) << endl;
  cout << "             Qn1Qn2: " <<  Qn1Qn2(n1,n2)  << endl;
  cout << "          Qn1Qn2Qn3: " <<  Qn1Qn2Qn3(n1,n2,n3) << endl;
  cout << "       Qn1Qn2Qn3Qn4: " <<  Qn1Qn2Qn3Qn4(n1,n2,n3,n4) << endl;
  cout << "    Qn1Qn2Qn3Qn4Qn5: " <<  Qn1Qn2Qn3Qn4Qn5(n1,n2,n3,n4,n5) << endl;
  cout << " Qn1Qn2Qn3Qn4Qn5Qn6: " <<  Qn1Qn2Qn3Qn4Qn5Qn6(n1,n2,n3,n4,n5,n6) << endl;

  cout << Sum_qn1(n1)  << " = "  << Calculate_Sum_qn1(n1) << endl;
  cout << Sum_qn1_qn2(n1,n2)   << " = "  << Calculate_Sum_qn1_qn2(n1,n2) << endl;
  cout << Sum_qn1_qn2_qn3(n1,n2,n3)  << " = "  << Calculate_Sum_qn1_qn2_qn3(n1,n2,n3) << endl;
  cout << Sum_qn1_qn2_qn3_qn4(n1,n2,n3,n4)  << " = "  << Calculate_Sum_qn1_qn2_qn3_qn4(n1,n2,n3,n4) << endl;
  cout << Sum_qn1_qn2_qn3_qn4_qn5(n1,n2,n3,n4,n5)  << endl;
  cout << Sum_qn1_qn2_qn3_qn4_qn5_qn6(n1,n2,n3,n4,n5,n6)  << endl;

  return 0;
}


