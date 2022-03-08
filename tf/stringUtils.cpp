//
//
//

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include	<algorithm>
#include	<cctype>				// for tolower/toupper
#include	"stringUtils.h"

using namespace std;

string	stringTrim ( const string& str )
{
	int	len = str.length ();
	int	i, j;

	for ( i = 0; i < len && isspace ( str [i] ); i++ )
		;

	for ( j = len - 1; j >= 0 && isspace ( str [j] ); j-- )
		;

	return str.substr ( i, j - i + 1 );
}

string	replaceTabs ( const string& str )
{
	int		len = str.length ();
	string	s   = str;

	for ( int i = 0; i < len; i++ )
		if ( s [i] == '\t' )
			s [i] = ' ';

	return s;
}

string	stringLower ( const string& s )
{
	string	str ( s );
	
	transform ( str.begin (), str.end (), str.begin (), ::tolower );
	
	return str;
}

string	stringUpper ( const string& s )
{
	string	str ( s );
	
	transform ( str.begin (), str.end (), str.begin (), ::toupper );
	
	return str;
}

string	stringDequote ( const string& str )
{
	if ( str [0] == '\"' || str [0] == '\'' )			// remove quotes
		return str.substr ( 1, str.length () - 2 );

	return str;
}

void	stringParse ( const string& str, string& cmd, string& args )
{
	int	len = str.length ();
	int	pos;

	for ( pos = 0; pos < len && str [pos] != ' ' && str [pos] != '\t'; pos++ )
		;

	cmd  = str.substr ( 0, pos );
	args = replaceTabs ( stringTrim ( str.substr ( pos ) ) );
}

                            // simple check whether the file exists
bool    fileExist ( const string& fileName )
{
#ifdef  _WIN32
    struct _stat    buf;

    return ::_stat ( fileName.c_str (), &buf ) != -1;
#else
    struct stat buf;

    return stat ( fileName.c_str (), &buf ) != -1;
#endif
}
							// build filename from path and a name
string		buildFileName ( const string& path, const string& name )
{
#ifdef	_WIN32
	char	pathSeparator = '\\';
#else
	char	pathSeparator = '/';
#endif

	if ( path.empty () )
		return name;

	int	pos = path.length () - 1;

	string	res ( path );

	if ( path [pos] == '\\' || path [pos] == '/' || path [pos] == ':' )
		res += name;
	else
	{
		res += pathSeparator;
		res += name;
	}

	for ( int i = 0; i < (int)res.length (); i++ )
		if ( res [i] == '/' )
			res [i] = pathSeparator;

	return res;

}
								// extract path from a filename
string	getPath ( const string& fullName )
{
	for ( int i = fullName.length () - 1; i >= 0; i-- )
	{
		char	ch = fullName [i];

		if ( ch == '\\' || ch == '/' || ch == ':' )
			return fullName.substr ( 0, i + 1 );
	}

	return "";
}
							// extract filename and extension
string	getFileName ( const string& fullName )
{
	int	len = fullName.length ();

	for ( int i = len - 1; i >= 0; i-- )
	{
		char	ch = fullName [i];

		if ( ch == '\\' || ch == '/' || ch == ':' )
			return fullName.substr ( i + 1 );
	}

	return fullName;
}
							// get only name without extension
string	getName ( const string& fullName )
{
	string	name = getFileName ( fullName );
	size_t	pos  = name.find ( '.' );

	if ( pos == string :: npos )
		return name;

	return name.substr ( 0, pos );
}

							// get only extension
string	getExtension ( const string& fullName )
{
	string	name = getFileName ( fullName );
	size_t	pos  = name.find ( '.' );

	if ( pos == string :: npos )
		return "";

	return name.substr ( pos + 1 );
}

//////////////////// class Tokenizer ////////////////////////

Tokenizer :: Tokenizer ( const string& str, const string& seps )  : buffer ( str ), separators ( seps )
{
	pos = buffer.begin ();
}

void Tokenizer :: setString ( const string& str, const string& seps )
{
	buffer     = str;
	separators = seps;
	pos        = buffer.begin ();
}

string	Tokenizer :: next () 
{
    if ( buffer.length () < 1 )				// skip if buffer is empty
		return "";           		

    while ( pos != buffer.end () && separators.find ( *pos ) != string::npos )
        ++pos;

	string	token;
	
    while ( pos != buffer.end () && separators.find ( *pos ) == string::npos )
    {
        token += *pos;
        ++pos;
    }
	
    return token;
}
