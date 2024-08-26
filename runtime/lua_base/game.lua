local engine = require("engine")

game	= {}	-- This table holds the engine functions

-- This is a wrapper function to call the C++ engine function that
-- loads the window.
function game:Launch(w, h)
	width = 0
	height = 0 
	
	if (w == null) then
		width = 1600
	end

	if (h == null) then
		height = 900
	end

	engine:LaunchWindow(w, h, "Test")
end

function game:Fullscreen(_mode)
	mode = string.lower(_mode)

	if (mode == "windowed") then
		engine:SetFullscreen(0)
	elseif (mode == "fullscreen") then
		engine:SetFullscreen(1)
	elseif (mode == "desktop") then
		engine:SetFullscreen(4097)
	else
		print("Invalid fullscreen mode provided")
	end
end

function game:MousePosition()
	local mPos = engine:GetMousePosition()

	return { 
		x = mPos[1], 
		y = mPos[2] 
	}
end