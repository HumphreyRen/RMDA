#pragma once
#import "c:\windows\system32\msxml3.dll"
using namespace MSXML2;

#define SAFERELEASEXML(p) if (p) {(p).Release(); p = NULL;} else ;

class __declspec(dllexport) CMyXml3
{
public:	
    static bool myGetNodeTextByIndex(MSXML2::IXMLDOMNodeListPtr nodes,int n,LPSTR str,int nLen);    	
	static bool myGetChildNodesByIndex(MSXML2::IXMLDOMNodeListPtr nodes,int n, MSXML2::IXMLDOMNodeList **childList);
	static bool myGetAttrsByIndex(MSXML2::IXMLDOMNodeListPtr nodes,int n, MSXML2::IXMLDOMNamedNodeMap **attrs);	
	static bool myGetAttrByName(MSXML2::IXMLDOMNamedNodeMapPtr XMLNODEATTS,LPSTR attrName,LPSTR str,int nLen);

	static bool myCreateProcessingInstruction(MSXML2::IXMLDOMDocumentPtr XMLDOC);
	static bool myCreateElement(MSXML2::IXMLDOMDocumentPtr XMLDOC,MSXML2::IXMLDOMElementPtr XMLNODEr,LPSTR eleName,LPSTR eleText);
	static bool mySetNodeTextByIndex(MSXML2::IXMLDOMNodeListPtr nodes,int n,LPSTR str);
	static bool mySetAttrByName(MSXML2::IXMLDOMNamedNodeMapPtr XMLNODEATTS,LPSTR attrName,LPSTR str);
};