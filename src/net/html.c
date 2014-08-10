
#include "html.h"
/*
#include <htmlcxx/html/ParserDom.h>

#include <string>
#include <iostream>
#include <map>
using namespace std;
using namespace htmlcxx;
*/
void html_extract_urls(byte_t *data, uint_t dlen)
{
/*
   string html((char*)data, dlen);
   HTML::ParserDom parser;
   tree<HTML::Node> dom = parser.parseTree(html);

   //Print whole DOM tree
   //cout << dom << endl;

   //Dump all links in the tree
   tree<HTML::Node>::iterator it = dom.begin();
   tree<HTML::Node>::iterator end = dom.end();
   for (; it != end; ++it) {
      if (it->tagName() == "A"){
         it->parseAttributes();
  		 map<string, string> attr = it->attributes();
  		 cout << "********************" << endl;
  		 cout << attr["href"] << endl;
  		 cout << "********************" << endl;
      }
   }
*/
}
