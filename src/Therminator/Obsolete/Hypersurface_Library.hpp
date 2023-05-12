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

#ifndef _TH2_HYPERSURFACE_LIBRARY_H_
  #define _TH2_HYPERSURFACE_LIBRARY_H_

#include <TString.h>
#include "VectorField.hpp"
#include "StructXML.hpp"

class Hypersurface_Library
{
  public:
    Hypersurface_Library();
    ~Hypersurface_Library();
     
    Hypersurface_Library* getXMLTag(const char* aTagName, const char* aAttName, const char* aAttVal) throw(int); 
    TString   getXMLAttribute(const char* aAttName) throw(int);
    TString   getXMLContent();
    VectorField* getXMLVector3D();
    TString   getXMLTagDescription();
  private:
    friend class Parser;    
    
    int       XMLAttributeamptch(const char* aAttName, const char* aAttVal);
    
    TString   mXMLFileName;
    XMLtag*   mTags;
    XMLtag*   mCurrTag;
};

#endif

/*! @file Hypersurface_Library.h
 * @brief Definition of Hypersurface_Library class. Operation on the hypersurface library.
 */
/*! @class Hypersurface_Library
 * @brief Class to operate the hypersurface library created from XML file.
 *
 * @fn Hypersurface_Library::Hypersurface_Library()
 * @brief Default constructor.
 *
 * @fn Hypersurface_Library::~Hypersurface_Library()
 * @brief Destructor.
 *
 * @fn Hypersurface_Library* Hypersurface_Library::getXMLTag(const char* aTagName, const char* aAttName, const char* aAttVal) throw(int)
 * @brief Searches the Hypersurface_Library for an XML Tag with a given attribute name and value.
 * 
 * If successful an internal pointer is set to StructXML structure.
 * @param [in] aTagName name of the XML tag
 * @param [in] aAttName XML tag attribute name
 * @param [in] aAttVal XML tag attribute value
 * @retval Hypersurface_Library* pointer to Hypersurface_Library 
 * @exception int ERROR_LIBRARY_EMPTY defined in THGlobal.h
 * @exception int ERROR_LIBRARY_TAG_NOT_FOUND defined in THGlobal.h
 *
 * @fn TString Hypersurface_Library::getXMLAttribute(const char* aAttName) throw(int)
 * @brief Returns the attribute value for the given XML tag attribute name. 
 * @warning XML tag must be first found in the library with the getXMLTag() function.
 * @param [in] aAttName name of the XML tag attribute
 * @exception int ERROR_LIBRARY_TAG_ATTRIB_NOT_FOUND defined in THGlobal.h
 *
 * @fn TString Hypersurface_Library::getXMLContent()
 * @brief Returns the content stored between an currently pointed XML tag.
 * @warning XML tag must be first found in the library with the getXMLTag() function. 
 *
 * @fn VectorField* Hypersurface_Library::getXMLVector3D()
 * @brief Returns the VectorField object stored stored in the currently pointed XML tag.
 * @warning XML tag must be first found in the library with the getXMLTag() function.
 * 
 * @fn TString Hypersurface_Library::getXMLTagDescription()
 * @brief Returns a description of the XML tag. For debugging purpose only.
 */
