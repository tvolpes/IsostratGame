#include "base.h"
#include "config.h"
#include "def.h"

#include <boost\filesystem.hpp>
#include <boost\property_tree\xml_parser.hpp>

CConfigLoader::CConfigLoader() {
}
CConfigLoader::~CConfigLoader() {
}

bool CConfigLoader::saveConfig()
{
	boost::filesystem::path configPath;

	PrintInfo( L"Saving config file...\n" );

	m_xmlTree.put( "GameConfig.Video.WindowSize.X", m_windowX );
	m_xmlTree.put( "GameConfig.Video.WindowSize.Y", m_windowY );
	m_xmlTree.put( "GameConfig.Video.Resolution.X", m_resolutionX );
	m_xmlTree.put( "GameConfig.Video.Resolution.Y", m_resolutionY );
	m_xmlTree.put( "GameConfig.Video.IsFullscreen", m_isFullScreen );
	m_xmlTree.put( "GameConfig.Video.IsBorderless", m_isBorderless );
	m_xmlTree.put( "GameConfig.Video.FOV", m_fieldOfView );

	// Keybinds
	m_xmlTree.put( "GameConfig.Keybinds.WalkForward", m_keybinds[KEYBIND_WALK_FORWARD] );
	m_xmlTree.put( "GameConfig.Keybinds.WalkBackward", m_keybinds[KEYBIND_WALK_BACKWARD] );
	m_xmlTree.put( "GameConfig.Keybinds.StrafeLeft", m_keybinds[KEYBIND_STRAFE_LEFT] );
	m_xmlTree.put( "GameConfig.Keybinds.StrafeRight", m_keybinds[KEYBIND_STRAFE_RIGHT] );
	m_xmlTree.put( "GameConfig.Keybinds.Run", m_keybinds[KEYBIND_RUN] );
	m_xmlTree.put( "GameConfig.Keybinds.Crouch", m_keybinds[KEYBIND_WALK] );
	m_xmlTree.put( "GameConfig.Keybinds.ToggleDebugDraw", m_keybinds[KEYBIND_TOGGLE_DEBUGDRAW] );
	m_xmlTree.put( "GameConfig.Keybinds.SetFrustrum", m_keybinds[KEYBIND_SET_FRUSTRUM] );

	// Make sure the folder exists
	configPath = boost::filesystem::current_path();
	configPath /= FILESYSTEM_CONFIGDIR;
	if( !boost::filesystem::is_directory( configPath ) )
		boost::filesystem::create_directory( configPath );
	configPath /= "game.config";
	// Save the config
	try {
		boost::property_tree::write_xml( configPath.string(), m_xmlTree );
	}
	catch( boost::property_tree::xml_parser_error &e ) {
		PrintError( L"Failed to write config file (%hs)", e.what() );
		return false;
	}

	return true;
}

bool CConfigLoader::initializeAndLoad()
{
	boost::filesystem::path configPath;

	// Find the config file and load it
	configPath = boost::filesystem::current_path();
	configPath /= FILESYSTEM_CONFIGDIR;
	configPath /= "game.config";
	try {
		PrintInfo( L"Loading config file...\n" );
		boost::property_tree::read_xml( configPath.string(), m_xmlTree );
	}
	catch( boost::property_tree::xml_parser_error &e ) {
		PrintWarn( L"Failed to load game config (%hs)\n", e.what() );
	}

	m_windowX = m_xmlTree.get( "GameConfig.Video.WindowSize.X", WINDOW_DEFRES_X );
	m_windowY = m_xmlTree.get( "GameConfig.Video.WindowSize.Y", WINDOW_DEFRES_Y );
	m_resolutionX = m_xmlTree.get( "GameConfig.Video.Resolution.X", WINDOW_DEFRES_X );
	m_resolutionY = m_xmlTree.get( "GameConfig.Video.Resolution.Y", WINDOW_DEFRES_Y );
	m_isFullScreen = m_xmlTree.get( "GameConfig.Video.IsFullscreen", false );
	m_isBorderless = m_xmlTree.get( "GameConfig.Video.IsBorderless", false );
	m_fieldOfView = m_xmlTree.get( "GameConfig.Video.FOV", 45.0f );

	// Get the key binds
	m_keybinds.resize( KEYBIND_COUNT );
	m_keybinds[KEYBIND_WALK_FORWARD] = m_xmlTree.get( "GameConfig.Keybinds.WalkForward", (unsigned short)SDL_SCANCODE_W );
	m_keybinds[KEYBIND_WALK_BACKWARD] = m_xmlTree.get( "GameConfig.Keybinds.WalkBackward", (unsigned short)SDL_SCANCODE_S );
	m_keybinds[KEYBIND_STRAFE_LEFT] = m_xmlTree.get( "GameConfig.Keybinds.StrafeLeft", (unsigned short)SDL_SCANCODE_A );
	m_keybinds[KEYBIND_STRAFE_RIGHT] = m_xmlTree.get( "GameConfig.Keybinds.StrafeRight", (unsigned short)SDL_SCANCODE_D );
	m_keybinds[KEYBIND_RUN] = m_xmlTree.get( "GameConfig.Keybinds.Run", (unsigned short)SDL_SCANCODE_LSHIFT );
	m_keybinds[KEYBIND_WALK] = m_xmlTree.get( "GameConfig.Keybinds.Crouch", (unsigned short)SDL_SCANCODE_LCTRL );
	m_keybinds[KEYBIND_TOGGLE_DEBUGDRAW] = m_xmlTree.get( "GameConfig.Keybinds.ToggleDebugDraw", (unsigned short)SDL_SCANCODE_F2 );
	m_keybinds[KEYBIND_SET_FRUSTRUM] = m_xmlTree.get( "GameConfig.Keybinds.SetFrustrum", (unsigned short)SDL_SCANCODE_F3 );

	return true;
}
void CConfigLoader::destroy() {
	// Save the config file
	this->saveConfig();
}

// WINDOW X
int CConfigLoader::getWindowX() {
	return m_windowX;
}
void CConfigLoader::setWindowX( int X ) {
	m_windowX = X;
}
// WINDOW Y
int CConfigLoader::getWindowY() {
	return m_windowY;
}
void CConfigLoader::setWindowY( int Y ) {
	m_windowY = Y;
}
// RESOLUTION X
int CConfigLoader::getResolutionX() {
	return m_resolutionX;
}
void CConfigLoader::setResolutionX( int X ) {
	m_resolutionX = X;
}
// RESOLUTION Y
int CConfigLoader::getResolutionY() {
	return m_resolutionY;
}
void CConfigLoader::setResolutionY( int Y ) {
	m_resolutionY = Y;
}
// FULLSCREEN
bool CConfigLoader::isFullscreen() {
	return m_isFullScreen;
}
void CConfigLoader::setFullscreen( bool fullscreen ) {
	m_isFullScreen = fullscreen;
}
// BORDERLESS
bool CConfigLoader::isBorderless() {
	return m_isBorderless;
}
void CConfigLoader::setBorderless( bool borderless ) {
	m_isBorderless = borderless;
}
// FIELD OF VIEW
float CConfigLoader::getFieldOfView() {
	return m_fieldOfView;
}
void CConfigLoader::setFieldOfView( float fov ) {
	m_fieldOfView = fov;
}
// Key binds
unsigned short CConfigLoader::getKeybind( unsigned short keybind ) {
	if( m_keybinds[keybind] >= SDL_NUM_SCANCODES )
		return 0;
	return m_keybinds[keybind];
}
void CConfigLoader::setKeybind( unsigned short keybind, unsigned short key ) {
	m_keybinds[keybind] = key;
}