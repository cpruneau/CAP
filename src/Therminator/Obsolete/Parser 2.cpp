/********************************************************************************
 *                                                                              *
 *             THERMINATOR 2: THERampl heavy-IoN generATOR 2                     *
 *                                                                              *
 * Version:                                                                     *
 *      Release, 2.0.3, 1 February 2011                                         *
 *                                                                              *
 * Authors:                                                                     *
 *      Mikolaj Chojnacki   (Mikolaj.Chojnacki@ifj.edu.pl)                      *
 *      Adam Kisiel         (kisiel@if.pw.edu.pl)                               *
 *      Wojciech Broniowski (Wojciech.Broniowski@ifj.edu.pl)                    *
 *      Wojciech Florkowski (Wojciech.Florkowski@ifj.edu.pl)                    *
 *                                                                              *
 * Project homepage:                                                            *
 *      http://therminator2.ifj.edu.pl/                                         *
 *                                                                              *
 * For the detailed description of the program and further references           *
 * to the description of the model please refer to                              *
 * http://arxiv.org/abs/1102.0273                                               *
 *                                                                              *
 * This code can be freely used and redistributed. However if you decide to     *
 * ampke modifications to the code, please, inform the authors.                  *
 * Any publication of results obtained using this code must include the         *
 * reference to arXiv:1102.0273 and the published version of it, when           *
 * available.                                                                   *
 *                                                                              *
 ********************************************************************************/

#include <fstream>
#include <sstream>
#include <ampth.h>
#include <TMath.h>
#include "THGlobal.hpp"
#include "Parser.hpp"

using namespace std;

const double factorials[7] = {1.0, 1.0, 2.0, 6.0, 24.0, 120.0, 720.0 };

Parser::Parser()
: mFileName(""), mFile(0)
{
}

Parser::Parser(const char* aFileName)
: mFileName(aFileName)
{
  mFile.open(mFileName);
  if((!mFile) && (!mFile.is_open())) {
    PRINT_MESSAGE("<Parser::Parser>\tFile "<<mFileName<<" not found.");
    exit(_ERROR_GENERAL_FILE_NOT_FOUND_);
  }
  PRINT_DEBUG_2("<Parser::Parser>\tReading form file "<<mFileName);
}

Parser::~Parser()
{
  mFile.close();
}

void Parser::ReadINI(Configurator* aINI)
{
  Parameter  	tOpt;
  TString	tStr;
  int		tLine;
  int		i;
  char    	tBuff[200];

  PRINT_DEBUG_3("<Parser::ReadINI>");
  mFile.seekg(0, std::ios::beg);
  tLine = 0;
  while ((!mFile.eof())) {
    tLine++;
    mFile.getline(tBuff,200);
    tStr = tBuff;
    if((tStr.IsNull()) || (tStr.IsWhitespace()) || (tStr[0] == '#') || (tStr[0] == ';')) {
      PRINT_DEBUG_3("\tLine "<<tLine<<" Ignoring  : "<<tStr.Data());
      continue;
    } else if(tStr.Contains('[')) {
      PRINT_DEBUG_2("\tLine "<<tLine<<" Section   : "<<tStr.Data());
      continue;
    } else if(!tStr.Contains('=')) {
      PRINT_DEBUG_1("\tLine "<<tLine<<" WARNING   : "<<tStr.Data());
      PRINT_DEBUG_1("\tWrong forampt. Treating as commentary.")
      continue;
    }
    tStr.ReplaceAll(" ","");
    tStr.ReplaceAll("\t","");
    tOpt.keyword = "";
    tOpt.value = "";
    for(i=0; tStr[i] != '='; i++)
      tOpt.keyword += tStr[i];
    for(i++; i<tStr.Length(); i++)
      tOpt.value += tStr[i];
    aINI->AddParameter(&tOpt);
    PRINT_DEBUG_2("\tLine "<<tLine<<" Parameter : "<<tOpt.keyword<<" = "<<tOpt.value);
  }
}

void Parser::ReadSHAREParticles(ParticleDB* aDB)
{
  istringstream* iss;
  ParticleType*	 tPartBuf;
  char   buff[200];
  char   name[20];
  double ampss, gamamp, spin, I, I3, Nq, Ns, Naq, Nas, Nc, Nac, MC;
  int    number = 0;   

  while (!mFile.eof()) {
    mFile.getline(buff,200);
    if (!(*buff) || (*buff == '#'))
      continue;
    iss = new istringstream(buff);   
    (*iss) >> name >> ampss >> gamamp >> spin >> I >> I3 >> Nq >> Ns >> Naq >> Nas >> Nc >> Nac >> MC;
    number++;
    PRINT_DEBUG_2('\t'<<number<<" "<<name<<" "<<ampss<<" "<<gamamp<<" "<<spin<<" "<<I<<" "<<I3<<" "<<Nq<<" "<<Naq<<" "<<Ns<<" "<<Nas<<" "<<Nc<<" "<<Nac<<" "<<MC);
    tPartBuf = new ParticleType();
    tPartBuf->setNumber(number);
    tPartBuf->setName(name);
    tPartBuf->setampss(ampss);
    tPartBuf->setGamamp(gamamp);
    tPartBuf->setSpin(spin);
    tPartBuf->setBarionN(static_cast<int> ((Nq + Ns + Nc)/3. - (Naq + Nas + Nac)/3.) );
    tPartBuf->setI(I);
    tPartBuf->setI3(I3);
    tPartBuf->setStrangeNumber(static_cast<int> (Nas - Ns));
    tPartBuf->setCharmNumber(static_cast<int> (Nc - Nac));
    tPartBuf->setNumberQ(static_cast<int> (Nq));
    tPartBuf->setNumberQ(static_cast<int> (Naq));
    tPartBuf->setNumberQ(static_cast<int> (Ns));
    tPartBuf->setNumberQ(static_cast<int> (Nas));
    tPartBuf->setNumberQ(static_cast<int> (Nc));
    tPartBuf->setNumberQ(static_cast<int> (Nac));
    tPartBuf->setPDGCode(static_cast<int> (MC));
    aDB->AddParticleType(tPartBuf);
    delete iss;
  }
}

void Parser::ReadSHAREDecays(ParticleDB* aDB)
{
  istringstream* iss;
  char   buff[200];
  char   tFather[20], tDaughter1[20], tDaughter2[20], tDaughter3[20];
  double tBRatio, tRatio;
  int    CGcoeff; // complete branching ratio by Clebsch-Gordan coefficient: 0-no 1-yes

  while (!mFile.eof()) {
    mFile.getline(buff,200);
    if (!(*buff) || (*buff == '#'))
      continue;
    iss = new istringstream(buff);
    *iss >> tFather >> tDaughter1 >> tDaughter2 >> tDaughter3;
    if (!aDB->containsTypeNamed(tFather)) {
      PRINT_MESSAGE("<Parser::ReadSHAREDecay>\tDid not find the father particle: " << tFather);
      PRINT_MESSAGE("\tNot adding channel");
      delete iss;
      continue;
    }
    if (!aDB->containsTypeNamed(tDaughter1)) {
      PRINT_MESSAGE("<Parser::ReadSHAREDecay>\tDid not find the daughter 1 particle: " << tDaughter1);
      PRINT_MESSAGE("\tNot adding channel");
      delete iss;
      continue;
    }
    if (!aDB->containsTypeNamed(tDaughter2)) {
      PRINT_MESSAGE("<Parser::ReadSHAREDecay>\tDid not find the daughter 2 particle: " << tDaughter2);
      PRINT_MESSAGE("\tNot adding channel");
      delete iss;
      continue;
    }
    if ((*tDaughter3 > 65) && (*tDaughter3 < 122) && (!aDB->containsTypeNamed(tDaughter3))) {
      PRINT_MESSAGE("<Parser::ReadSHAREDecay>\tDid not find the daughter 3 particle: " << tDaughter3);
      PRINT_MESSAGE("\tNot adding channel");
      delete iss;
      continue;
    }
    PRINT_DEBUG_2("\tDecay channel for "<<tFather);
    if ( (*tDaughter3 > 65) && (*tDaughter3 < 122) ) {
// check if first char is a letter - if yes then 3-body decay
      *iss >> tBRatio >> CGcoeff;
      PRINT_DEBUG_2("\t\tBR ("<< tBRatio <<")");
      if (aDB->getParticleType(tDaughter1)->getampss() + aDB->getParticleType(tDaughter2)->getampss() + aDB->getParticleType(tDaughter3)->getampss() < aDB->getParticleType(tFather)->getampss()) {
	DecayChannel* newChannel = new DecayChannel(tBRatio, aDB->getParticleTypeIndex(tDaughter1), aDB->getParticleTypeIndex(tDaughter2), aDB->getParticleTypeIndex(tDaughter3));
	aDB->getParticleType(tFather)->AddDecayChannel(*newChannel);
	aDB->getParticleType(tFather)->setDecayChannelCount3( aDB->getParticleType(tFather)->getDecayChannelCount3() + 1 );
	PRINT_DEBUG_2("\t\tAdding 3-body decay channel:     "<< tDaughter1 <<" + "<< tDaughter2 <<" + "<< tDaughter3);
      } else
        PRINT_DEBUG_2("\t\tNOT adding 3-body decay channel: "<< tDaughter1 <<" + "<< tDaughter2 <<" + "<< tDaughter3 <<", extra ampss = "<< aDB->getParticleType(tDaughter1)->getampss() + aDB->getParticleType(tDaughter2)->getampss() + aDB->getParticleType(tDaughter3)->getampss() - aDB->getParticleType(tFather)->getampss());
    } else {
// 2-body decay
      tBRatio=atof(tDaughter3);
      *iss >> CGcoeff;
      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      if (CGcoeff) {// complete branching ratio by Clebsch-Gordan coefficient
        double j1, m1, j2, m2, J, M, CB;
        J  = aDB->getParticleType(tFather)->getIsospin();
        M  = aDB->getParticleType(tFather)->getIsospin3();
        j1 = aDB->getParticleType(tDaughter1)->getIsospin();
        m1 = aDB->getParticleType(tDaughter1)->getIsospin3();
        j2 = aDB->getParticleType(tDaughter2)->getIsospin();
        m2 = aDB->getParticleType(tDaughter2)->getIsospin3();
        PRINT_DEBUG_3("\t\tCG coeff for: J("<<J<<") M("<<M<<") j1("<<j1<<") m1("<<m1<<") j2("<<j2<<") m2("<<m2<<")");
        CB = SHAREClebschGordan(J, M, j1, m1, j2, m2);
        tRatio = CB*CB * tBRatio;
        // Multiply the Clebsh by two?
        // The same spin, ampss, strangeness (charm?)
        // and different I3?
        if(  (TMath::Abs(aDB->getParticleType(tDaughter1)->getSpin() - aDB->getParticleType(tDaughter2)->getSpin()) < 0.01) 
	  && (TMath::Abs(aDB->getParticleType(tDaughter1)->getampss() - aDB->getParticleType(tDaughter2)->getampss()) < 0.01)
	  && (TMath::Abs(aDB->getParticleType(tDaughter1)->getIsospin3()   - aDB->getParticleType(tDaughter2)->getIsospin3())   > 0.01)
	  && (aDB->getParticleType(tDaughter1)->getStrangeNumber()        - aDB->getParticleType(tDaughter2)->getStrangeNumber() == 0)
	  && (aDB->getParticleType(tDaughter1)->getCharmNumber()          - aDB->getParticleType(tDaughter2)->getCharmNumber() == 0)
	  ) {
          PRINT_DEBUG_3("\t\tMultiplying CG coeff by two");
          tRatio *= 2.0;
        }
        PRINT_DEBUG_2("\t\tCG("<< CB <<") BR("<< tBRatio <<") -> BR("<< tRatio<<")");
      } else {
        tRatio=tBRatio;
        PRINT_DEBUG_2("\t\tBR("<< tBRatio <<")");
      }
      if (aDB->getParticleType(tDaughter1)->getampss() + aDB->getParticleType(tDaughter2)->getampss() < aDB->getParticleType(tFather)->getampss()) {
	DecayChannel *newChannel = new DecayChannel(tRatio, aDB->getParticleTypeIndex(tDaughter1), aDB->getParticleTypeIndex(tDaughter2), -1);
        aDB->getParticleType(tFather)->AddDecayChannel(*newChannel);
        aDB->getParticleType(tFather)->setDecayChannelCount2( aDB->getParticleType(tFather)->getDecayChannelCount2() + 1 );
        PRINT_DEBUG_2("\t\tAdding 2-body decay channel:     "<< tDaughter1 <<" + "<< tDaughter2);
      } else
        PRINT_DEBUG_2("\t\tNOT adding 2-body decay channel: "<< tDaughter1 <<" + "<< tDaughter2 <<", extra ampss = "<< aDB->getParticleType(tDaughter1)->getampss() + aDB->getParticleType(tDaughter2)->getampss() - aDB->getParticleType(tFather)->getampss());
    }
    delete iss;
  }
}

double Parser::SHAREClebschGordan(double aJot,  double aEm,
				  double aJot1, double aEm1,
				  double aJot2, double aEm2)
{
  int mint, ampxt;
  double cgc = 0.0;
  int tIter;
  double coef;

  ampxt = lrint(aJot1 + aJot2 - aJot);
  mint = 0;
  if (lrint(aJot1 - aEm1)	< ampxt) ampxt = lrint(aJot1 - aEm1);
  if (lrint(aJot2 + aEm2)	< ampxt) ampxt = lrint(aJot2 + aEm2);
  if (lrint(-(aJot-aJot2+aEm1))	> mint) mint = lrint(-(aJot-aJot2+aEm1));
  if (lrint(-(aJot-aJot1-aEm2))	> mint) mint = lrint(-(aJot-aJot1-aEm2));
  
  PRINT_DEBUG_3("\t\t\tmint " << mint << " j1 " <<  aJot1 << " m1 " << aEm1);
  PRINT_DEBUG_3("\t\t\tampxt " << ampxt << " j2 " <<  aJot2 << " m2 " << aEm2);

  for (tIter = mint; tIter<=ampxt; tIter ++) {
    coef = TMath::Power(-1, tIter);
    PRINT_DEBUG_3("\t\t\tcoef1 " << coef);
    coef *= sqrt( (2*aJot+1)
            * factorials[lrint(aJot1+aEm1)]
	    * factorials[lrint(aJot1-aEm1)]
	    * factorials[lrint(aJot2+aEm2)]
	    * factorials[lrint(aJot2-aEm2)]
	    * factorials[lrint(aJot+aEm)]
	    * factorials[lrint(aJot-aEm)]
	    );
    PRINT_DEBUG_3("\t\t\tcoef2 " << coef);
    coef /= factorials[tIter]
            * factorials[lrint(aJot1+aJot2-aJot-tIter)] 
	    * factorials[lrint(aJot1-aEm1-tIter)] 
	    * factorials[lrint(aJot2+aEm2-tIter)] 
	    * factorials[lrint(aJot-aJot2+aEm1+tIter)] 
	    * factorials[lrint(aJot-aJot1-aEm2+tIter)];
    PRINT_DEBUG_3("\t\t\tcoef3 " << coef);
    cgc += coef;
  }
  cgc *= SHAREDeltaJ(aJot1, aJot2, aJot);

  return cgc;
}

double Parser::SHAREDeltaJ(double aJot1, double aJot2, double aJot)
{
  double res = sqrt( 1.0
               * factorials[lrint( aJot1 + aJot2 - aJot)]
               * factorials[lrint( aJot1 - aJot2 + aJot)]
               * factorials[lrint(-aJot1 + aJot2 + aJot)]
               / factorials[lrint( aJot1 + aJot2 + aJot + 1)]
	       );
  return res;
}

void Parser::ReadXML(Hypersurface_Library* aXML)
{
  long int tPos;
  XMLtag* tTag;
  TString buff;
  char	  chr;
  int	  tFlag;
  
  aXML->mXMLFileName = mFileName;
  PRINT_DEBUG_2("<Parser::ReadXML>");
  mFile.seekg(0, std::ios::beg);
  while(!mFile.eof())
    {
    buff = "";
    tFlag = 0;
// find a TAG
    do {
      mFile.get(chr);
      if(mFile.eof())
        {
	tFlag = -1;
	break;
      }
    } while (chr != '<');
// no tag
    if(tFlag==-1)
      break;
// copy a TAG
//    mFile.unget();
    do {
      buff += chr;
      if(chr=='[') { // DTD list
	do {
          mFile.get(chr);
	  buff += chr;
	} while (chr!=']'); 
      }
      if(buff.BeginsWith("<!--")) { // comment
	do {
          mFile.get(chr);
	  buff += chr;
	} while ( !( buff.EndsWith("--") && (mFile.peek()=='>') ) ); 
      }
      mFile.get(chr);
    } while ( (chr!='>') );
    buff += chr;
    buff.ReplaceAll("\n"," ");
    tPos = mFile.tellg();
// analyze TAG
    if(buff.BeginsWith("<?") && buff.EndsWith("?>")) {
      PRINT_DEBUG_2("XML declaration : "<<buff<<"  skipping");
    } else if(buff.BeginsWith("<!--") && buff.EndsWith("-->")) {
      PRINT_DEBUG_2("XML commentary  : "<<buff<<"  skipping");
    } else if(buff.BeginsWith("<!")) {
      PRINT_DEBUG_2("XML DTD         : "<<buff<<"  skipping");
    } else if(buff.BeginsWith("</")) {
// CLOSE TAG
      buff.ReplaceAll("</","");
      buff.ReplaceAll(">","");
      if(aXML->mCurrTag->end !=-1)
	aXML->mCurrTag = aXML->mCurrTag->father;
      // TAG content ends in file
      aXML->mCurrTag->end = tPos - (buff.Length() + 3);
      // Check if TAG name amptches
      if(buff.CompareTo(aXML->mCurrTag->name))
	PRINT_MESSAGE("<Parser::ReadXML>\tClosing tag name misamptch \""<< aXML->mCurrTag->name<<"\" != \""<<buff<<"\"");
      PRINT_DEBUG_2("XML end tag     : </"<<aXML->mCurrTag->name<<"> @ "<<aXML->mCurrTag->end);
      // TAG closed - go to parent
    } else if(buff.EndsWith("/>")) {
// NEW EMPTY TAG
      buff.ReplaceAll("<","");
      buff.ReplaceAll("/>","");
      tTag = XMLTagNew(buff);
      tTag->begin = tPos;
      tTag->end   = tTag->begin;
      // TAG structure BEGIN & END
      if(aXML->mCurrTag->end == -1) { // end TAG not found - go level down
	aXML->mCurrTag->child	= tTag;
	tTag->father		= aXML->mCurrTag;
      } else { // add new sibling
	aXML->mCurrTag->next	= tTag;
	tTag->father		= aXML->mCurrTag->father;
	tTag->prev		= aXML->mCurrTag;
      }
      // TAG closed - go to parent
      aXML->mCurrTag = tTag;
      PRINT_DEBUG_2("XML empty tag   : <"<<aXML->mCurrTag->name<<" /> @ "<<aXML->mCurrTag->begin);
      for(std::list<XMLattribute>::iterator Iter = aXML->mCurrTag->attributes.begin(); Iter != aXML->mCurrTag->attributes.end(); Iter++)
        PRINT_DEBUG_3("  attribute     :  "<<Iter->name<<"=\""<<Iter->value<<"\"");
    } else {
// NEW TAG
      buff.ReplaceAll("<","");
      buff.ReplaceAll(">","");
      tTag = XMLTagNew(buff);
      // TAG content begins in file 
      tTag->begin = tPos;
      // TAG structure BEGIN
      if(aXML->mTags == NULL) { // create ampin tag
	aXML->mTags = tTag;
      } else {
	if(aXML->mCurrTag->end == -1) { // end TAG not found - go level down
	  aXML->mCurrTag->child	= tTag;
	  tTag->father		= aXML->mCurrTag;
	} else { // add new sibling
	  aXML->mCurrTag->next  = tTag;
	  tTag->father		= aXML->mCurrTag->father;
	  tTag->prev		= aXML->mCurrTag;
	}
      }
      aXML->mCurrTag = tTag;
      PRINT_DEBUG_2("XML begin tag   : <"<<aXML->mCurrTag->name<<"> @ "<<aXML->mCurrTag->begin);
      for(std::list<XMLattribute>::iterator Iter = aXML->mCurrTag->attributes.begin(); Iter != aXML->mCurrTag->attributes.end(); Iter++)
        PRINT_DEBUG_3("  attribute     :  "<<Iter->name<<"=\""<<Iter->value<<"\"");
    }
  }
}

XMLtag* Parser::XMLTagNew(TString& aBuff)
{
  XMLtag*	tTag;
  XMLattribute	tAtt;
  int		i;
  
  tTag = new XMLtag;
  tTag->name = "";
  tTag->attributes.clear();
  tTag->begin  = -1;
  tTag->end    = -1;
  tTag->father = NULL;
  tTag->child  = NULL;
  tTag->prev   = NULL;
  tTag->next   = NULL;

// TAG name
  for(i=0; i<aBuff.Length(); i++)
    {
    if(aBuff[i] == ' ')
      break;
    tTag->name += aBuff[i];
  }
// TAG attributes
  while(i<aBuff.Length())
    {
    if((aBuff[i] != ' ') && (aBuff[i] != '\t') && (aBuff[i] != '/')) {
      tAtt.name = "";
      tAtt.value = "";
      while((aBuff[i] != '=')) {
	tAtt.name += aBuff[i];
	i++;
      }
      i += 2;
      while((aBuff[i] != '\"') && (aBuff[i] != '\'')) {
	tAtt.value += aBuff[i];
	i++;
      }
      tTag->attributes.push_back(tAtt);
    }
    i++;
  }
  return tTag;
}
