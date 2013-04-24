// Strings + Messages
#define	STR_EN_TTL_SPP_ERR	L"SmartPropoPlus Error"
#define	STR_EN_TTL_SPP_NONE	L"SmartPropoPlus"
#define	STR_EN_OS_TOO_OLD	L"Cannot start SmartPropoPlus\r\nRuns on Vista SP2 or higher\r\nQuitting"
#define	STR_EN_CANNOT_INIT	L"Cannot initialize SmartPropoPlus\r\nQuitting"
#define	STR_EN_DFLT_TOOLTIP	L"Running"
#define	STR_EN_RUN_TOOLTIP	L"Running"
#define	STR_EN_ERR_TOOLTIP	L"General Error"
#define	STR_EN_UNDF_TOOLTIP	L"Undefined state - Error"
#define	STR_EN_CONF_TOOLTIP	L"SmartPropoPlus need to be configured"
#define	STR_EN_ALREADY_RUN	L"Another instance of SmartPropoPlus is already running\r\nQuitting"
#define	STR_SINGLETON_MTX	L"This is a name of a mutex that serves for SPP singleton-ness"


// Structures and Enums

struct jack_info
{
	/* Inter-unit information about jack/capture endpoint information */
	int	struct_size;
	WCHAR * id;
	COLORREF color;
	WCHAR * FriendlyName;
	bool	Enabled;
};