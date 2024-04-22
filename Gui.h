#pragma once

namespace Gui
{
	void	Init( HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* device_context );
	void	Customize( void );
	void	Render( void );
	void	Unload( void );

	inline  ImVec2  Size = { 533 , 691 };

	inline  ImFont* LogoFont;
	inline  ImFont* IconFont;

	inline  int		PageID = 1, MenuID = 0;

	inline  stImNotify ImNotify;
}