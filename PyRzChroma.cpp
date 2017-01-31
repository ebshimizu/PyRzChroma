#include "PyRzChroma.h"

#ifdef _WIN64
#define CHROMASDKDLL        _T("RzChromaSDK64.dll")
#else
#define CHROMASDKDLL        _T("RzChromaSDK.dll")
#endif

typedef RZRESULT(*INIT)(void);
typedef RZRESULT(*UNINIT)(void);
typedef RZRESULT(*CREATEEFFECT)(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEKEYBOARDEFFECT)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEHEADSETEFFECT)(ChromaSDK::Headset::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEMOUSEPADEFFECT)(ChromaSDK::Mousepad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEMOUSEEFFECT)(ChromaSDK::Mouse::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEKEYPADEFFECT)(ChromaSDK::Keypad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*SETEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*DELETEEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*REGISTEREVENTNOTIFICATION)(HWND hWnd);
typedef RZRESULT(*UNREGISTEREVENTNOTIFICATION)(void);
typedef RZRESULT(*QUERYDEVICE)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE &DeviceInfo);

INIT Init = NULL;
UNINIT UnInit = NULL;
CREATEEFFECT CreateEffect = NULL;
CREATEKEYBOARDEFFECT CreateKeyboardEffect = NULL;
CREATEMOUSEEFFECT CreateMouseEffect = NULL;
CREATEHEADSETEFFECT CreateHeadsetEffect = NULL;
CREATEMOUSEPADEFFECT CreateMousepadEffect = NULL;
CREATEKEYPADEFFECT CreateKeypadEffect = NULL;
SETEFFECT SetEffect = NULL;
DELETEEFFECT DeleteEffect = NULL;
QUERYDEVICE QueryDevice = NULL;

RzChroma::RzChroma()
{
  m_ChromaSDKModule = NULL;
}

RzChroma::~RzChroma()
{
  UnInitialize();
}

bool RzChroma::Initialize()
{
  if (m_ChromaSDKModule == NULL)
  {
    m_ChromaSDKModule = LoadLibrary(CHROMASDKDLL);

    if (m_ChromaSDKModule == NULL)
    {
      ASSERT(GetLastError() == ERROR_SUCCESS);
      return false;
    }
  }

  if (Init == NULL)
  {
    RZRESULT Result = RZRESULT_INVALID;
    Init = (INIT)GetProcAddress(m_ChromaSDKModule, "Init");
    if (Init)
    {
      Result = Init();
      if (Result == RZRESULT_SUCCESS)
      {
        CreateEffect = (CREATEEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateEffect");
        CreateKeyboardEffect = (CREATEKEYBOARDEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateKeyboardEffect");
        CreateMouseEffect = (CREATEMOUSEEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateMouseEffect");
        CreateHeadsetEffect = (CREATEHEADSETEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateHeadsetEffect");
        CreateMousepadEffect = (CREATEMOUSEPADEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateMousepadEffect");
        CreateKeypadEffect = (CREATEKEYPADEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateKeypadEffect");
        SetEffect = (SETEFFECT)GetProcAddress(m_ChromaSDKModule, "SetEffect");
        DeleteEffect = (DELETEEFFECT)GetProcAddress(m_ChromaSDKModule, "DeleteEffect");
        QueryDevice = (QUERYDEVICE)GetProcAddress(m_ChromaSDKModule, "QueryDevice");

        if (CreateEffect &&
          CreateKeyboardEffect &&
          CreateMouseEffect &&
          CreateHeadsetEffect &&
          CreateMousepadEffect &&
          CreateKeypadEffect &&
          SetEffect &&
          DeleteEffect &&
          QueryDevice)
        {
          return true;
        }
        else
        {
          return false;
        }
      }
    }
  }

  return true;
}

bool RzChroma::UnInitialize()
{
  if (m_ChromaSDKModule != NULL)
  {
    RZRESULT Result = RZRESULT_INVALID;
    UNINIT UnInit = (UNINIT)GetProcAddress(m_ChromaSDKModule, "UnInit");
    if (UnInit)
    {
      Result = UnInit();
      ASSERT(Result == RZRESULT_SUCCESS);
    }

    FreeLibrary(m_ChromaSDKModule);
    m_ChromaSDKModule = NULL;

    return true;
  }

  return false;
}

void RzChroma::setAll(float red, float green, float blue)
{
  _staticColor = getColor(red, green, blue);
  applyKeyColors();
}

void RzChroma::setKey(RZKEY2 key, float red, float green, float blue)
{
  _setKeyColors[key] = getColor(red, green, blue);
  applyKeyColors();
}

void RzChroma::unsetKey(RZKEY2 key)
{
  _setKeyColors.erase(key);
  applyKeyColors();
}

void RzChroma::clearAllKeys()
{
  _setKeyColors.clear();
  applyKeyColors();
}

COLORREF RzChroma::getColor(float red, float green, float blue)
{
  return RGB((UINT8)(red * 255), (UINT8)(green * 255), (UINT8)(blue * 255));
}

void RzChroma::applyKeyColors()
{
  ChromaSDK::Keyboard::CUSTOM_KEY_EFFECT_TYPE keys = {};

  for (int r = 0; r < ChromaSDK::Keyboard::MAX_ROW; r++) {
    for (int c = 0; c < ChromaSDK::Keyboard::MAX_COLUMN; c++) {
      keys.Color[r][c] = _staticColor;
      keys.Key[r][c] = 0x01000000 | _staticColor;
    }
  }

  for (auto k : _setKeyColors) {
    keys.Color[HIBYTE(k.first)][LOBYTE(k.first)] = k.second;
    keys.Key[HIBYTE(k.first)][LOBYTE(k.first)] = 0x01000000 | k.second;
  }

  CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM_KEY, &keys, NULL);
}
