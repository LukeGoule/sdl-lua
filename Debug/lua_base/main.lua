--[[
ReadMe:
	Edit this file to do things, not engine.lua.
	This file is loaded by gam3_v1.exe as the main file and engine.lua
	is just a wrapper to make your life when coding much easier. Don't bother 
	editing it there's nothing interesting you can do in there that you can't do here.
]]

require("lua_base/g3Lua_v1")

local pepe = render:EmptyTexture() -- Preload an empty texture as a placeholder

local WIDTH  = 1600
local HEIGHT = 900
local FSCREEN = false
local RESISTANCE = 0.03
local PI = 3.141592653589793238
local tick = 0

local world = {
	gravity = 0.1,
	friction = {
		air = 0.01,
		surface = 0.05
	},
	maximums = {
		velocity = {
			x = 3,
			y = 3
		}
	}
}

local player = {
	onground = false,

	pos = {
		x = ( WIDTH / 2 ) - 12.5,
		y = 100,
	},
	dim = {
		w = 25,
		h = 25,
	},
	vel = {
		x = 0,
		y = 0
	},

	tracer_positions = {}
}

local nTracers = 15

for i=1,nTracers do
	player.tracer_positions[i+1] = { x = 0, y = 0 }
end

-- This hook runs during the loading of SDL and etc, but after OpenGL has loaded.
hooks:Add("MainLoad", "Engine_LoadResources", function ()
	--pepe = render:OpenTexture("res/pepe.png") -- Load up the "res/pepe.png" file, the data goes into the GPU's VRAM.

	if (FSCREEN) then game:Fullscreen("fullscreen") end -- activate fullscreen.

	-- do more things.
end)

function CalcAng(Pos0, Pos1)
	local result = E_Atan2(Pos0.y - Pos1.y, Pos0.x - Pos1.x)
	result = result + (3.141592653589793238 / 2) -- Add 90deg
	return result
end

-- This hook runs each frame, all render calls should be run here.
hooks:Add("MainRender", "Engine_Render", function ()
	-- Set the colour that everything should be drawn as.
	render:SetDrawColour(1.0, 1.0, 1.0)

	local mpos = game:MousePosition()

	-- Player movement controls
	if E_CheckKey( SDLK.SDL_SPACE ) and player.onground then
		player.vel.y = 5 -- Jump velocity (negative to move upwards)
		player.onground = false -- The player is no longer on the ground after jumping
	end

	if E_CheckKey( SDLK.SDLK_LEFT ) then
		player.vel.x = player.vel.x - 0.1
	end

	if E_CheckKey( SDLK.SDLK_RIGHT ) then
		player.vel.x = player.vel.x + 0.1
	end

	-- Update player position based on velocity
	player.pos.y = player.pos.y - player.vel.y
	player.pos.x = player.pos.x + player.vel.x

	for i=1,nTracers do
		local pos = player.tracer_positions[i]
		if pos and pos.x and pos.y then
			local time = tick / 60
			local r = math.abs(math.sin(time* 0.5) * 255) * (i/nTracers)
			local g = math.abs(math.sin(time * 0.5 + 2) * 255)  * (i/nTracers)
			local b = math.abs(math.sin(time * 0.5 + 4) * 255)  * (i/nTracers)

			render:SetDrawColour( r / 255, g / 255, b / 255 )
			render:DrawRect(pos.x - (player.dim.w / 2), pos.y - (player.dim.h / 2), player.dim.w, player.dim.h) 
		end
	end

	-- Render the player
	render:SetDrawColour(1,1,1)
	render:DrawRect(player.pos.x - (player.dim.w / 2), player.pos.y - (player.dim.h / 2), player.dim.w, player.dim.h)

	local dx = 0 -- delta x
	local dy = 0 -- delta y
	local friction = 0

	if player.onground then
		friction = world.friction.surface
	else
		friction = world.friction.air
	end

	-- Calculate horizontal velocity change
	if player.vel.x > 0 then 
		dx = friction
	else 
		dx = -friction 
	end

	-- Check if player is on the ground (middle of the screen)
	if (player.pos.y + (player.dim.h / 2)) >= HEIGHT / 2 then
		player.onground = true
		player.pos.y = (HEIGHT / 2) - (player.dim.h / 2) -- Correct the player's position to align with the floor
	else
		player.onground = false
	end

	-- Boundary checks to prevent the player from moving outside the screen
	if player.pos.x - (player.dim.w / 2) < 0 then
		player.pos.x = player.dim.w / 2 -- Align the player with the left edge
		player.vel.x = -player.vel.x -- Stop horizontal movement to the left
	elseif player.pos.x + (player.dim.w / 2) > WIDTH then
		player.pos.x = WIDTH - (player.dim.w / 2) -- Align the player with the right edge
		player.vel.x = -player.vel.x -- Stop horizontal movement to the right
	end

	-- Apply gravity or stop vertical movement
	if player.onground then 
		dy = 0 
		player.vel.y = 0
	else 
		dy = world.gravity -- Gravity effect
	end

	-- todo: clamp vel values.

	-- Apply velocity changes
	player.vel.x = player.vel.x - dx
	player.vel.y = player.vel.y - dy

	print( math.fmod( nTracers, tick + 1 ) )
	print( tick )

	player.tracer_positions[ math.fmod( tick + 1, nTracers ) ] = {
		x = player.pos.x,
		y = player.pos.y
	}

	-- tick.
	tick = tick + 1
	if tick > 1000 then tick = 0 end
end)

print(SDLK.RETURN)

-- This function has to be called to start the game.
game:Launch(WIDTH, HEIGHT)