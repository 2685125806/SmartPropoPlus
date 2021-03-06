/* Global memory source code  file */
#include "stdafx.h"
#include <assert.h> 
#include "GlobalMemory.h"
#include "SmartPropoPlus.h"

HANDLE	ghDataLock;
struct SharedDataBlock * gpSharedBlock;



void SetNumberOfFiltersToGlobalMemory(const int n)
{
	if (!isGlobalMemoryExist())
		return;

	/* Open Mutex and Wait for it */
	ghDataLock = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_LABEL);
	if (!ghDataLock)
		return;

	WaitForSingleObject(ghDataLock, INFINITE);
	
	/* Lock access to global memory */
	// --> ghDataLock = CreateMutex(NULL, TRUE, MUTEX_LABEL);

	gpSharedBlock->n_fltr = n;

	/* Release acces lock */
	ReleaseMutex(ghDataLock);
	CloseHandle(ghDataLock);

	return;
}

int GetNumberOfFiltersFromGlobalMemory()
{
	int n;

	if (!isGlobalMemoryExist())
		return 0;

	/* Open Mutex and Wait for it */
	ghDataLock = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_LABEL);
	if (!ghDataLock)
		return 0;

	WaitForSingleObject(ghDataLock, INFINITE);
	
	/* Lock access to global memory */
	// --> ghDataLock = CreateMutex(NULL, TRUE, MUTEX_LABEL);

	n = gpSharedBlock->n_fltr;

	/* Release acces lock */
	ReleaseMutex(ghDataLock);
	CloseHandle(ghDataLock);

	return n;
}


void SetSelectedFilterIndexToGlobalMemory(const int i)
{
	if (!isGlobalMemoryExist())
		return;

	/* Open Mutex and Wait for it */
	ghDataLock = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_LABEL);
	if (!ghDataLock)
		return;

	WaitForSingleObject(ghDataLock, INFINITE);
	
	/* Lock access to global memory */
	// --> ghDataLock = CreateMutex(NULL, TRUE, MUTEX_LABEL);

	gpSharedBlock->i_sel_fltr = i;

	/* Release acces lock */
	ReleaseMutex(ghDataLock); 
	CloseHandle(ghDataLock);

	return;
}


int GetSelectedFilterIndexFromGlobalMemory()
{
	int i;

	if (!isGlobalMemoryExist())
		return -1;

	/* Open Mutex and Wait for it */
	ghDataLock = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_LABEL);
	if (!ghDataLock)
		return -1;

	WaitForSingleObject(ghDataLock, INFINITE);
	
	/* Lock access to global memory */
	// --> ghDataLock = CreateMutex(NULL, TRUE, MUTEX_LABEL);

	i = gpSharedBlock->i_sel_fltr;

	/* Release acces lock */
	ReleaseMutex(ghDataLock);
	CloseHandle(ghDataLock);

	return i;
}


//void SetFilterNameByIndexToGlobalMemory(const int i, const char * name);
void SetFilterNamesToGlobalMemory(LPCTSTR * name)
{
	int index;
	size_t total_length=0;
	LPTSTR mode;
	TCHAR pCurrentFilterNameAr[MAX_MSG_SIZE];
	LPTSTR pCurrentFilterName = pCurrentFilterNameAr;


	if (!isGlobalMemoryExist())
		return;
	
	/* Open Mutex and Wait for it */
	ghDataLock = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_LABEL);
	if (!ghDataLock)
		return;

	WaitForSingleObject(ghDataLock, INFINITE);
	
	/* Lock access to global memory */
	// --> ghDataLock = CreateMutex(NULL, TRUE, MUTEX_LABEL);
	
	// Find the begining of the list (just after the list of modulation names)
	for (index = 0 ; index < gpSharedBlock->nModulations ; index++)
	{
		mode = (LPTSTR)gpSharedBlock->pInternalModName[index];
		if (mode)
			total_length+=_tcslen(mode)+1;
		else
			break;
	};
	pCurrentFilterName = (LPTSTR)((char *)gpSharedBlock + sizeof(struct SharedDataBlock) + total_length + 1);
	
	// Take the filternames one by and Place them on the global memory
	index=0;
	while (name[index])
	{
		_tcscpy_s(pCurrentFilterName, MAX_MSG_SIZE, name[index] );
		gpSharedBlock->name_fltr[index] = (TCHAR __based( gpSharedBlock ) *)pCurrentFilterName;
		pCurrentFilterName +=_tcslen(name[index])+1;
		index++;
	};
	gpSharedBlock->name_fltr[index] = NULL;
	

	ReleaseMutex(ghDataLock);
	CloseHandle(ghDataLock);

}

LPTSTR GetFilterNameByIndexFromGlobalMemory(const int i)
{
	LPTSTR out;

	if (!isGlobalMemoryExist())
		return NULL;

	if (i>=gpSharedBlock->n_fltr || i<0)
		return NULL;


	/* Open Mutex and Wait for it */
	ghDataLock = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_LABEL);
	if (!ghDataLock)
		return NULL;

	WaitForSingleObject(ghDataLock, INFINITE);
	
	/* Lock access to global memory */
	// --> ghDataLock = CreateMutex(NULL, TRUE, MUTEX_LABEL);
	
	out = _tcsdup((LPTSTR)gpSharedBlock->name_fltr[i]);

	/* Release acces lock */
	ReleaseMutex(ghDataLock);
	CloseHandle(ghDataLock);

	return out;
}

int GetFilterIndexByNameFromGlobalMemory(LPCTSTR name)
{
	int i=0, out=-1;
	LPTSTR current;

	if (!isGlobalMemoryExist())
		return -1;

	if (i>=gpSharedBlock->n_fltr || i<0)
		return -1;
	/* Open Mutex and Wait for it */
	ghDataLock = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_LABEL);
	if (!ghDataLock)
		return -1;

	WaitForSingleObject(ghDataLock, INFINITE);
	
	/* Lock access to global memory */
	// --> ghDataLock = CreateMutex(NULL, TRUE, MUTEX_LABEL);
	
	/* Go over all entries and compair to input string */
	for (i=0; i<MAX_FLTRS; i++)
	{
		current = (LPTSTR)gpSharedBlock->name_fltr[i];
		if (!_tcscmp(current, name))
		{
			out = i;
			break;
		};
	};

	/* Release acces lock */
	ReleaseMutex(ghDataLock);
	CloseHandle(ghDataLock);
	return out;
}

int SetActiveModeToGlobalMemory(LPCTSTR selected)
{
	LPCTSTR DefMods[] = MOD_DEF_STR;
	int nMod=0;
	int iMod;

	if (!isGlobalMemoryExist())
		return 0;

	/* Open Mutex and Wait for it */
	ghDataLock = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_LABEL);
	if (!ghDataLock)
		return 0;

	WaitForSingleObject(ghDataLock, INFINITE);
	
	/* Lock access to global memory */
	// --> ghDataLock = CreateMutex(NULL, TRUE, MUTEX_LABEL);

	/* Convert Display Name to Internal Name */
	while (DefMods[nMod*2])
	{
		if (_tcscmp(DefMods[nMod*2], selected))
		{
			nMod++;
			continue;
		};
		break;
	};

	iMod  = GetModulationIndexFromGlobalMemory(DefMods[nMod*2]);

	if (iMod >= 0)
		gpSharedBlock->ActiveModulation.iModType = iMod;
	else
		iMod = nMod;

	ReleaseMutex(ghDataLock);
	CloseHandle(ghDataLock);

	return iMod;
}


int SetShiftAutoDetectToGlobalMemory(const int sel)
{
	if (!isGlobalMemoryExist())
		return 0;

	/* Open Mutex and Wait for it */
	ghDataLock = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_LABEL);
	if (!ghDataLock)
		return 0;

	WaitForSingleObject(ghDataLock, INFINITE);
	
	/* Lock access to global memory */
	// --> ghDataLock = CreateMutex(NULL, TRUE, MUTEX_LABEL);

	gpSharedBlock->ActiveModulation.AutoDetectModShift = sel;

	/* Release acces lock */
	ReleaseMutex(ghDataLock);
	CloseHandle(ghDataLock);

	return sel;

}



int isGlobalMemoryExist()
{
	if (!gpSharedBlock)
		OpenSharedDataStruct();

	if (gpSharedBlock)
		return 1;
	else
		return 0;
}


int GetModulationIndexFromGlobalMemory(LPCTSTR InternalModName)
{
	int index;
	HANDLE hLock;

	if (!isGlobalMemoryExist() || !InternalModName)
		return 0;
	
	/* Open Mutex and Wait for it */
	hLock = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_LABEL);
	if (!hLock)
		return 0;

	WaitForSingleObject(hLock, INFINITE);
	
	/* Lock access to global memory */
	// --> ghDataLock = CreateMutex(NULL, TRUE, MUTEX_LABEL);

	for (index = 0 ; index < gpSharedBlock->nModulations ; index++)
	{
		if (_tcscmp((LPTSTR)gpSharedBlock->pInternalModName[index], InternalModName))
			continue;
		ReleaseMutex(hLock);
		CloseHandle(hLock);
		return index;
	}

	ReleaseMutex(hLock);
	CloseHandle(hLock);

	return -1;
}

far void * OpenSharedDataStruct(void)
{
    HANDLE hFileMapping;
	LPCTSTR DefMods[] = MOD_DEF_STR;

	gpSharedBlock = NULL;
	
	/* Wait for Mutex */
	ghDataLock = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_LABEL);
	if (!ghDataLock)
		return NULL;

	WaitForSingleObject(ghDataLock, INFINITE);

	// --> ghDataLock = CreateMutex(NULL, TRUE, MUTEX_LABEL);
	
    hFileMapping = OpenFileMapping
		(
		FILE_MAP_ALL_ACCESS, 
		FALSE,
		BLOCK_LABEL); // Name
	
	/* Version 3.3.2 */
	if (!hFileMapping)
	{ 		
		ReleaseMutex(ghDataLock);
		CloseHandle(ghDataLock);

		return NULL;
	};
	
    gpSharedBlock = (struct SharedDataBlock *)MapViewOfFile
		(
		hFileMapping,        // File mapping object
		FILE_MAP_ALL_ACCESS, // Read/Write
		0,                  // Offset - high 32 bits
		0,                  // Offset - low 32 bits
		0);                 // Map the whole thing
	

    ReleaseMutex(ghDataLock);

	if (!gpSharedBlock)
	{
		/* Version 3.3.2 */
		SppMessageBoxWithErrorCode();
		return NULL;
	};

	return (far void *)gpSharedBlock;
}

void SetPositiveShiftToGlobalMemory(const int sel)
{
	if (!isGlobalMemoryExist())
		return;

	/* Open Mutex and Wait for it */
	ghDataLock = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_LABEL);
	if (!ghDataLock)
		return;

	WaitForSingleObject(ghDataLock, INFINITE);
	
	/* Lock access to global memory */
	// --> ghDataLock = CreateMutex(NULL, TRUE, MUTEX_LABEL);

	gpSharedBlock->ActiveModulation.ActiveModShift = sel;

	/* Release acces lock */
	ReleaseMutex(ghDataLock);
	CloseHandle(ghDataLock);

}

struct Modulations * GetModulationFromGlobalMemory()
{
	struct Modulation ** Mod;
	struct Modulations * out;
	unsigned int VersionDll;
	int index;
//	char tmp[MAX_VAL_NAME];

	if (!isGlobalMemoryExist())
		return NULL;

	/* Open Mutex and Wait for it */
	ghDataLock = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_LABEL);
	if (!ghDataLock)
		return NULL;

	WaitForSingleObject(ghDataLock, INFINITE);
	
	/* Lock access to global memory */
	// --> ghDataLock = CreateMutex(NULL, TRUE, MUTEX_LABEL);

	VersionDll = gpSharedBlock->VersionDll;

	Mod = (struct Modulation **)calloc(gpSharedBlock->nModulations+1, sizeof(struct Modulation *));
	if (!Mod)
		return NULL;

	index = 0;
	
	do 
	{
		Mod[index] = (struct Modulation *)malloc(sizeof(struct Modulation));
		if (!Mod[index])
			break;
		Mod[index]->index = index;
		Mod[index]->ModTypeInternal =  _tcsdup((LPTSTR)gpSharedBlock->pInternalModName[index]);
		Mod[index]->ModTypeDisplay = NULL;
		index++;
	}while ((char *)gpSharedBlock->pInternalModName[index]);

	out = (struct Modulations *)calloc(1, sizeof(struct Modulations));
	if (!out)
		return NULL;

	out->ModulationList = Mod;
	out->Active = gpSharedBlock->ActiveModulation.iModType;
	out->PositiveShift = gpSharedBlock->ActiveModulation.ActiveModShift;
	out->ShiftAutoDetect = gpSharedBlock->ActiveModulation.AutoDetectModShift;
	
	ReleaseMutex(ghDataLock);
	CloseHandle(ghDataLock);

	return out;
}


LPWSTR GetMixerNameFromGlobalMemory(void)
{	
	LPWSTR MixerName;

	if (!isGlobalMemoryExist())
		return NULL;

	/* Open Mutex and Wait for it */
	ghDataLock = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_LABEL);
	if (!ghDataLock)
		return NULL;

	WaitForSingleObject(ghDataLock, INFINITE);

	/* Lock access to global memory */
	//// --> ghDataLock = CreateMutex(NULL, TRUE, MUTEX_LABEL);

	MixerName = _tcsdup(gpSharedBlock->SrcName);

	/* Release acces lock */
	ReleaseMutex(ghDataLock);
	CloseHandle(ghDataLock);

	return MixerName;

}

void SwitchMixerRequestViaGlobalMemory(LPCWSTR MixerName)
{
	if (!isGlobalMemoryExist())
		return;

	if (!MixerName)
		return;

	/* Open Mutex and Wait for it */
	ghDataLock = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_LABEL);
	if (!ghDataLock)
		return ;
	WaitForSingleObject(ghDataLock, INFINITE);
	
	/* Lock access to global memory */
	//// --> ghDataLock = CreateMutex(NULL, TRUE, MUTEX_LABEL);

	_tcsncpy_s(gpSharedBlock->SrcName, MAX_MODS, MixerName, MAX_MODS);
	gpSharedBlock->MixerDeviceStatus = SharedDataBlock::MDSTAT::CHANGE_REQ;

	/* Release acces lock */
	ReleaseMutex(ghDataLock);
	CloseHandle(ghDataLock);

}

void SwitchMixerAckViaGlobalMemory(LPWSTR MixerName)
{
	if (!isGlobalMemoryExist())
		return;

	/* Open Mutex and Wait for it */
	ghDataLock = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_LABEL);
	if (!ghDataLock)
		return;
	WaitForSingleObject(ghDataLock, INFINITE);
	
	/* Lock access to global memory */
	//// --> ghDataLock = CreateMutex(NULL, TRUE, MUTEX_LABEL);

	_tcsncpy_s(gpSharedBlock->SrcName, MAX_MODS, MixerName, MAX_MODS);
	//gpSharedBlock->MixerDeviceStatus = RUNNING;

	/* Release acces lock */
	ReleaseMutex(ghDataLock);
	CloseHandle(ghDataLock);
}


void SetSwitchMixerRequestStatToGlobalMemory(enum SharedDataBlock::MDSTAT Stat)
{
	if (!isGlobalMemoryExist())
		return;

	/* Open Mutex and Wait for it */
	ghDataLock = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_LABEL);
	if (!ghDataLock)
		return;
	WaitForSingleObject(ghDataLock, INFINITE);
	
	/* Lock access to global memory */
	//// --> ghDataLock = CreateMutex(NULL, TRUE, MUTEX_LABEL);

	gpSharedBlock->MixerDeviceStatus = Stat;

	/* Release acces lock */
	ReleaseMutex(ghDataLock);
	CloseHandle(ghDataLock);
}

/*
	Create the global shared memory that is used by the DLL and the GUI
	> Open an existing Mutex or create initially owned Mutex named MUTEX_LABEL ("SPP Global Shared memory Mutex")
	> Create or get a File Mapping Object for an operating-system paging file. Object name is BLOCK_LABEL
	> If error from creation of the  File Mapping Object is ERROR_ALREADY_EXISTS then it means that it already exists
	> Map the whole of the Object into a File View that is casted into a global pointer.
	> If this is the CREATION of the Object then the global structure is populated
	> The Mutex is released just before the function returns
	Return: File View
*/
far void * CreateSharedDataStruct(struct Modulations * data)
{
    HANDLE hFileMapping;
	DWORD dwMapErr;
	int index;
	LPCTSTR DefMods[] = MOD_DEF_STR;
	int nModulations;
	LPTSTR pCurrentModName;

	gpSharedBlock = NULL;
	
	/* Wait for Mutex */
	ghDataLock = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_LABEL);
	if (ghDataLock)
		WaitForSingleObject(ghDataLock, INFINITE);
	else
		ghDataLock = CreateMutex(NULL, TRUE, MUTEX_LABEL);

	if (!ghDataLock)
		return NULL;
	
    hFileMapping = CreateFileMapping
		(
		INVALID_HANDLE_VALUE,// File handle
		NULL,                // Security attributes
		PAGE_READWRITE,      // Protection
		0,                   // Size - high 32 bits
		1<<16 ,               // Size - low 32 bits
		BLOCK_LABEL); // Name
	
    dwMapErr = GetLastError();

	/* Version 3.3.2 */
	if (!hFileMapping)
	{ 
		SppMessageBoxWithErrorCode();
		ReleaseMutex(ghDataLock);
		CloseHandle(ghDataLock);
		return NULL;
	};

	
    gpSharedBlock = (struct SharedDataBlock *)MapViewOfFile
		(
		hFileMapping,        // File mapping object
		FILE_MAP_ALL_ACCESS, // Read/Write
		0,                  // Offset - high 32 bits
		0,                  // Offset - low 32 bits
		0);                 // Map the whole thing
	
	if (!gpSharedBlock)
	{
		/* Version 3.3.2 */
		SppMessageBoxWithErrorCode();
		ReleaseMutex(ghDataLock);
		CloseHandle(ghDataLock);
		return NULL;
	};

    // Only initialize shared memory if we actually created.
    if (dwMapErr != ERROR_ALREADY_EXISTS)
	{
		/* Get all available modulation types */
		index = 0;
		if (data)
		{	/* Data from registry */

			/* Get number of modulations */
			nModulations=0;
			while (data->ModulationList[nModulations])
				nModulations++;
			gpSharedBlock->nModulations = nModulations;

			/* Get starting point for list of (internal) modulation names */
			pCurrentModName = (LPTSTR)((char *)gpSharedBlock + sizeof(struct SharedDataBlock));

			for  (index=0 ; index<=nModulations+1 ; index++) 
			{
				if (!data->ModulationList[index] || index == MAX_MODS)
				{
					gpSharedBlock->pInternalModName[index] = NULL;
					break;
				};

				_tcscpy_s(pCurrentModName, MAX_MODS, data->ModulationList[index]->ModTypeInternal);
				gpSharedBlock->pInternalModName[index] = (TCHAR __based( gpSharedBlock ) *)pCurrentModName;

				/* Mark selected Modulation type */
				if (index == data->Active)
					gpSharedBlock->ActiveModulation.iModType = index;

				pCurrentModName+=_tcslen(data->ModulationList[index]->ModTypeInternal)+1;
			};

			gpSharedBlock->ActiveModulation.ActiveModShift		= data->PositiveShift;
			gpSharedBlock->ActiveModulation.AutoDetectModShift	= data->ShiftAutoDetect;
		}
		else
		{	/* Default data */

			/* Get number of modulations */
			nModulations=0;
			while (DefMods[nModulations*2])
				nModulations++;
			gpSharedBlock->nModulations = nModulations;

			/* Get starting point for list of (internal) modulation names */
			pCurrentModName = (LPTSTR)((char *)gpSharedBlock + sizeof(struct SharedDataBlock));

			for  (index=0 ; index<=nModulations+1 ; index++) 
			{
				if (!DefMods[index*2] || index == MAX_MODS)
				{
					gpSharedBlock->pInternalModName[index] = NULL;
					break;
				};

				_tcscpy_s(pCurrentModName,  MAX_MODS, DefMods[index*2]);
				gpSharedBlock->pInternalModName[index] = (TCHAR __based( gpSharedBlock ) *)pCurrentModName;

				/* Mark selected Modulation type */
				if (!_tcscmp(gpSharedBlock->pInternalModName[index], MOD_TYPE_PPM))
					gpSharedBlock->ActiveModulation.iModType = index;

				pCurrentModName+=_tcslen( DefMods[index*2])+1;
			};			
			
			/* Get shift-related info */
			gpSharedBlock->ActiveModulation.ActiveModShift		= 1;
			gpSharedBlock->ActiveModulation.AutoDetectModShift	= 1;

		};
			
		/* Store version of THIS file */
		gpSharedBlock->VersionDll = VER_DLL;
		gpSharedBlock->GuiDialogBoxTitle[0] = '\0';

		/* Default values */
		gpSharedBlock->VersionGui = 0;
		gpSharedBlock->i_sel_fltr = -1;
		gpSharedBlock->MixerDeviceStatus = SharedDataBlock::MDSTAT::RUNNING;
		_tcscpy_s(gpSharedBlock->SrcName, 4, L"");

	};
	
	if (ghDataLock)
		ReleaseMutex(ghDataLock);

	return (far void *)gpSharedBlock;
}
