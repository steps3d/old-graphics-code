//
// Simple ray class
//

ray :: ray ( const vec3& theOrigin, const vec3& theDir, bool normalizeDir )
{
	org = theOrigin;
	dir = theDir;
	
	if ( normalizeDir )
		dir = normalize ( dir );
}

