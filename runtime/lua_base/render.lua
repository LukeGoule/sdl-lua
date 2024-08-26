local engine = require("engine")

render = { }

function render:Texture()
	new_texture = {
		name	= "UNKNOWN",
		id		= 0
	}

	return new_texture
end

function render:EmptyTexture()
	return self:Texture()
end

function render:OpenTexture(txName)
	new_texture = {
		name = txName,
		id = engine:LoadTexture(txName)
	};

	if (id == -1) then
		engine:Error("Failed to load texture with name " .. txName)
		return
	end

	return new_texture	
end

function render:UseTexture(tx)
	if (tx.id == -1) then return end

	engine:BindTexture(tx.id)
end

function float_clamp(f)
	if (f > 1.0) then
		f = 1.0
	end

	if (f < 0.0) then
		f = 0.0
	end

	return f
end

function render:SetDrawColour(r, g, b)
	r = float_clamp(r)
	g = float_clamp(g)
	b = float_clamp(b)

	engine:SetDrawColour(r, g, b)
end

function render:DrawRect(x, y, w, h)
	engine:DrawRect(x, y, w, h)
end