
#pragma	once

//----------------------------------------------------------------------------------------
//-- LoadLibrary API �̃��b�p�[�N���X ----------------------------------------------------
//----------------------------------------------------------------------------------------

class	YCLibrary
{
public:

											YCLibrary();
	virtual									~YCLibrary();

	BOOL									Load( LPCTSTR pszPathToFile );
	void									Free();

	FARPROC									GetProcAddress( LPCTSTR pszProcName );


protected:

	HMODULE									m_hModule;
};
