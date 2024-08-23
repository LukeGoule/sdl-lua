--[[
ReadMe:
	Edit this file to do things, not engine.lua.
	This file is loaded by gam3_v1.exe as the main file and engine.lua
	is just a wrapper to make your life when coding much easier. Don't bother 
	editing it there's nothing interesting you can do in there that you can't do here.
]]

require("g3Lua_v1")

local pepe = render:EmptyTexture() -- Preload an empty texture as a placeholder

local WIDTH  = 1600
local HEIGHT = 900
local FSCREEN = false
local RESISTANCE = 0.03
local PI = 3.141592653589793238

local particles = {}
local iParticles = 0

function atan2(y,x)
	return math.atan(y / x)
end

-- This hook runs during the loading of SDL and etc, but after OpenGL has loaded.
hooks:Add("MainLoad", "Engine_LoadResources", function ()
	--pepe = render:OpenTexture("res/pepe.png") -- Load up the "res/pepe.png" file, the data goes into the GPU's VRAM.

	if (FSCREEN) then game:Fullscreen("fullscreen") end -- activate fullscreen.

	for i = 0, 3, 1
	do
		particles[i] = { 
			x = i*100, 
			y = 0.0,

			vel_dir = 0.0,
			vel_mag = 0.0
		}
		iParticles = iParticles + 1
	end

	print("Loaded!")
end)

function CalcAng(Pos0, Pos1)
	local result = E_Atan2(Pos0.y - Pos1.y, Pos0.x - Pos1.x)
	result = result + (3.141592653589793238 / 2) -- Add 90deg
	return result
end

function CalcDistance(Pos0, Pos1)
    local dx = Pos1.x - Pos0.x
    local dy = Pos1.y - Pos0.y
    return math.sqrt(dx * dx + dy * dy)
end

function AddImpulse(Particle, Direction, Magnitude)
	--[[	
	Particle.x = Particle.x + (math.sin(Direction)) * Magnitude--(RESISTANCE + 0.001)
	Particle.y = Particle.y + (-math.cos(Direction)) * Magnitude--(RESISTANCE + 0.001)
	]]

	if Particle.vel_dir == 0.0 then
		Particle.vel_dir = Direction
	end

	Particle.vel_dir = (Particle.vel_dir + Direction) / 2.0
	Particle.vel_mag = Particle.vel_mag + Magnitude -- Is this right?

	return Particle
end

function TickPosition(Particle)
	Particle.x = Particle.x + ((math.sin(Particle.vel_dir)) * Particle.vel_mag)--(RESISTANCE + 0.001)
	Particle.y = Particle.y + ((-math.cos(Particle.vel_dir)) * Particle.vel_mag)--(RESISTANCE + 0.001)

	if Particle.vel_mag >= RESISTANCE then
		Particle.vel_mag = Particle.vel_mag - RESISTANCE
	end
end

-- This hook runs each frame, all render calls should be run here.
hooks:Add("MainRender", "Engine_Render", function ()
	-- Set the colour that everything should be drawn as.
	render:SetDrawColour(1.0, 1.0, 1.0)

	local mpos = game:MousePosition()

	for i = 0, iParticles-1, 1
	do
		if CalcDistance( particles[i], mpos) > 1 then 
			local Direction = CalcAng(mpos, particles[i])

			AddImpulse(particles[i], Direction, RESISTANCE + ( math.random(1, 5 ) * 0.001 ) );
			TickPosition(particles[i])
		else
			particles[i].vel_mag = 0
			particles[i].vel_dir = -1
		end

		--print( "Expected: ", SDLK.SDL_SPACE )
		if E_CheckKey( SDLK.SDL_SPACE ) then
			print("PRESSING SPACE")
		end

		
		render:DrawBasicRect(particles[i].x - 5, particles[i].y - 5, 10, 10);
	end

end)

print(SDLK.RETURN)

-- This function has to be called to start the game.
game:Launch(WIDTH, HEIGHT)