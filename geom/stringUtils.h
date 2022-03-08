//
//
//

#ifndef	__STRING_UTILS__
#define	__STRING_UTILS__

#include	<string>

using namespace std;

string	stringTrim  ( const string& s );
string	stringLower ( const string& s );
string	stringUpper ( const string& s );
string	stringDequote ( const string& str );
void	stringParse ( const string& str, string& cmd, string& args );
string	replaceTabs ( const string& str );

#endif
